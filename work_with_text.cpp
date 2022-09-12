#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h> 

#include "Generals_func\generals.h"
#include "include\config.h"
#include "include\log_info.h"
#include "include\work_with_text.h"

static int _Read_file_to_buffer (FILE *fpin, Text_info *text);

static int _Create_buffer       (FILE *fpin, Text_info *text);

static long _Get_file_size      (FILE *fpin);

static int _Read_to_buffer      (FILE *fpin, char *buf, long text_size);

static int _Get_count_lines     (const char *buf);

static int _Lines_initialize    (Text_info *text);

int Text_read (FILE *fpin, Text_info *text){
    assert (fpin != nullptr && "file fpin nullptr");
    assert (text != nullptr && "struct Text_info is nullptr");
    
    if (_Create_buffer (fpin, text)){
       Print_error ("Faild create buffer\n");
       return ERR_INIT_BUF;
    }

    if (_Read_file_to_buffer (fpin, text)){
        Print_error ("Faild reading from file\n");
        return ERR_INIT_BUF;
    }
    
    text->cnt_lines = _Get_count_lines (text->text_buf);   
    
    if (_Lines_initialize (text)){
        Print_error ("Failed to complete structure\n");
        return ERR_INIT_LINES;
    }

    return 0;
}

static int _Read_file_to_buffer (FILE *fpin, Text_info *text){
    assert (fpin != nullptr && "file fpin nullptr");

    int real_read_char = _Read_to_buffer (fpin, text->text_buf, text->text_size);    
    if (*(text->text_buf + real_read_char - 1) != '\n')                               
        *(text->text_buf + real_read_char) = '\n';        

    if(!feof (fpin)){
        Print_error ("Not all copy to buffer\n");
        return ERR_FILE_READING;
    }

    return 0;
}

static int _Create_buffer (FILE *fpin, Text_info *text){
    assert (fpin != nullptr && "File fpin nullptr");

    text->text_size = _Get_file_size (fpin);
    if (text->text_size == 1L){
        Print_error ("Not everything was read into the file");             
        return ERR_FILE_READING;
    }

    text->text_buf = (char *) calloc (text->text_size, sizeof(char));
    if (text->text_buf == nullptr){
        Print_error ("failed to allocted\n");
        return ERR_INIT_BUF;
    } 

    return 0;
} 

static long _Get_file_size (FILE *fpin){
    assert (fpin != nullptr && "file fpin nullptr");

    fseek (fpin, 0, SEEK_END);
    long file_size_type = ftell (fpin) + 1;
    
    rewind (fpin);
    return file_size_type;
}

static int _Read_to_buffer (FILE *fpin, char *buf, long text_size){
    fseek (fpin, 0, SEEK_SET);
    return fread (buf, sizeof (char), text_size, fpin);
}

static int _Get_count_lines (const char *buf){
    assert (buf != nullptr && "buffer is nullptr");
    
    const char* buffer = buf;

    int counter = 0;
    char ch = 0;

    while ((ch = *(buffer++)) != '\0'){     
        if (ch == '\n')
            counter++;
    }

    return counter;
}

static int _Lines_initialize (Text_info *text){
    text->lines = (Line*) calloc (text->cnt_lines, sizeof (Line));
    
    char *str_start = strtok (text->text_buf, "\r\n");

    Line *cur_line = text->lines;

    while (str_start != nullptr){
        cur_line->str = str_start;
        cur_line->len_str = strlen (cur_line->str);

        str_start = strtok (nullptr, "\r\n");
        
        cur_line++;
    }

    return 0;
}

int Text_write (FILE *fpout, int cnt_lines, Line *lines){
    assert (fpout != nullptr && "file fpout is nullptr");

    for (int cur_line = 0; cur_line < cnt_lines; cur_line++){
        if ((lines+cur_line)->str)
            fprintf (fpout, "%s\n", (lines+cur_line)->str);
        else 
            fprintf (fpout, "\n");
    }

    return 0;
}

void Qsort_lines (Line *lines, int left, int right, int (*comp) (const void *, const void *)){
    if (left >= right)
        return; 

    My_swap ((lines + left), (lines + (left + right)/2), sizeof (Line));

    int last = left;
    for (int i = left+1; i <= right; i++){ 
        if ((*comp) (lines + i, lines + left) < 0){
            last++;
            My_swap ((lines + last), (lines + i), sizeof (Line));
        }
    }

    My_swap ((lines + left), (lines + last), sizeof (Line));

    Qsort_lines (lines,     left, last - 1, comp);
    Qsort_lines (lines, last + 1,    right, comp);
}

int Direct_lex_comparator (Line *line1, Line *line2){
    assert (line1 != line2 && "line1 and line2 is equals");

    int empty_str1 = (line1->len_str == 0);
    int empty_str2 = (line2->len_str == 0);

    if (empty_str1 && empty_str2)
        return 0;
    
    if (!empty_str1 && empty_str2)
        return -1;
        
    if (empty_str1 && !empty_str2)
        return 1;

    char *str1 = line1->str;
    char *str2 = line2->str;

    do{ 
        str1 = Skip_not_alpha_dir_begin (str1);
        str2 = Skip_not_alpha_dir_begin (str2);

        if (*str1 == '\0')
            return 0;

    }while (*str1++ == *str2++);

    return *(str1 - 1) - *(str2 - 1);
}

int Reverse_lex_comparator (Line *line1, Line *line2){
    assert (line1 != line2 && "line1 and line2 is equals");

    int empty_str1 = (line1->len_str == 0);
    int empty_str2 = (line2->len_str == 0);

    if (empty_str1 && empty_str2)
        return 0;
    
    if (!empty_str1 && empty_str2)
        return -1;
        
    if (empty_str1 && !empty_str2)
        return 1;

    char *str1 = line1->str + line1->len_str;
    char *str2 = line2->str + line2->len_str;

    do{ 
        str1 = Skip_not_alpha_dir_back (str1, line1->str);     
        str2 = Skip_not_alpha_dir_back (str2, line2->str);

        if (str1 == line1->str || str2 == line2->str)
            return *str1 - *str2;

    }while (*str1-- == *str2--);

    return *(str1 + 1) - *(str2 + 1);
}

char *Skip_not_alpha_dir_begin (char *str){
    assert (str != nullptr && "str is nullptr");

    char ch = 0;
    while ((ch = *str) != '\0' && !isalpha(ch))
        str++;

    return str;
}

char *Skip_not_alpha_dir_back (char *str, char *str_start){
    assert (str != nullptr && "str is nullptr");

    while (str != str_start && !isalpha(*str))
        str--;

    return str;
}
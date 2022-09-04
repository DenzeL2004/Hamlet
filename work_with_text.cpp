#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h> 

#include "Generals_func\generals.h"
#include "headers\config.h"
#include "headers\log_info.h"
#include "headers\work_with_text.h"

int Text_read (FILE *fpin, Text_info *text){
    assert (fpin != nullptr && "file fpin nullptr");
    assert (text != nullptr && "struct Text_info is nullptr");
    
    if (Create_buffer (fpin, text)){
       Process_error (ERR_INIT_BUF, LOG_ARGS, "Faild create buffer\n");
       return ERR_INIT_BUF;
    }

    if (Read_file_to_buffer (fpin, text)){
        Process_error (ERR_INIT_BUF, LOG_ARGS, "Faild reading from file\n");
        return ERR_INIT_BUF;
    }
    
    text->cnt_lines = Get_count_lines (text->text_buf);   
    
    if (Lines_initialize (text)){
        Process_error (ERR_INIT_LINES, LOG_ARGS, "Failed to complete structure\n");
        return ERR_INIT_LINES;
    }

    return 0;
}

int Read_file_to_buffer (FILE *fpin, Text_info *text){
    assert (fpin != nullptr && "file fpin nullptr");

    int real_read_char = Read_to_buffer (fpin, text->text_buf, text->text_size);    
    if (*(text->text_buf + real_read_char - 1) != '\n')                               
        *(text->text_buf + real_read_char) = '\n';        

    if(!feof (fpin)){
        Process_error (ERR_FILE_READING, LOG_ARGS, "Not all copy to buffer\n");
        return ERR_FILE_READING;
    }

    return 0;
}

int Create_buffer (FILE *fpin, Text_info *text){
    assert (fpin != nullptr && "File fpin nullptr");

    text->text_size = Get_file_size (fpin);
    if (text->text_size == 1L){
        Process_error (ERR_FILE_READING, LOG_ARGS, "Not everything was read into the file");             
        return ERR_FILE_READING;
    }

    text->text_buf = (char *) calloc (text->text_size, sizeof(char));
    if (text->text_buf == nullptr){
        Process_error (ERR_INIT_BUF, LOG_ARGS, "failed to allocted\n");
        return ERR_INIT_BUF;
    } 

    return 0;
} 

long Get_file_size (FILE *fpin){
    assert (fpin != nullptr && "file fpin nullptr");

    fseek (fpin, 0, SEEK_END);
    long file_size_type = ftell (fpin) + 1;
    
    rewind (fpin);
    return file_size_type;
}

int Read_to_buffer (FILE *fpin, char *buf, long text_size){
    fseek (fpin, 0, SEEK_SET);
    return fread (buf, sizeof (char), text_size, fpin);
}

int Get_count_lines (const char *buf){
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

int Lines_initialize (Text_info *text){
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

void Sort_lines(Text_info *text, int (*comp) (const void *, const void *)){
    #ifdef MY_SORT
        Qsort_lines(text->lines, 0, text->cnt_lines - 1, (*comp));
    #else
        qsort (text->lines, text->cnt_lines, sizeof (Line), (*comp));
    #endif
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

int Hamlet_parsing (int argc, char *argv[], Options *flags){
    while (--argc > 0){
        argv++;

        if((*argv)[0] == '-'){
            if (!strcmp (*argv, "-in"))
                flags->Read_on_file = true;
            
            if (!strcmp (*argv, "-out"))
                flags->Write_on_file = true;

        }
    }

    return 0;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h> 

#include "headers\work_with_text.h"

int Text_read (FILE *fpin, Text_info *text){
    assert (fpin != nullptr && "File fpin nullptr");
    assert (text != nullptr && "Strut Text_info is nullptr");
    
    if (Create_buffer (fpin, text)){
       printf ("Faild create buffer\n");
       return ERR_INIT_BUF;
    }

    if (Read_file_to_buffer (fpin, text)){
        printf ("Faild reading from file\n");
        return ERR_INIT_BUF;
    }
    
    text->cnt_lines = Get_count_lines (text->text_buf);   
    
    if (Lines_initialize (text)){
        printf ("Failed to complete structure\n");
        return ERR_INIT_LINES;
    }

    return 0;
}

int Read_file_to_buffer (FILE *fpin, Text_info *text){
    assert (fpin != nullptr && "File fpin nullptr");

    int real_read_char = Read_to_buffer (fpin, text->text_buf, text->text_size);

    if (*(text->text_buf + real_read_char - 1) != '\n')
        *(text->text_buf + real_read_char) = '\n';        

    if(!feof (fpin)){
        printf ("Not all copy to buffer\n");
        return ERR_FILE_READING;
    }

    return 0;
}

int Create_buffer (FILE *fpin, Text_info *text){
    assert (fpin != nullptr && "File fpin nullptr");

    text->text_size = Get_file_size (fpin);
    if (text->text_size == 1L){
        printf ("Memory not allocated\n");
        return ERR_MEMORY_ALLOC;
    }

    text->text_buf = (char *) calloc (text->text_size, sizeof(char));
    if (text->text_buf == nullptr){
        printf ("Failed to allocted\n");
        return ERR_INIT_BUF;
    } 

    return 0;
} 

long Get_file_size (FILE *fpin){
    assert (fpin != nullptr && "File fpin nullptr");

    fseek (fpin, 0, SEEK_END);
    long file_size = ftell (fpin) + 1;
    
    rewind (fpin);
    return file_size;
}

int Read_to_buffer (FILE *fpin, char *buf, long text_size){
    fseek (fpin, 0, SEEK_SET);
    return fread (buf, sizeof (char), text_size, fpin);
}

int Get_count_lines (const char *buf){
    assert (buf != nullptr && "Buffer is nullptr");
    
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
    
    char ch = 0;
    char *buffer = text->text_buf;
    char *str_start = buffer, *str_end = nullptr;

    Line *cur_line = text->lines;

    while ((ch = *buffer++) != '\0'){
        if (ch == '\n'){
            *(buffer - 1) = '\0';

            str_end = buffer - 1;

            cur_line->len_str = str_end - str_start;

            cur_line->str = str_start;

            str_start = str_end + 1;

            cur_line++;
        }
    }

    return 0;
}

int Text_write (FILE *fpout, int cnt_lines, Line *lines){
    assert (fpout != nullptr && "File fpout is nullptr");

    for (int cur_line = 0; cur_line < cnt_lines; cur_line++){
        fprintf (fpout, "%s\n", (lines+cur_line)->str);
    }

    return 0;
}

//#define MY_SORT

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

    Swap_lines (lines, left, (left + right)/2);

    int last = left;
    for (int i = left+1; i <= right; i++){ 
        if ((*comp) (lines + i, lines + left) < 0){
            Swap_lines (lines, ++last, i);
        }
    }

    Swap_lines (lines, left, last);

    Qsort_lines (lines,     left, last - 1, comp);
    Qsort_lines (lines, last + 1,    right, comp);
}

void Swap_lines (Line *lines, int id1, int id2){
    Line temp = {};

    temp = lines[id1];
    lines[id1] = lines[id2];
    lines[id2] = temp;
}

int Lines_comp (Line *line1, Line *line2){
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
        str1 = Skip_not_alpha_left (str1);
        str2 = Skip_not_alpha_left (str2);

        if (*str1 == '\0')
            return 0;

    }while (*str1++ == *str2++);

    //printf ("|%s| |%s| |%c| |%c| %d\n", line1->str, line2->str, *str1, *str2, *(str1 - 1) - *(str2 - 1));

    return *(str1 - 1) - *(str2 - 1);
}

int Reverse_lines_comp (Line *line1, Line *line2){
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
        str1 = Skip_not_alpha_right (str1, line1->str);
        str2 = Skip_not_alpha_right (str2, line2->str);

        if (str1 == line1->str || str2 == line2->str)
            return *str1 - *str2;

    }while (*str1-- == *str2--);

    //printf ("|%s| |%s| |%c| |%c| %d\n", line1->str, line2->str, *str1, *str2, *(str1 - 1) - *(str2 - 1));

    return *(str1 + 1) - *(str2 + 1);
}

char *Skip_not_alpha_left (char *str){
    assert (str != nullptr && "str is nulptr");

    char ch = 0;
    while ((ch = *str) != '\0' && !isalpha(ch))
        str++;

    return str;
}

char *Skip_not_alpha_right (char *str, char *str_start){
    assert (str != nullptr && "str is nulptr");

    while (str != str_start && !isalpha(*str))
        str--;

    return str;
}
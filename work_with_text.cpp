#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h> 

#include "headers\work_with_text.h"

int Text_definition (FILE *fpin, Text_info *Text){
    assert (fpin != nullptr && "File is nullptr");
    assert (Text != nullptr && "Text is nullptr");
    
    if (Create_buffer (fpin, Text)){
       printf ("Faild create buffer\n");
       return INIT_BUF;
    }

    Text->cnt_lines = Get_count_lines (Text->text_buf);   
    
    if (Lines_initialize (Text)){
        printf ("Failed to complete structure\n");
        return INIT_LINES;
    }

    return 0;
}

int Create_buffer (FILE *fpin, Text_info *Text){
    assert (fpin != nullptr && "File is not nullptr");

    Text->text_size = Get_file_size (fpin);
    if (Text->text_size == 1L){
        printf ("Memory not allocated\n");
        return MEMORY_ALLOC;
    }

    Text->text_buf = (char *) calloc (Text->text_size, sizeof(char));
    if (Text->text_buf == nullptr){
        printf ("BUFF is nullptr\n");
        return INIT_BUF;
    } 

    int real_read_char = Write_to_buffer (fpin, Text->text_buf, Text->text_size);

    if (*(Text->text_buf + real_read_char - 1) != '\n')
        *(Text->text_buf + real_read_char) = '\n';        

    if(!feof (fpin)){
        printf ("Not all copy to buffer\n");
        return FILE_READING;
    }

    return 0;
} 

int Get_file_size (FILE *fpin){
    fseek (fpin, 0, SEEK_END);
    return ftell (fpin) + 1;
}

int Write_to_buffer (FILE *fpin, char *buf, int text_size){
    fseek (fpin, 0, SEEK_SET);
    return fread (buf, sizeof (char), text_size, fpin);
}

int Get_count_lines (const char *buf){
    assert (buf != nullptr && "File is not nullptr");
    
    int counter = 0, count_ch = 0;
    char ch = 0;

    while ((ch = *(buf + count_ch)) != '\0'){     
        if (ch == '\n')
            counter++;
        
        count_ch++;
    }

    return counter;
}

int Lines_initialize (Text_info *Text){
    Text->lines = (Line*) calloc (Text->cnt_lines, sizeof (Line));
    
    char ch = 0;
    int last_back_slash = -1, count_back_slash = 0, count_ch = 0;

    while ((ch = *(Text->text_buf + count_ch)) != '\0'){
        if (ch == '\n'){
            *(Text->text_buf + count_ch) = '\0';

            (Text->lines + count_back_slash)->len_str = count_ch - 1 - last_back_slash;

            last_back_slash = count_ch;

            (Text->lines + count_back_slash)->str = (Text->text_buf + count_ch - (Text->lines + count_back_slash)->len_str);

            count_back_slash++;
        }

        count_ch++;
    }

    return 0;
}

int Text_write (FILE *fpout, int cnt_lines, Line *lines){
    for (int cur_lines = 0; cur_lines < cnt_lines; cur_lines++){
        fprintf (fpout, "%s\n", (lines+cur_lines)->str);
    }

    return 0;
}

void Qsort_lines (Line *lines, int left, int right, int (*comp) (void *, void *)){
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

int Strcomp (Line *line1, Line *line2){
    assert (line1 != line2 && "PTRS is not equals");

    char *str1 = line1->str;
    char *str2 = line2->str;

    str1 = Skip_empty (str1);
    str2 = Skip_empty (str2);
    
    int empty_str1 = (*str1 == '\0');
    int empty_str2 = (*str2 == '\0');

    if (empty_str1 && empty_str2)
        return 0;
    
    if (!empty_str1 && empty_str2)
        return -1;
        
    if (empty_str1 && !empty_str2)
        return 1;

    for (; *str1 == *str2; str1++, str2++)
        if (*str1 == '\0')
            return 0;

    return *str1 - *str2;
}

int Strcomp_end (Line *line1, Line *line2){
    assert (line1 != line2 && "PTRS is not equals");

    char *str1 = line1->str + line1->len_str - 1;
    char *str2 = line2->str + line2->len_str - 1;
    
    int empty_str1 = (*str1 == '\0');
    int empty_str2 = (*str2 == '\0');

    if (empty_str1 && empty_str2)
        return  0;
    
    if (!empty_str1 && empty_str2)
        return -1;
        
    if (empty_str1 && !empty_str2)
        return  1;

    for (; *str1 == *str2; str1--, str2--)
        if (str1 == line1->str && str2 == line2->str)
            return 0;
    
    printf ("|%c| |%c| \n", *str1, *str2);
    return *str1 - *str2;
}

char *Skip_empty (char *str){
    assert (str != nullptr);

    char ch = 0;
    while ((ch = *str++) != '\0' && isspace(ch))
        continue;

    return str - 1;
}

char *Skip_empty (char *str){
    assert (str != nullptr);

    char ch = 0;
    while ((ch = *str++) != '\0' && isspace(ch))
        continue;

    return str - 1;
}
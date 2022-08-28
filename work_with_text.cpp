#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "headers\Readf.h"

int Text_definition (FILE *fpin, Text_info *Text){
    assert (fpin != NULL && "File is NULL");
    assert (Text != NULL && "Text is NULL");
    
    if (Buffering (fpin, Text))
       return -1;

    Text->cnt_lines = -1;
    Text->cnt_lines = Count_lines (Text->text_buf, Text->text_size);
    if (Text->cnt_lines == -1)
        return -1;   
    
    if (Line_initialization (Text))
        return -1;

    return 0;
}

int Buffering (FILE *fpin, Text_info *Text){
    assert (fpin != NULL && "File is not NULL");

    fseek (fpin, 0, SEEK_END);

    Text->text_size = ftell (fpin) + 1;
    if (Text->text_size == 1L){
        printf ("%s\n", "Memory not allocated");
        return -1;
    }

    Text->text_buf = (char *) malloc (Text->text_size);
    if (Text->text_buf == NULL){
        printf ("%s\n", "BUFF is NULL");
        return -1;
    } 

    fseek (fpin, 0, SEEK_SET);
    int real_read_char = fread (Text->text_buf, sizeof (char), Text->text_size, fpin);

    if (*(Text->text_buf + real_read_char - 1) != '\n')
        *(Text->text_buf + real_read_char) = '\n';        

    if(!feof (fpin)){
        printf ("%s\n", "Not all copy to buffer");
        return -1;
    }

    return 0;
} 

int Count_lines (char *buf, int text_size){
    assert (buf != NULL && "File is not NULL");
    
    int counter = 0, cnt_ch = 0;
    char ch = 0;
    while (cnt_ch < text_size){
        ch = *(buf + cnt_ch);        
        if (ch == '\n')
            counter++;
        
        cnt_ch++;
    }
    return counter;
}

int Line_initialization (Text_info *Text){
    Text->lines = (Line*) calloc (Text->cnt_lines, sizeof (Line));
    
    char ch = 0;
    int last_back_slash = -1, count_back_slash = 0, symbols_counter = 0;

    while (count_back_slash < Text->cnt_lines){
        ch = *(Text->text_buf + symbols_counter);

        if (ch == '\n'){
            *(Text->text_buf + symbols_counter) = '\0';

            (Text->lines + count_back_slash)->len_str = symbols_counter - 1 - last_back_slash;

            last_back_slash = symbols_counter;

            (Text->lines + count_back_slash)->str = (Text->text_buf + symbols_counter - (Text->lines + count_back_slash)->len_str);
            
            count_back_slash++;
        }
        
        symbols_counter++;
    }

    return 0;
}

int Text_write (FILE *fpout, int cnt_lines, Line *lines){
    for (int cur_lines = 0; cur_lines < cnt_lines; cur_lines++){
        fprintf (fpout, "%s\n", (lines+cur_lines)->str);
    }

    return 0;
}

void qsort_lines (Line *lines, int left, int right, int (*comp) (void *, void *)){
    if (left >= right)
        return; 
    
    swap_lines (lines, left, (left + right)/2);
    
    int last = left;
    for (int i = left+1; i <= right; i++){ 
        if (strcomp((lines + i)->str, (lines + left)->str) < 0){
            last++;
            swap_lines(lines, last, i);
        }
    }

    swap_lines (lines, left, last);

    qsort_lines(lines,     left, last - 1, comp);
    qsort_lines(lines, last + 1,    right, comp);
}

void swap_lines (Line *lines, int id1, int id2){
    Line temp = {};

    temp = lines[id1];
    lines[id1] = lines[id2];
    lines[id2] = temp;
}

int strcomp(const char *str1, const char *str2){
    str1 = skip_empty (str1);
    str2 = skip_empty (str2);
    
    return strcmp (str1, str2);   
}

const char *skip_empty(const char *str){
    assert (str != NULL);

    char ch = 0;
    while ((ch = *str++) != '\0' && (ch == ' ' || ch == '\t'))
        continue;;

    return str - 1;
}
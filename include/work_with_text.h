#ifndef _WORK_WITH_TEXT_H_
#define _WORK_WITH_TEXT_H_

#include "config.h"

#ifdef MY_SORT

    #define Qsort(text, comp)                                               \
        Qsort_lines((text)->lines, 0, (text)->cnt_lines - 1, *(comp));

#else

    #define Qsort(text, comp)                                               \
        qsort ((text)->lines, (text)->cnt_lines, sizeof (Line), *(comp));

#endif

struct Line{
    char *str = nullptr;
    int len_str = 0;
    int id = 0;
};

struct Text_info { 
    size_t text_size = 0; 
    char *text_buf = nullptr;
    
    size_t cnt_lines = 0;
    Line *lines = nullptr;
};

int Text_read    (FILE *fpin, Text_info *text); 

int Text_write   (FILE *fpout, int cnt_lines, Line *lines);

int Free_buffer  (Text_info *text);

void Qsort_lines (Line *lines, int left, int right, int (*comp) (const void *, const void *));

int Direct_lex_comparator  (void *line1, void *line2);

int Reverse_lex_comparator (void *line1, void *line2);

int Id_comparator          (Line *line1, Line *line2);

char *Skip_not_alpha_dir_begin  (char *str);

char *Skip_not_alpha_dir_back   (char *str, char *str_start);

#endif
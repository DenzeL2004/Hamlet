#ifndef _READF_H_
#define _READF_H_

struct Line{
    char *str;
    int len_str = 0;
};

struct Text_info {
    long int text_size;
    char *text_buf;
    int cnt_lines;
    Line *lines;
};

int Text_definition (FILE *fpin, Text_info *Text);

int Buffering (FILE *fpin, Text_info *Text);

int Count_lines (char *buf, int text_size);

int Line_initialization (Text_info *Text);

int Text_write (FILE *fpout, int cnt_lines, Line *lines);

void qsort_lines (Line *lines, int left, int right, int (*comp) (void *, void *));

void swap_lines (Line *lines, int id1, int id2);

int strcomp(const char *str1, const char *str2);

const char *skip_empty(const char *str);

#endif

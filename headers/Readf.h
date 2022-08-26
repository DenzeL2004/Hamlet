#ifndef _READF_H_
#define _READF_H_

typedef struct Info_line{
    char *str;
    int len_str = 0;
} Line;

int counter_lines (FILE *fpin);

void read_lines (FILE *fpin, Line *lines);

void write_lines (int cnt_lines, Line *lines);

void qsort_lines (Line *lines, int left, int right);

void swap_lines (Line *lines, int i, int j);

#endif

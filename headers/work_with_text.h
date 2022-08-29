#ifndef _WORK_WITH_TEXT_H_
#define _WORK_WITH_TEXT_H_

enum Errors {
    MEMORY_ALLOC = -1,
    INIT_BUF     = -2,
    FILE_READING = -3,
    INIT_LINES   = -4
};

struct Line{
    char *str = nullptr;
    int len_str = 0;
};

struct Text_info { 
    size_t text_size = 0; 
    char *text_buf = nullptr;
    
    size_t cnt_lines = 0;
    Line *lines = nullptr;
};

int Text_definition (FILE *fpin, Text_info *Text);

int Create_buffer (FILE *fpin, Text_info *Text);

int Get_file_size (FILE *fpin);

int Write_to_buffer (FILE *fpin, char *buf, int text_size);

int Get_count_lines (const char *buf);

int Lines_initialize (Text_info *Text);

int Text_write (FILE *fpout, int cnt_lines, Line *lines);

void Qsort_lines (Line *lines, int left, int right, int (*comp) (void *, void *));

void Swap_lines (Line *lines, int id1, int id2);

int Strcomp (Line *line1, Line *line2);

int Strcomp_end (Line *line1, Line *line2);

char *Skip_empty (char *str);

#endif

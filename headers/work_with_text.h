#ifndef _WORK_WITH_TEXT_H_
#define _WORK_WITH_TEXT_H_

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

int Text_read           (FILE *fpin, Text_info *text); 

int Read_file_to_buffer (FILE *fpin, Text_info *text);

int Create_buffer       (FILE *fpin, Text_info *text);

long Get_file_size      (FILE *fpin);

int Read_to_buffer      (FILE *fpin, char *buf, long text_size);

int Get_count_lines (const char *buf);

int Lines_initialize (Text_info *text);

int Text_write (FILE *fpout, int cnt_lines, Line *lines);

void Sort_lines  (Text_info *text, int (*comp) (const void *, const void *));

void Qsort_lines (Line *lines, int left, int right, int (*comp) (const void *, const void *));

void Swap_lines  (Line *lines, int id1, int id2);

int Direct_lex_comparator  (Line *line1, Line *line2); 

int Reverse_lex_comparator (Line *line1, Line *line2);

char *Skip_not_alpha_dir_begin  (char *str);

char *Skip_not_alpha_dir_back   (char *str, char *str_start);

int Hamlet_parsing (int argc, char *argv[], Options *flags);

#endif

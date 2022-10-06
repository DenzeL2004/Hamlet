#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h> 
#include <sys/stat.h>
#include <io.h>

#include "Generals_func\generals.h"
#include "include\config.h"
#include "include\log_info.h"
#include "include\work_with_text.h"

static int _Read_file_to_buffer (int fdin, Text_info *text);

static int _Create_buffer       (int fdin, Text_info *text);

static int _Work_with_bufer     (int fdin, Text_info *text);

static long _Get_file_size      (int fdin);

static int _Read_to_buffer      (int fdin, char *buf, long text_size);

static int _Get_count_lines     (const char *buf);

static int _Lines_initialize    (Text_info *text);

int Text_read (int fdin, Text_info *text){
    assert (fdin >= 0 && "fdin is negative number");
    assert (text != nullptr && "struct Text_info is nullptr");
    
    if (_Work_with_bufer (fdin, text)){
        Print_error (ERR_INIT_BUF);
        return ERR_INIT_BUF;
    }

    text->cnt_lines = _Get_count_lines (text->text_buf);   
    
    if (_Lines_initialize (text)){
        Print_error (ERR_INIT_LINES);
        return ERR_INIT_LINES;
    }

    return 0;
}

static int _Work_with_bufer (int fdin, Text_info *text){
    assert (fdin >= 0 && "fdin is negative number");
    assert (text != nullptr && "struct Text_info is nullptr");
    
    if (_Create_buffer (fdin, text)){
       Print_error (ERR_INIT_BUF);
       return ERR_INIT_BUF;
    }

    if (_Read_file_to_buffer (fdin, text)){
        Print_error (ERR_INIT_BUF);
        return ERR_INIT_BUF;
    }

    return 0;
}

static int _Read_file_to_buffer (int fdin, Text_info *text){
    assert (fdin >= 0 && "fdin is negative number");
    assert (text != nullptr && "struct Text_info is nullptr");

    int real_read_char = _Read_to_buffer (fdin, text->text_buf, text->text_size);    

    if(real_read_char == -1){
        Print_error (ERR_FILE_READING);
        return ERR_FILE_READING;
    }
   
    if (*(text->text_buf + real_read_char - 1) != '\n')                               
        *(text->text_buf + real_read_char) = '\n';   

    return 0;
}

static int _Create_buffer (int fdin, Text_info *text){
    assert (fdin >= 0 && "fdin is negative number");

    text->text_size = _Get_file_size (fdin) + 1;
    if (text->text_size == 1L){
        Print_error (ERR_FILE_READING);             
        return ERR_FILE_READING;
    }

    text->text_buf = (char*) calloc (text->text_size, sizeof(char));
    if (text->text_buf == nullptr){
        Print_error (ERR_INIT_BUF);
        return ERR_INIT_BUF;
    } 

    return 0;
} 

static long _Get_file_size (int fdin){
    assert (fdin >= 0 && "fdin is negative number");

    struct stat file_info = {};
    fstat (fdin, &file_info);

    return file_info.st_size;

    /*fseek (fpin, 0, SEEK_END);
    long file_size_type = ftell (fpin);
    
    rewind (fpin);
    return file_size_type;*/
}

static int _Read_to_buffer (int fdin, char *buf, long text_size){
    assert (fdin >= 0 && "fdin is negative number");
    assert (buf  != nullptr && "Buffer is nullptr");
    
    return read (fdin, buf, text_size);
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
    assert (text != nullptr && "Text info is nullptr");

    text->lines = (Line*) calloc (text->cnt_lines, sizeof (Line));
    
    char *buffer = text->text_buf, *str_start = text->text_buf;
    char ch = 0;

    Line *cur_line = text->lines;

    while ((ch = *(buffer++)) != '\0'){
        
        if (ch == '\n'){
            *(buffer - 1) = '\0';
            cur_line->str = str_start;

            str_start = buffer;

            cur_line->len_str = str_start - cur_line->str - 1;
            
            cur_line->id = cur_line - text->lines;
            cur_line++;
        }
    }

    return 0;
}

int Free_buffer (Text_info *text){
    assert (text != nullptr && "Text_info text is nullptr");

    if (text->text_buf == nullptr){
        Print_error (ERR_RE_ClEARING_MEMORY);

        return ERR_RE_ClEARING_MEMORY;
    }

    if (text->lines == nullptr){
        Print_error (ERR_RE_ClEARING_MEMORY);

        return ERR_RE_ClEARING_MEMORY;
    }

    free (text->text_buf);
    free (text->lines);

    text->text_buf = nullptr;
    text->lines    = nullptr;

    return 0;
}

int Text_write (FILE *fpout, int cnt_lines, Line *lines){
    assert (fpout != nullptr && "file fpout is nullptr");

    for (int cur_line = 0; cur_line < cnt_lines; cur_line++){
        fprintf (fpout, "%s\n", (lines+cur_line)->str);
    }

    return 0;
}

void My_qsort (void *data, size_t left, size_t right, size_t size_of_element, comp_t *comp){
    assert (data != nullptr && "data is nullptr");

    if (left >= right){
        return; 
    }

    char *_data = (char*) data;

    My_swap ((_data + left * size_of_element), (_data + (left + right)/2 * size_of_element), size_of_element);

    size_t last = left;
    for (size_t i = left + 1; i <= right; i++){ 
        if ((*comp) (_data + i * size_of_element, _data + left * size_of_element) < 0){
            last++;
            My_swap ((_data + last * size_of_element), (_data + i * size_of_element), size_of_element);
        }
    }

    My_swap ((_data + left * size_of_element), (_data + last * size_of_element), size_of_element);

    My_qsort (_data,     left, last, size_of_element, comp);
    My_qsort (_data, last + 1,    right, size_of_element, comp);
}

int Direct_lex_comparator (void *line1, void *line2){
    assert (line1 != nullptr && "line1 is not nullptr");
    assert (line2 != nullptr && "line2 is not nullptr");
    assert (line1 != line2 && "line1 and line2 is equals");

    Line *_line1 = (Line *) line1;
    Line *_line2 = (Line *) line2;

    char *str1 = _line1->str;
    char *str2 = _line2->str;

    str1 = Skip_not_alpha_dir_begin (str1);
    str2 = Skip_not_alpha_dir_begin (str2);

    int empty_str1 = (*str1 == '\0');
    int empty_str2 = (*str2 == '\0');

    if (empty_str1 && empty_str2)
        return 0;
    
    if (!empty_str1 && empty_str2)
        return -1;
        
    if (empty_str1 && !empty_str2)
        return 1;

    do{ 
        str1 = Skip_not_alpha_dir_begin (str1);
        str2 = Skip_not_alpha_dir_begin (str2);

        if (*str1 == '\0')
            return 0;

    }while (*str1++ == *str2++);

    return *(str1 - 1) - *(str2 - 1);
}

int Reverse_lex_comparator (void *line1, void *line2){
    assert (line1 != nullptr && "line1 is not nullptr");
    assert (line2 != nullptr && "line2 is not nullptr");
    assert (line1 != line2 && "line1 and line2 is equals");

    Line *_line1 = (Line *) line1;
    Line *_line2 = (Line *) line2;

    char *str1 = _line1->str + _line1->len_str;
    char *str2 = _line2->str + _line2->len_str;

    int empty_str1 = (_line1->len_str == 0);
    int empty_str2 = (_line2->len_str == 0);

    str1 = Skip_not_alpha_dir_back (str1, _line1->str);     
    str2 = Skip_not_alpha_dir_back (str2, _line2->str);

    if (str1 == _line1->str && *str1 == ' ') empty_str1 = 1;
    if (str2 == _line2->str && *str2 == ' ') empty_str2 = 1;

    if (empty_str1 && empty_str2)
        return 0;
    
    if (!empty_str1 && empty_str2)
        return -1;
        
    if (empty_str1 && !empty_str2)
        return 1;

    do{ 
        str1 = Skip_not_alpha_dir_back (str1, _line1->str);     
        str2 = Skip_not_alpha_dir_back (str2, _line2->str);

        if (str1 == _line1->str || str2 == _line2->str)
            return *str1 - *str2;

    }while (*str1-- == *str2--);

    return *(str1 + 1) - *(str2 + 1);
}

int Id_comparator (Line *line1, Line *line2){
    assert (line1 != nullptr && "line1 is not nullptr");
    assert (line2 != nullptr && "line2 is not nullptr");
    assert (line1 != line2 && "line1 and line2 is equals");

    return line1->id - line2->id;
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
    assert (str_start != nullptr && "str_start is not nullptr");

    while (str != str_start && !isalpha(*str))
        str--;

    return str;
}

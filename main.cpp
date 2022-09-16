#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "Generals_func\generals.h"
#include "include\work_with_text.h"
#include "include\log_info.h"
#include "include\config.h"

int main(int argc, char *argv[]) {
    Creat_empty_file ("logs/logs_info.txt");

    FILE *fpin = stdin, *fpout = stdout;
    Options options = {};    

    if (Parsing (argc, argv, &options)){
        Print_error (ERR_PARSING);
        return ERR_PARSING;    
    }
    
    Process_parsing (&options);
    
                                  fpin  = Open_file (options.file_input_name,  "r");
    if (options.file_output_name) fpout = Open_file (options.file_output_name, "w");
    
    Text_info text = {};

    if (Text_read (fpin, &text)){
        errno = EACCES;
        fprintf (stderr, "Structure Text_info was not read\n");
        perror ("Status error ");

        return ERR_TEXT_READING;
    }

    fclose (fpin);

    Qsort (&text, (int (*)(const void*, const void*)) Reverse_lex_comparator);

    if (Text_write (fpout, text.cnt_lines, text.lines)){
        errno = EIO;
        fprintf (stderr, "Not everything was written to file");
        perror ("Status error ");

        return ERR_WRITING;
    }  

    Qsort (&text, (int (*)(const void*, const void*)) Id_comparator);
    
    if (Text_write (fpout, text.cnt_lines, text.lines)){
        errno = EIO;
        fprintf (stderr, "Not everything was written to file");
        perror ("Status error ");

        return ERR_WRITING;
    }

    Free_buffer (&text);

    fclose (fpout);

    errno = 0;
    return 0;
}

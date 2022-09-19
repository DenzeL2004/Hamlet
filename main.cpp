#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <locale.h>

#include "Generals_func\generals.h"
#include "include\work_with_text.h"
#include "include\log_info.h"
#include "include\config.h"

int main(int argc, char *argv[]) {

    setlocale(LC_ALL, "Russian");

    #ifdef ERR_REPORTS

        if (Open_logs_file()){
            fprintf (stderr, "Logs file does not open in main\n");
            return ERR_FILE_OPEN;
        }

    #endif

    Options options = {};    

    if (Parsing (argc, argv, &options)){
        Print_error (ERR_PARSING);
        return ERR_PARSING;    
    }
    
    Process_parsing (&options);
    
    FILE *fpin = nullptr, *fpout = nullptr;
    
                            fpin  = Open_file (options.file_input_name,  "r");
    options.write_on_file ? fpout = Open_file (options.file_output_name, "w") : fpout = stdout;
    
    Text_info text = {};

    if (Text_read (fpin, &text)){
        errno = EACCES;
        fprintf (stderr, "Structure Text_info was not read\n");
        perror ("Status error ");

        return ERR_TEXT_READING;
    }

    if (Close_file (fpin)){
        Print_error (ERR_FILE_CLOSE);
        return ERR_FILE_CLOSE;
    }
    
    Qsort (&text, (comp_t*) Direct_lex_comparator);

    if (Text_write (fpout, text.cnt_lines, text.lines)){
        errno = EIO;
        fprintf (stderr, "Not everything was written to file");
        perror ("Status error ");

        return ERR_WRITING;
    }
    
    Qsort (&text, (comp_t*) Reverse_lex_comparator);

    if (Text_write (fpout, text.cnt_lines, text.lines)){
        errno = EIO;
        fprintf (stderr, "Not everything was written to file");
        perror ("Status error ");

        return ERR_WRITING;
    }  

    Qsort (&text, (comp_t*) Id_comparator);
    
    if (Text_write (fpout, text.cnt_lines, text.lines)){
        errno = EIO;
        fprintf (stderr, "Not everything was written to file");
        perror ("Status error ");

        return ERR_WRITING;
    }

    Free_buffer (&text);

    if (Close_file (fpout)){
        Print_error (ERR_FILE_CLOSE);
        return ERR_FILE_CLOSE;
    }
    
    #ifdef ERR_REPORTS

        if (Close_logs_file()){
            fprintf (stderr, "Logs file does not close in main\n");
            return ERR_FILE_CLOSE;
        }
    
    #endif

    return 0;
}

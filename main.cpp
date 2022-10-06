#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <fcntl.h>

#include "Generals_func\generals.h"
#include "include\work_with_text.h"
#include "include\log_info.h"
#include "include\config.h"

int main(int argc, const char *argv[]) {

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
    

    int fdin = Open_file_discriptor (options.file_input_name, O_RDONLY);
    
    FILE *fpout = nullptr;
    options.write_on_file ? fpout = Open_file_ptr (options.file_output_name, "w") : fpout = stdout;
    
    Text_info text = {};

    if (Text_read (fdin, &text)){
        errno = EACCES;
        fprintf (stderr, "Structure Text_info was not read\n");
        perror ("Status error ");

        return ERR_TEXT_READING;
    }

    if (Close_file_discriptor (fdin)){
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

    if (Close_file_ptr (fpout)){
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

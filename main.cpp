#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "Generals_func\generals.h"
#include "include\work_with_text.h"
#include "include\log_info.h"
#include "include\config.h"

int main(int argc, char *argv[]) {
    FILE *fpin = stdin, *fpout = stdout;
    Options options = {};    

    if (Parsing (argc, argv, &options)){
        fprintf (stderr, "Parsing was not succes\n");
        return ERR_PARSING;    
    }
    
    Process_parsing (&options);
    
                                  fpin =  Open_file (options.file_input_name, "rb");
    if (options.file_output_name) fpout = Open_file (options.file_output_name, "w");
    
    Text_info text = {};

    if (Text_read (fpin, &text)){
        errno = EACCES;
        fprintf (stderr, "Structure Text_info was not read\n");
        perror ("Status error ");

        return ERR_TEXT_READING;
    }

    fclose (fpin);

    Qsort (&text, (int (*)(const void*, const void*)) Direct_lex_comparator);
    
    if (Text_write (fpout, text.cnt_lines, text.lines)){
        errno = EIO;
        fprintf (stderr, "Not everything was written to file");
        return ERR_WRITING;
    }

    free (text.text_buf);
    free (text.lines);

    fclose (fpout);

    errno = 0;
    return 0;
}

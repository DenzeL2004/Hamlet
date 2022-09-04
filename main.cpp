#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "Generals_func\generals.h"
#include "headers\work_with_text.h"
#include "headers\log_info.h"

int main(int argc, char *argv[]) {
    Options flags = {};    
    Hamlet_parsing (argc, argv, &flags);

    FILE *fpin = stdin, *fpout = stdout;

    if (flags.Read_on_file){
        char *name_file = argv[2];
        fpin = Open_file (name_file, "rb");
    }

    Text_info text = {};

    if (Text_read (fpin, &text)){
        Process_error (ERR_TEXT_READING, LOG_ARGS, "Structure Text_info was not read");
        return ERR_TEXT_READING;
    }

    fclose (fpin);

    Sort_lines (&text, (int (*)(const void*, const void*)) Direct_lex_comparator);
    
    if (flags.Write_on_file){
        char *name_file = argv[4];
        fpout = Open_file (name_file, "w");
    }
    
    if (Text_write (fpout, text.cnt_lines, text.lines)){
        Process_error (ERR_WRITING, LOG_ARGS, "Not everything was written to file");
        return ERR_WRITING;
    }

    free (text.text_buf);
    free (text.lines);
    fclose (fpout);
}

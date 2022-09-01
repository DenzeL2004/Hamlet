#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "headers\work_with_text.h"

const char *name_file = "hamlet.txt";

int main() {
    FILE *fpin = fopen (name_file, "r");
    if (!fpin){
        errno = ENOENT;
        Process_error (__LINE__, __FILE__);
        return ERR_FILE_OPEN;
    }

    Text_info text = {};

    if (Text_read (fpin, &text)){
        printf ("Structure Text_info was not read\n");
        return ERR_TEXT_READING;
    }

    fclose (fpin);

    Sort_lines (&text, (int (*)(const void*, const void*)) Lines_comp);
    
    FILE *fpout = fopen ("ans.txt", "w");
    if (!fpout){
        errno = ENOENT;
        Process_error (__LINE__, __FILE__);
        return ERR_FILE_OPEN;
    }
    
    if (Text_write (fpout, text.cnt_lines, text.lines)){
        errno = EIO;
        Process_error (__LINE__, __FILE__);
        return ERR_WRITING;
    }

    free (text.text_buf);
    free (text.lines);
    fclose (fpout);
}

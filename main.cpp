#include <stdio.h>
#include <stdlib.h>

#include "headers\work_with_text.h"

const char *name_file = "hamlet.txt";

int main() {
    FILE *fpin = fopen (name_file, "r");
    if (!fpin){
        printf ("File fpin is not open\n");
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
    if (!fpin){
        printf ("File fpout is not open\n");
        return ERR_FILE_OPEN;
    }
    
    if (Text_write (fpout, text.cnt_lines, text.lines)){
        printf ("Program can't write in file\n");
        return ERR_WRITING;
    }

    free (text.text_buf);
    free (text.lines);
    fclose (fpout);
}

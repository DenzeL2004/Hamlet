#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "headers\work_with_text.h"

const char *name_file = "song.txt";

int main() {

    //  -> file names for otutput 

    // log_file_open(name, ...)

    FILE *fpin = fopen (name_file, "rb");      //
    if (!fpin){
        errno = ENOENT;
        Process_error ();
        return ERR_FILE_OPEN;
    }

    /*char s1[] = "Goodfdhgfhg", s2[] = "Buyoo00000000000000000000000oooo";
    printf ("%s %s\n", s1, s2);
    My_swap (s1, s2, sizeof(char));
    printf ("%s %s\n", s1, s2);*/

    int a = 9, b = 0;
    printf ("%d %d\n", a, b);
    My_swap (&a, &b, sizeof(int));
    printf ("%d %d\n", a, b);

    Text_info text = {};

    if (Text_read (fpin, &text)){
        printf ("Structure Text_info was not read\n");
        return ERR_TEXT_READING;
    }

    fclose (fpin);

    Sort_lines (&text, (int (*)(const void*, const void*)) Direct_lex_comparator);
    
    FILE *fpout = fopen ("ans.txt", "w");
    if (!fpout){
        errno = ENOENT;
        Process_error ();
        return ERR_FILE_OPEN;
    }
    
    if (Text_write (fpout, text.cnt_lines, text.lines)){
        errno = EIO;
        Process_error ();
        return ERR_WRITING;
    }

    free (text.text_buf);
    free (text.lines);
    fclose (fpout);
}

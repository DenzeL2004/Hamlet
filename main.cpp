#include <stdio.h>
#include <stdlib.h>

#include "headers\work_with_text.h"

const char *name_file = "song.txt";

int main() {
    FILE *fpin = fopen (name_file, "r");
    if (!fpin){
        printf ("File is not open\n");
        return FILE_READING;
    }

    Text_info Text = {};
    Text_definition (fpin, &Text);
    fclose (fpin);

    Qsort_lines (Text.lines, 0, Text.cnt_lines - 1, (int (*)(void*, void*)) Strcomp_end);
    
    FILE *fpout = fopen ("ans.txt", "w");
    Text_write (fpout, Text.cnt_lines, Text.lines);

    free (Text.text_buf);
    free (Text.lines);
    fclose (fpout);
}

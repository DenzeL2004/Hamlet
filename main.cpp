#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "headers\Readf.h"

const char *name_file = "hamlet.txt";

int main() {
    FILE *fpin = fopen (name_file, "r");
    if (!fpin){
        printf ("File is not open\n");
        return -1;
    }

    Text_info Text = {};
    Text_definition (fpin, &Text);
    fclose (fpin);
    
    qsort_lines (Text.lines, 0, Text.cnt_lines - 1, (int (*)(void *, void *))strcomp);
    
    FILE *fpout = fopen ("ans.txt", "w");
    Text_write (fpout, Text.cnt_lines, Text.lines);

    fclose (fpout);
}

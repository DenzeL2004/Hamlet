#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "headers\Readf.h"

const char name_file[] = "hamlet.txt";

int main() {
    FILE *fpin = fopen (name_file, "r");

    if (!fpin){
        printf ("File is not open\n");
        return -1;
    }

    Line *lines = nullptr;

    int cnt_lines = counter_lines (fpin);
    lines = (Line*) calloc (cnt_lines, sizeof (Line));
    fclose (fpin);

    fpin = fopen (name_file, "r");

    read_lines (fpin, lines);

    printf ("Count reading lines %d\n", cnt_lines);
    //write_lines (cnt_lines, lines);

    qsort_lines (lines, 0, cnt_lines-1);
    write_lines (cnt_lines, lines);

    fclose (fpin);
    free (lines);
}

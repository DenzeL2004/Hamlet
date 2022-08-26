#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "headers\Readf.h"

const int Maxlen = 1000;

int counter_lines (FILE *fpin){
    assert (fpin != NULL);

    int cnt_lines = 0;
    char *cur_str = (char*) calloc (Maxlen, sizeof (char));

    while (fgets (cur_str, Maxlen, fpin))
        cnt_lines++;

    free (cur_str);
    return cnt_lines;
}

void read_lines (FILE *fpin, Line *lines){
    assert (fpin != NULL);

    char *cur_str = (char*) calloc (Maxlen, sizeof (char));

    while (fgets (cur_str, Maxlen, fpin)){
        (*lines).str = (char*) calloc (Maxlen, sizeof (char));   ///->

        strcpy ((*lines).str, cur_str);
        (*lines).len_str = strlen (cur_str);
        lines++;
    }

    free (cur_str);
}

void write_lines (int cnt_lines, Line *lines){
    for (int cur_line = 0; cur_line < cnt_lines; cur_line++){
        printf ("%s", (lines + cur_line)->str);
    }
}

void qsort_lines (Line *lines, int left, int right){

    if (left >= right)
        return;
    swap_lines (lines, left, (left+ right)/2);

    int last = left;
    for (int i = left+1; i <= right; i++)
        if (strcmp ((lines + i)->str, (lines + left)->str) < 0)
            swap_lines (lines, ++last, i);

    swap_lines (lines, left, last);

    qsort_lines (lines, left, last-1);
    qsort_lines (lines, last+1, right);
}

void swap_lines (Line *lines, int i, int j){
    Line temp = {};

    temp = lines[i];
    lines[i] = lines[j];
    lines[j] = temp;
}


#ifdef MY_SORT
        Qsort_lines(text->lines, 0, text->cnt_lines - 1, (*comp));
    #else
        qsort (text->lines, text->cnt_lines, sizeof (Line), (*comp));
#endif
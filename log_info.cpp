#include <stdio.h>

#include "headers\log_info.h"

int Process_error(int status_error, const char *file_name, const char *func_name, int line, const char *message) {
    fprintf (stderr, "WTF!!!\n");
    fprintf (stderr, "Something went wrong the way you wanted\n\n");
    
    fprintf (stderr, "In file %s\n", file_name);
    fprintf (stderr, "In function %s\n", func_name);
    fprintf (stderr, "In line %d\n", line);

    fprintf (stderr, "Error status: %d\n", status_error);
    fprintf (stderr, "Programm message: %s\n\n", message);        

    return 0;                                                       
}
#include <stdio.h>

#include "include\log_info.h"
#include "Generals_func\generals.h"

int _Print_error (const char *file_name, const char *func_name, int line, int error) {
    FILE *fp_logs = Open_file ("logs/logs_info.txt", "a");

    fprintf (fp_logs, "WTF!!!\n");
    fprintf (fp_logs, "Something went wrong the way you wanted\n\n");
    
    fprintf (fp_logs, "In file %s\n", file_name);
    fprintf (fp_logs, "In function %s\n", func_name);
    fprintf (fp_logs, "In line %d\n\n", line);


    fprintf (fp_logs, "Function returned an error value: %d\n", error);
    fprintf (fp_logs, "Error value: "); 

    switch (error){
    
        case ERR_MEMORY_ALLOC:
            fprintf (fp_logs, "Memory is not allocated\n\n");
            break;

        case ERR_INIT_BUF:
            fprintf (fp_logs, "Buffer has not been initialized\n\n");
            break;
        
        case ERR_FILE_READING:
            fprintf (fp_logs, "Not all data was read from the file\n\n");
            break;

        case ERR_INIT_LINES:
            fprintf (fp_logs, "Lines does not initialize\n\n");
            break;

        case ERR_TEXT_READING:
            fprintf (fp_logs, "Text does not initialize\n\n");
            break;

        case ERR_FILE_OPEN:
            fprintf (fp_logs, "File is not open\n\n");
            break;

        case ERR_WRITING:
            fprintf (fp_logs, "Data writing failed\n\n");
            break;

        case ERR_MANY_ARGUMENTS:
            fprintf (fp_logs, "Too many values on the command line\n\n");
            break;

        case ERR_PARSING:
            fprintf (fp_logs, "Parsing does not succeeded\n\n");
            break;
    
        default:
            fprintf (fp_logs, "ACHTUNG!!! OMG!!! WTF!!! Unknown error\\n");
            break;
    }       

    fclose (fp_logs);

    return 0;                                                       
}
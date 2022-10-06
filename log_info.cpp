#include <stdio.h>
#include <time.h>

#include "include\log_info.h"
#include "Generals_func\generals.h"

FILE *fp_logs = nullptr;

int Open_logs_file (){
    
    fp_logs = Open_file_ptr ("logs/logs_info.txt", "a");

    if (!fp_logs){
        fprintf (stderr, "Logs file does not open\n");

        return ERR_FILE_OPEN;
    }

    return 0;
}

int _Print_error (const char *file_name, const char *func_name, int line, int error) {
    
    time_t seconds = time (NULL) + 3 * 60* 60;

    fprintf (fp_logs, "=================================================\n\n");

    fprintf (fp_logs, "WTF!!!\n");
    fprintf (fp_logs, "Something went wrong the way you wanted\n\n");

    fprintf (fp_logs, "Error time: %s\n", asctime(gmtime(&seconds)));
    
    fprintf (fp_logs, "In file %s\n", file_name);
    fprintf (fp_logs, "In function %s\n", func_name);
    fprintf (fp_logs, "In line %d\n\n", line);

    fprintf (fp_logs, "Function returned an error value: %d\n", error);
    fprintf (fp_logs, "Error value: %s\n", Process_error (error));
    fprintf (fp_logs, "=================================================\n\n\n");

    return 0;                                                       
}

const char* Process_error(int error){
    
    switch (error){
    
        case ERR_MEMORY_ALLOC:
            return "Memory is not allocated\n";
            break;

        case ERR_INIT_BUF:
            return "Buffer has not been initialized\n";
            break;
        
        case ERR_FILE_READING:
            return  "Not all data was read from the file\n";
            break;

        case ERR_INIT_LINES:
            return  "Lines does not initialize\n";
            break;

        case ERR_TEXT_READING:
            return "Text does not initialize\n";
            break;

        case ERR_FILE_OPEN:
            return "File is not open\n";
            break;

        case ERR_FILE_CLOSE:
            return "File is not close";

        case ERR_WRITING:
            return "Data writing failed\n";
            break;

        case ERR_MANY_ARGUMENTS:
            return "Too many values on the command line\n";
            break;

        case ERR_PARSING:
            return "Parsing does not succeeded\n";
            break;
    
        default:
            return "ACHTUNG!!! OMG!!! WTF!!! Unknown error\n";
            break;
    }       

    return "ACHTUNG!!! OMG!!! WTF!!! Unknown error\n";
}

int Close_logs_file (){

    time_t seconds = time (NULL)  + 3 * 60* 60;;   

    fprintf (fp_logs, "Time close logs file: %s\n\n", asctime(gmtime(&seconds))); 

    if (Close_file_ptr (fp_logs)){
        fprintf (stderr, "Logs file does not close\n");

        return ERR_FILE_OPEN;
    }

    return 0;
}
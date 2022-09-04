#ifndef _LOG_INFO_H_
#define _LOG_INFO_H_

    #define LOG_ARGS                                      \
        __FILE__, __PRETTY_FUNCTION__, __LINE__

int Process_error(int status_error, const char *file_name, const char *func_name, int line, const char *message);

#endif
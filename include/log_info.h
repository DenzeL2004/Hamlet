#ifndef _LOG_INFO_H_
#define _LOG_INFO_H_

#include "config.h"

#define LOG_ARGS                                        \
    __FILE__, __PRETTY_FUNCTION__, __LINE__

#ifdef ERR_REPORTS

    #define Print_error(error)                         \
        _Print_error (LOG_ARGS, (error));

#else

    #define Print_error(message) ""

#endif

int _Print_error (const char *file_name, const char *func_name, int line, int error);

#endif
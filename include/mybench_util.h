#ifndef MYBENCH_MYBENCH_UTIL_H
#define MYBENCH_MYBENCH_UTIL_H

#include <stdarg.h>

enum loglevel {
    ERROR_LEVEL=       0,
    WARNING_LEVEL=     1,
    INFORMATION_LEVEL= 2
};

class Mybench_util
{
public:
    Mybench_util();
    virtual ~Mybench_util();
    virtual void log_info(const char *format, ...);
    virtual void log_warning(const char *format, ...);
    virtual void log_error(const char *format, ...);
    virtual void print_log_to_file(enum loglevel level, const char *format, va_list args);
};

#endif //MYBENCH_MYBENCH_UTIL_H

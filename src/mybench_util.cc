#include "mybench_util.h"
#include <iostream>
#include <sstream>


static const uint MAX_LOG_BUFFER_SIZE= 1024;

Mybench_util::Mybench_util(){}
Mybench_util::~Mybench_util(){}

void Mybench_util::log_info(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  print_log_to_file(INFORMATION_LEVEL, format, args);
  va_end(args);

}

void Mybench_util::log_warning(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  print_log_to_file(WARNING_LEVEL, format, args);
  va_end(args);
}

void Mybench_util::log_error(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  print_log_to_file(ERROR_LEVEL, format, args);
  va_end(args);
}

void Mybench_util::print_log_to_file(enum loglevel level, const char *format, va_list args)
{
  char   buff[MAX_LOG_BUFFER_SIZE];
  size_t length;

  length= vsprintf(buff, format, args);
  std::ostringstream s;
  if (level == ERROR_LEVEL)
    s << "[Error] ";
  else if (level == WARNING_LEVEL)
    s << "[Warning] ";
  else
    s << "[Info] ";
  s << buff << std::endl;
  std::cout << s.str();
}

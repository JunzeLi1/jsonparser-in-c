#ifndef __CLOG_H__
#define __CLOG_H__

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#define debugf(...)  \
  do{                                                   \
	  printf("[%s:%d] ",__FILE__,__LINE__);             \
	  printf(__VA_ARGS__);                              \
  }while(false)                                         \

#define printf_red(...)  __mu_printf_any_color("\x1b[31m", __VA_ARGS__)
#define printf_green(...)  __mu_printf_any_color("\x1b[32m", __VA_ARGS__)
#define printf_yellow(...)  __mu_printf_any_color("\x1b[33m", __VA_ARGS__)
#define printf_blue(...)  __mu_printf_any_color("\x1b[34m", __VA_ARGS__)
#define printf_magenta(...)  __mu_printf_any_color("\x1b[35m", __VA_ARGS__)
#define printf_cyan(...)  __mu_printf_any_color("\x1b[36m", __VA_ARGS__)

#define log_int(n)   printf("%s == %d\n", (#n), (n)) 
#define log_char(ch)   printf("%s == '%c'\n",(#ch),ch)
#define log_str(s)   printf("%s == \"%s\"\n",(#s),s)
#define log_addr(addr)        printf("%s == %p\n", (#addr),      ((void*)(addr)))
#define log_float(n)   printf("%s == %0.16f\n",(#n),n)
#define log_bool(condition)   printf("%s == %s\n", (#condition),((condition)? "true" : "false"))

#define __mu_printf_any_color(color, ...)\
do {   \
    if(isatty(STDOUT_FILENO)){\
		fprintf(stdout, "%s", color);\
	}\
    fprintf(stdout, __VA_ARGS__); \
	if(isatty(STDOUT_FILENO)){\
		fprintf(stdout,"%s","\x1b[0m");\
	}\
} while(false)


#endif /* end of include guard: __CLOG_H__ */
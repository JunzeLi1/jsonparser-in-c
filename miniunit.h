#include <stdbool.h>
#include <string.h>
#include "clog.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#ifndef __MINIUNIT_H__
#define __MINIUNIT_H__

#define __MU_SUCCESS 0

#define mu_start() int __mu_line_number = __MU_SUCCESS

#define mu_end()	return __mu_line_number\

#define mu_check(condition)\
	do{\
		if(__mu_line_number == __MU_SUCCESS && !(condition)){\
			__mu_line_number = __LINE__;\
		}\
		}while(false)

#define mu_check_strings_equal(s1, s2) mu_check(strcmp((s1), (s2)) == 0)


#define mu_run(function)   \
    do {                        \
		int functionOutput = function();             \
		if(functionOutput == __MU_SUCCESS){                             \
			printf_green("Test passed: %s\n",(#function));               \
		}                    \
		else{                      \
			printf_red("Test failed: %s at line %d\n", (#function),functionOutput);                              \
		}                   \
	}while(false)




#endif /* end of include guard: __MINIUNIT_H__ */
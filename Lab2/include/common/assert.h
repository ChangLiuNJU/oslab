#ifndef __ASSERT_H__
#define __ASSERT_H__

#include "lib.h"

int abort(const char *, int);

/* assert: 断言条件为真，若为假则蓝屏退出 */
#define assert(cond) do { \
	if (!(cond)) {	\
		abort(__FILE__, __LINE__);	\
	}	\
} while(0)

#define panic(info) do { \
	disable_interrupt(); \
	printf("\n\33[1;31msystem panic\33[0m in function \"%s\", line %d, file \"%s\":\n", \
		__FUNCTION__, __LINE__, __FILE__); \
	printf(info); \
	while(1) wait_for_interrupt(); \
} while(0)
#endif



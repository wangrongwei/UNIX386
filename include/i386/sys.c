
/*
 * 实现sys.h下相关系统调用，采用宏实现部分相关系统调用
 *
 */


#include "sys.h"


/* __NR_##name ---> __NR_fork */

#define _create_systemcall(type name)\
type name(void)\
{\
	long __res;\
	__asm__ volatile ("int $0x80"\
		: "=a"(__res)\
		: "0"(__NR_##name));\
	if(__res >= 0){\
		return (type)__res;\
	}
	else{\
		return -1;\
	}\
}














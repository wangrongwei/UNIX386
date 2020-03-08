#ifndef __VARGS_H__
#define __VARGS_H__


/*
 * __builtin***()相关函数是gcc的内置变量和函数
 */
typedef __builtin_va_list va_list;
#define va_start(ap,last)	(__builtin_va_start(ap,last))
#define va_arg(ap,type)		(__builtin_va_arg(ap,type))
#define va_end(ap)

/*
__builtin_expect
__builtin_clz
__builtin_bswap32 0xaabbccdd---> 0xddccbbaa
__builtin_constant_p
__sync_synchronize
__builtin_fetch_and_add
__builtin_ctz
__builtin_bswap16

*/

/*  优化分支预测*/
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0) /* x很可能为假 */






#endif



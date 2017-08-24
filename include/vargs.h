#ifndef __VARGS_H__
#define __VARGS_H__


/*
 * __builtin***()相关函数是gcc的内置变量和函数
 */
typedef __builtin_va_list va_list;
#define va_start(ap,last)	(__builtin_va_start(ap,last))
#define va_arg(ap,type)		(__builtin_va_arg(ap,type))
#define va_end(ap)




#endif



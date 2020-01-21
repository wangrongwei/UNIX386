#ifndef __DEBUG_H__
#define __DEBUG_H__


#include "console.h"
#include "vargs.h"

/* 开启调试功能 */
#define _DEBUG 0

void printk(const char *format,...);
volatile void panic(const char * s);


#endif




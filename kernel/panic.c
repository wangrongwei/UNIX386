
#include "console.h"
#include "string.h"
#include "vargs.h"
#include "debug.h"

/*
 * panic
 */
volatile void panic(const char * s)
{
	printk("Kernel panic: %s\n\r",s);
	/* TODO 待实现打印调用栈 */
	for(;;);
}




#include "console.h"
#include "string.h"
#include "vargs.h"
#include "debug.h"

volatile void panic(const char * s)
{
	printk("Kernel panic: %s\n\r",s);
	for(;;);
}



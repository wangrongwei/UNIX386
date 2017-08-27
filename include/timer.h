#ifndef __TIMER_H__
#define __TIMER_H__

#include <interrupt.h>
//extern pt_regs;

void init_timer(unsigned int frequency);
void timer_callback(pt_regs *regs);


#endif




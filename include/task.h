
#ifndef __TASK_H__
#define __TASK_H__

void kernel_thread_create(int (*fn)(void *),void *args);

void task_tty();

#endif



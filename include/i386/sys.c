
/*
 * 实现sys.h下相关系统调用，采用宏实现部分相关系统调用
 *
 */


#include "sys.h"


#define filename #sys.h


/*
 * 先使用这7个系统调用，system_call_table会被kernel.asm中调用
 */
syscall_ptr system_call_table[]={sys_setup,sys_exit,sys_fork,
sys_read,sys_write,sys_open,sys_close};




int sys_setup()
{
	return 0;
}

int sys_exit()
{
	return 0;
}

 /* 汇编实现，执行fork时被调用 */
int sys_fork()
{
	return 0;
}

int sys_read()
{
	return 0;
}

int sys_write(char *       buf, int len, task_struct *task)
{
	tty_write(&tty_table[task->nr_tty], buf, len);
	return 0;
}

int sys_open()
{
	return 0;
}

int sys_close()
{
	return 0;
}



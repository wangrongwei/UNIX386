/*
 * 关于调度
 */
#include "schedule.h"
#include "task_struct.h"


#define PAGE_SIZE 4096

/* 在kernel.asm需要用到 */
long user_stack[PAGE_SIZE >> 2]={0};

long *_stack_top = &user_stack[PAGE_SIZE >> 2];



/*
 * 调度初始化，启动进程0
 */
void schedule_init(void)
{
	int i;
	// 第一步   在gdt表后边加上ldt和tss

	// 将tss挂接到TR寄存器


	// 将LDT挂接到LDTR寄存器


	// 初始化时钟


	//设置系统调度总入口
	//set_system_gate(0x80,&system_call);
	

}




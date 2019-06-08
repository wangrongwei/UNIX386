
/*
 * 关于进程调度相关实现
 */


#include "schedule.h"
#include "task_struct.h"
#include <i386/sys.h>
#include <i386/system.h>
#include <descriptor.h>


extern tss_struct;
extern gdt_struct_t gdt_list[];
extern union task_union task_tables[];

#define PAGE_SIZE 4096

/* 在kernel.asm需要用到 */
long user_stack[PAGE_SIZE >> 2]={0};

long *_stack_top = &user_stack[PAGE_SIZE >> 2];

/* 一个全局指针，指向当前正在执行的进程的task_struct */
struct task_struct *current = &(init_task.task); 
extern syscall_ptr system_call_table[];



/*
 * 调度初始化，启动进程0
 */
void schedule_init(void)
{
	printk("scheduler initial.\n");
	/* 在gdt表后边加上进程0的tss和ldt */
	//&(init_task.task.tss)
	//&(init_task.task.ldt)
	set_tssldt2_gdt(FIRST_TASKTSS_INDEX,&(init_task.task.tss),0x89);
	set_tssldt2_gdt(FIRST_TASKLDT_INDEX,&(init_task.task.ldt),0x82);
	__asm__("pushfl ; andl $0xffffbfff,(%esp) ; popfl");
	/* 将tss挂接到TR寄存器 */
	ltr(0);
	
	/* 将LDT挂接到LDTR寄存器 */
	lldt(0);

	/* 初始化时钟 */
	init_timer(HZ);

	/* 设置系统调度总入口 */
	set_system_gate(0x80,&system_call);
	printk("scheduler initial end...\n");

}

/*
 * 重新调度
 */
void reschedule(void)
{

	return;
}

/*
 * 准备init0
 */
void init0_ready()
{
	/* 设置栈 */
	union task_union *init0_point = task_tables;
	init0_point->task.tss.esp = (long)&(task_tables[0].task)+4096;
	init0_point->task.tss.eip = (long)init0_body;
	init0_point->task.tss.eflags = 0x1202;
}


/* 初始化一个函数作为init进程程序体 */
void init0_body(void)
{
	int i=0;
	i++;
	while(1);
	return;
}


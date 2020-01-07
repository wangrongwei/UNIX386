
/*
 * 关于进程调度相关实现
 */


#include "schedule.h"
#include "task_struct.h"
#include <i386/sys.h>
#include <i386/system.h>
#include <descriptor.h>
#include "unistd.h"

extern tss_struct;
extern gdt_struct_t gdt_list[];
extern syscall_ptr system_call_table[];

#define PAGE_SIZE 4096
#define STACK_SIZE 8192

struct task_struct* task_tables[NR_TASKS] = {&(init_task.task),};

/* 在kernel.asm需要用到 */
long user_stack[PAGE_SIZE >> 2]={0};
char kernel_stack[PAGE_SIZE << 1]={0};

long *_stack_top = &user_stack[PAGE_SIZE >> 2];
long kernel_stack_top;

/* 一个全局指针，指向当前正在执行的进程的task_struct */
struct task_struct *current = &(init_task.task); 


/*
 * 调度初始化，启动进程0
 */
void schedule_init(void)
{
	printk("scheduler init!\n");
	/* 在gdt表后边加上进程0的tss和ldt */
	//set_tssldt2_gdt(FIRST_TASKTSS_INDEX, &(init_task.task.tss), 0xe9);
	//set_tssldt2_gdt(FIRST_TASKLDT_INDEX, &(init_task.task.ldt), 0xe2);
	unsigned int base = &(init_task.task.tss);
	unsigned int limit = &(init_task.task.tss) + sizeof(init_task.task.tss);
	set_tssldt2_gdt(FIRST_TASKTSS_INDEX, base, limit, P_SET | (DPL3 << 5) | TYPE_TSS_NOTBUSY);
	//set_tssldt2_gdt(FIRST_TASKLDT_INDEX, &(init_task.task.ldt), P_SET | (DPL3 << 5) | TYPE_LDT);
	__asm__ __volatile__("pushfl ; andl $0xffffbfff,(%esp) ; popfl");
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
 * 调度
 */
void schedule(void)
{
	if(current != NULL){

	}
}

/*
 * 准备init0
 */
void init0_ready()
{
	/* 设置栈 */
	struct task_struct *init0_point = task_tables[0];
	init0_point->tss.esp0 = (long)&(task_tables[0]) + PAGE_SIZE;
	init0_point->tss.eip = (long)thread_init0;
	init0_point->tss.eflags = 0x3202; /* 设置IOPL=3 */
}


/* 初始化一个函数作为init进程程序体 */
void thread_init0(void)
{
	int i=0;
	i++;
	while(1){
		printk("A\n");
	};
}


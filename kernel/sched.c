
/*
 * 关于进程调度相关实现
 */


#include "schedule.h"
#include "task_struct.h"
#include <i386/sys.h>



extern tss_struct;


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
	int i;
	printk("scheduler initial.\n");
	// 在gdt表后边加上进程0的tss和ldt
	set_gdt(FIRST_TASKTSS_INDEX,&(init_task.task.tss),sizeof(tss_struct),0x89,0);
	set_gdt(FIRST_TASKLDT_INDEX,&(init_task.task.ldt),sizeof(tss_struct),0x82,0);
	printk("scheduler initial..\n");
	// 将tss挂接到TR寄存器
	ltr(0);
	
	// 将LDT挂接到LDTR寄存器
	lldt(0);

	// 初始化时钟
	init_timer(HZ);

	// 设置系统调度总入口
	set_system_gate(0x80,&system_call);
	printk("scheduler initial...\n");

}

/*
 * 重新调度
 */
void reschedule(void)
{

	return;
}


/*
 * 改变进程权限：从0到3特权级 ss,sp,eflags,cs,ip
 */
#define move_to_user_mode() \
__asm__ ("movl %%esp,%%eax\n\t"\
	"pushl $0x17\n\t"\ 	/* 压入ss */
	"pushl %eax\n\t"\ 	/* 压入sp */
	"pushfl\n\t"\ 		/* 压入eflags */
	"movl $0x0f\n\t"\	/* 压入cs */
	"pushl $1f\n\t"\	/* 压入ip */
	"iret\n\t"\
	"1:\tmovl $0x17,%%eax\n\t"\	/* RPL=3 TI=1(ldt) SEL=1 */
	"movl %%ax,%%ds\n\t"\
	"movl %%ax,%%es\n\t"\
	"movl %%ax,%%fs\n\t"\
	"movl %%ax,%%gs\n\t"\
	 : : :"ax")






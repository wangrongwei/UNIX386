
#ifndef __SCHEDULE_H__
#define __SCHEDULE_H__

/*
 * 定义与调度相关的变量
 *
 */

extern char kernel_stack[];
extern long kernel_stack_top;

void schedule_init(void);
void reschedule(void);
void schedule(void);
void init0_ready(void);
void thread_init0(void);

/* 在kernel.asm文件中，与_page_dir是相同的含义 */
#define pg_dir (0)

/*
 * 改变进程权限：从0到3特权级 ss,sp,eflags,cs,ip "lret\n"\
 * 压入ss sp eflags cs ip 
 * movl $0x17,%%eax	RPL=3 TI=1(ldt) SEL=1
 */
#define move_to_user_mode() \
__asm__ __volatile__("movl %%esp,%%eax\n\t"\
	"pushl $0x23\n\t" 	/* 压入ss */\
	"pushl %%eax\n\t" 	/* 压入sp */\
	"pushfl\n\t" 		/* 压入eflags */\
	"pushl $0x23\n\t"	/* 压入cs */\
	"pushl $1f\n\t"		/* 压入ip */\
	"iret\n"\
	"1:\tmovl $0x2b,%%eax\n\t"\
	"movw %%ax,%%ds\n\t" \
	"movw %%ax,%%es\n\t" \
	"movw %%ax,%%fs\n\t" \
	"movw %%ax,%%gs" \
	 :::"ax")

extern struct task_struct *current;

#endif




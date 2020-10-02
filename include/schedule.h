
#ifndef __SCHEDULE_H__
#define __SCHEDULE_H__

#include <task_struct.h>
#include <interrupt.h>
#include <i386/linkage.h>

/* 定义与调度相关的变量 */
#define STACK_SIZE 8192

enum schedr_index {
	PRI_SCHEDR = 0,
	MAX_SCHEDR
};

extern char kernel_stack[];
extern long kernel_stack_top;

void schedule_init(void);
void reschedule(void);
void schedule(void);
void init0_ready(void);
void thread_init1(void);
/* 线程cpu_idle应该是init0 */
void thread_cpu_idle(void);
void init_thread(void);

void save_context(pt_regs *regs);
struct task_struct * FASTCALL(__switch_to(struct task_struct *prev, struct task_struct *next));


/* 在kernel.asm文件中，与_page_dir是相同的含义 */
#define pg_dir (0)

/*
 * 改变进程权限：从0到3特权级 ss,sp,eflags,cs,ip "lret\n"\
 * 压入ss sp eflags cs ip 
 * movl $0x17,%%eax	RPL=3 TI=1(ldt) SEL=1
 */
void move_to_user_mode(void);
void init(void);
extern struct task_struct *current;
extern union task_union* task_tables[];

/*
 * 调度器
 */
struct scheduler {
	char *name;
	void (*fn)(void);	
};

 /*
  * 切换进程
  * 保存eflags
  */
#define switch_to(prev, next, last) do {					\
	unsigned long esi, edi;						\
	__asm__ __volatile__("pushfl\n\t"	/* 保存eflags */		\
		     "pushl %%ebp\n\t"					\
		     "movl %%esp,%0\n\t"	/* 保存ESP */		\
		     "movl %5,%%esp\n\t"	/* 切换ESP */		\
		     "movl $1f,%1\n\t"		/* 保存EIP */		\
		     "pushl %6\n\t"		/* 切换EIP */		\
		     "jmp __switch_to\n"				\
		     "1:\t"			/* 下一次切换回到1 */		\
		     "popl %%ebp\n\t"					\
		     "popfl"						\
		     :"=m" (prev->tss.esp0),"=m" (prev->tss.eip),	\
		      "=a" (last),"=S" (esi),"=D" (edi)			\
		     :"m" (next->tss.esp0),"m" (next->tss.eip),		\
		      "2" (prev), "d" (next));				\
} while (0);

#endif




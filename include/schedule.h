
#ifndef __SCHEDULE_H__
#define __SCHEDULE_H__

/*
 * 定义与调度相关的变量
 *
 */

void schedule_init(void);
void reschedule(void);

/* 在kernel.asm文件中，与_page_dir是相同的含义 */
#define pg_dir (0)

/*
 * 改变进程权限：从0到3特权级 ss,sp,eflags,cs,ip
 */
#define move_to_user_mode() \
__asm__("movl %%esp,%%eax\n\t"\
	"pushl $0x17\n\t" 	/* 压入ss */\
	"pushl %%eax\n\t" 	/* 压入sp */\
	"pushfl\n\t" 		/* 压入eflags */\
	"movl $0x0f\n\t"	/* 压入cs */\
	"pushl $1f\n\t"		/* 压入ip */\
	"iret\n\t"\
	"1:\tmovl $0x17,%%eax\n\t"	/* RPL=3 TI=1(ldt) SEL=1 */\
	"movl %%ax,%%ds\n\t" \
	"movl %%ax,%%es\n\t" \
	"movl %%ax,%%fs\n\t" \
	"movl %%ax,%%gs" \
	 :::"ax")

#endif





#ifndef __TASK_STRUCT_H__
#define __TASK_STRUCT_H__

#include "descriptor.h"
#include "page.h"



/*进程的五种状态 */
#define TASK_RUNNING 0
#define TASK_INTERRUPTIBLE 1
#define TASK_UNINTERRUPTIBLE 2
#define TASK_ZOMBIE 3
#define TASK_STOPPED 4


/* 在32位编译器下int 占4个字节 */

/* tss_struct是一个进程的tss描述符 */
struct tss_struct{
	long backlink;
	long esp0;
	long ss0;
	long esp1;
	long ss1;
	long esp2;
	long ss2;

	long cr3;
	long eip;
	long flags;

	long eax;
	long ecx;
	long edx;
	long ebx;
	long esp;
	long ebp;
	long esi;
	long edi;
	long es;
	long cs;
	long ss;
	long ds;
	long fs;
	long gs;
	long ldt;
	long trap;
	long iobase;

};

/* task_struct：进程的描述符 */
struct task_struct{
	int state; /* 进程当前的状态 */
	long counter;
	long priority; /* 优先级 */
	long signal;


	long blocked;
	int exit_code;
	unsigned long start_code,end_code,end_data,brk,start_stack;
	long pid,father,pgrp,session,leader; /* pid:进程ID号 */
	unsigned short uid,euid,suid;
	unsigned short gid,egid,sgid;

	long alarm;
	long utime,stime,cutime,cstime,start_time;

	int tty; /* 自设备号 */

	/* 接下来需要三个结构体代码该进程执行的对象 */
	// 目前先不设置

	/* 该进程的局部段描述符 */
	struct ldt_struct_t ldt[3];
	/* 该进程的tss */
	struct tss_struct tss;

};



/* 利用联合体，一个task_union正好占4K大小 */
union task_union {
	struct task_struct task;
	char stack[PAGE_SIZE]; //PAGE_SIZE == 4096
};

/* 填充一个task0，是所有进程的父进程 */
struct task_struct INIT_TASK={
	.state = TASK_RUNNING;
 	.counter = 0;
	.priority = 0; /* 优先级 */
	.signal = 0;


	.blocked = 0;
	.exit_code = 0;

	.start_code = 0;
	.end_code = 0;
	.end_data = 0;
	.brk = ;
	.start_stack = 0;
	
	.pid = 0;/* pid:进程ID号 */
	.father = 0;
	.pgrp = 0;
	.session = 0;
	.leader = 0; 

	.uid = 0;
	.euid = 0;
	.suid = 0;
	.gid = 0;
	.egid = 0;
	.sgid = 0;

	.alarm = 0;
	.utime = 0;
	.stime = 0;
	.cutime = 0;
	.cstime = 0;
	.start_time = 0;

	.tty = 0; /* 自设备号 */

	/* 填充ldt */
	{
		/* 填充ldt[0] */
		{
			.limit0 = ;
			.base0 = ;
			.base1 = ;
			.access = ;
			.limit1 = ;
			.GD_DB_L_AVL = ;
			.base2 = ;
		}
		
		/* 填充ldt[1] */
		{
			.limit0 = ;
			.base0 = ;
			.base1 = ;
			.access = ;
			.limit1 = ;
			.GD_DB_L_AVL = ;
			.base2 = ;
		}
		
		/* 填充ldt[2] */
		{
			.limit0 = ;
			.base0 = ;
			.base1 = ;
			.access = ;
			.limit1 = ;
			.GD_DB_L_AVL = ;
			.base2 = ;
		}
	
	}

	/* 填充tss */
}

static union task_union init_task={INIT_TASK,};


#endif



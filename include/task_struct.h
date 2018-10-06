/*
 * 与进程相关的结构体定义头文件，包括ldt/tss定义
 *
 * author:<wangrongwei>
 *
 */
#ifndef __TASK_STRUCT_H__
#define __TASK_STRUCT_H__

#include "schedule.h"

#include "descriptor.h"
#include "page.h"

#define NR_TASKS 128

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
	/* 填充ldt[0] */
	ldt[0].limit0 = 0;
	ldt[0].base0 = 0;
	ldt[0].base1 = 0;
	ldt[0].access = 0;
	ldt[0].limit1 = 0;
	ldt[0].GD_DB_L_AVL = 0;
	ldt[0].base2 = 0;
	
	/* 填充ldt[1] */
	ldt[1].limit0 = 0;
	ldt[1].base0 = 0;
	ldt[1].base1 = 0;
	ldt[1].access = 0;
	ldt[1].limit1 = 0;
	ldt[1].GD_DB_L_AVL = 0;
	ldt[1].base2 = ;
	
	/* 填充ldt[2] */
	ldt[2].limit0 = 0;
	ldt[2].base0 = 0;
	ldt[2].base1 = 0;
	ldt[2].access = 0;
	ldt[2].limit1 = 0;
	ldt[2].GD_DB_L_AVL = 0;
	ldt[2].base2 = 0;
	

	/* 填充tss */
	tss.backlink = 0;
	tss.esp0 = PAGE_SIZE + (long)&init_task;
	tss.ss0 = 0x10;
	tss.esp1 = 0;
	tss.ss1 = 0;
	tss.esp2 = 0;
	tss.ss2 = 0;

	tss.cr3 = pg_dir;//进程与内核使用同一个页目录表
	tss.eip = 0;
	tss.flags = 0;

	tss.eax = 0;
	tss.ecx = 0;
	tss.edx = 0;
	tss.ebx = 0;
	
	tss.esp = 0;
	tss.ebp = 0;
	tss.esi = 0;
	tss.edi = 0;
	tss.es = 0x17;
	tss.cs = 0x17;
	tss.ss = 0x17;
	tss.ds = 0x17;
	tss.fs = 0x17;
	tss.gs = 0x17;
	tss.ldt = _LDT(0); //需要实现
	tss.trap = 0x80000000;
	tss.iobase = 0;//暂时先留着

};

static union task_union init_task={INIT_TASK,};



#endif



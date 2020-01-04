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

/* 第一个进程在gdt中的位置 */
#define FIRST_TASKTSS_INDEX 6
#define FIRST_TASKLDT_INDEX 7

// 将tss或者ldt相对于第一个tss或者第一个ldt进行编码

// TR [---selector(16 bit, visible)---/---base(hidden)---/---limit(hidden)---]
#define _TSS(n) ((((unsigned long)n) << 4) + (FIRST_TASKTSS_INDEX << 3))
#define _LDT(n) ((((unsigned long)n) << 4) + (FIRST_TASKLDT_INDEX << 3)) 


/* 将tss和ldt地址加载到TR和LDTR寄存器 */
/* 根据TR寄存器的位图可以看出需要加载一个TSS选择子到TR寄存器 */
#define ltr(n) __asm__("ltr %%ax"::"a" (_TSS(n)))
#define lldt(n) __asm__("lldt %%ax"::"a" (_LDT(n)))


/* 在32位编译器下int 占4个字节 */

/* 
 * tss_struct是一个进程的tss描述符（TSS结构参考Intel 80386 programmer's
 * manual 1986） 
 */
struct tss_struct{
	long backlink;/* back link to previous TSS */
	long esp0;
	long ss0;/* 16位，后续若为16位寄存器则占用低16位，高16位为0 */
	long esp1;
	long ss1;
	long esp2;
	long ss2;

	long cr3;
	long eip;
	long eflags;

	long eax;
	long ecx;
	long edx;
	long ebx;
	
	long esp;
	long ebp;
	long esi;
	long edi;
	long es;/* 以下皆为16位寄存器 */
	long cs;
	long ss;
	long ds;
	long fs;
	long gs;
	unsigned int ldt;/* 只取16位 */
	/* tss最后32位中，0：trap，16-31：iobase,其他位为0 */
	long trap;
	long iobase;

};

/* task_struct：进程的描述符 */
struct task_struct{
	long state; /* 进程当前的状态 */
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

	int nr_tty; /* 自设备号 */

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
#if 0
struct task_struct INIT_TASK={
	.state = TASK_RUNNING,
 	.counter = 0,
	.priority = 0, /* 优先级 */
	.signal = 0,


	.blocked = 0,
	.exit_code = 0,

	.start_code = 0,
	.end_code = 0,
	.end_data = 0,
	.brk = 0,
	.start_stack = 0,
	
	.pid = 0,/* pid:进程ID号 */
	.father = 0,
	.pgrp = 0,
	.session = 0,
	.leader = 0, 

	.uid = 0,
	.euid = 0,
	.suid = 0,
	.gid = 0,
	.egid = 0,
	.sgid = 0,

	.alarm = 0,
	.utime = 0,
	.stime = 0,
	.cutime = 0,
	.cstime = 0,
	.start_time = 0,

	.tty = 0, /* 自设备号 */

	/* 填充ldt */
	/* 填充ldt[0] */
	ldt[0].limit0 = 0,
	ldt[0].base0 = 0,
	ldt[0].base1 = 0,
	ldt[0].access = 0,
	ldt[0].limit1 = 0,
	ldt[0].GD_DB_L_AVL = 0,
	ldt[0].base2 = 0,
	
	/* 填充ldt[1] */
	ldt[1].limit0 = 0,
	ldt[1].base0 = 0,
	ldt[1].base1 = 0,
	ldt[1].access = 0,
	ldt[1].limit1 = 0,
	ldt[1].GD_DB_L_AVL = 0,
	ldt[1].base2 = 0,
	
	/* 填充ldt[2] */
	ldt[2].limit0 = 0,
	ldt[2].base0 = 0,
	ldt[2].base1 = 0,
	ldt[2].access = 0,
	ldt[2].limit1 = 0,
	ldt[2].GD_DB_L_AVL = 0,
	ldt[2].base2 = 0,
	

	/* 填充tss */
	tss.backlink = 0,
	tss.esp0 = PAGE_SIZE + (long)&init_task,
	tss.ss0 = 0x10,
	tss.esp1 = 0,
	tss.ss1 = 0,
	tss.esp2 = 0,
	tss.ss2 = 0,

	tss.cr3 = pg_dir,//进程与内核使用同一个页目录表
	tss.eip = 0,
	tss.flags = 0,

	tss.eax = 0,
	tss.ecx = 0,
	tss.edx = 0,
	tss.ebx = 0,
	
	tss.esp = 0,
	tss.ebp = 0,
	tss.esi = 0,
	tss.edi = 0,
	tss.es = 0x17,
	tss.cs = 0x17,
	tss.ss = 0x17,
	tss.ds = 0x17,
	tss.fs = 0x17,
	tss.gs = 0x17,
	tss.ldt = _LDT(0), //需要实现
	tss.trap = 0x80000000,
	tss.iobase = 0,//暂时先留着

};
#endif




/* 使用宏对INIT_TASK进行初始化 */
/*  
{0,0},{0x9f,0xc0fa00},{0x9f,0xc0f200},
*/
#define INIT_TASK \
{\
/* state */ TASK_RUNNING,\
/* counter */ 0,\
/* priority */ 0,\
/* signal */ 0,\
\
/* blocked */ 0,\
/* exit_code */ 0,\
\
/* start_code */ 0,\
/* end_code */ 0,\
/* end_data */ 0,\
/* brk */ 0,\
/* start_stack */ 0,\
\
/* pid */ 0,\
/* father */ 0,\
/* pgrp */ 0,\
/* session */ 0,\
/* leader */ 0, \
\
/* uid */ 0,\
/* euid */ 0,\
/* suid */ 0,\
/* gid */ 0,\
/* egid */ 0,\
/* sgid */ 0,\
\
/* alarm */ 0,\
/* utime */ 0,\
/* stime */ 0,\
/* cutime */ 0,\
/* cstime */ 0,\
/* start_time */ 0,\
\
/* tty */ 0,\
\
	/* 填充ldt（此处需要改进） */\
	/* 填充ldt[0] */\
{\
/* ldt[0].limit0 */	 {0,\
/* ldt[0].base0 */	 0,\
/* ldt[0].base1 */	 0,\
/* ldt[0].access */	 0,\
/* ldt[0].limit1 */	 0,\
/* ldt[0].GD_DB_L_AVL */ 0,\
/* ldt[0].base2 */	0},\
\
	/* 填充ldt[1] */\
/* ldt[1].limit0 */	 {0x9f,\
/* ldt[1].base0 */	 0,\
/* ldt[1].base1 */	 0,\
/* ldt[1].access */	 0xfa,\
/* ldt[1].limit1 */	 0,\
/* ldt[1].GD_DB_L_AVL */ 0x0c,\
/* ldt[1].base2 */	0},\
\	
	/* 填充ldt[2] */\
/* ldt[2].limit0 */	 {0x9f,\
/* ldt[2].base0 */	 0,\
/* ldt[2].base1 */	 0,\
/* ldt[2].access */	 0xf2,\
/* ldt[2].limit1 */	 0,\
/* ldt[2].GD_DB_L_AVL */ 0x0c,\
/* ldt[2].base2 */	0},\
},\
\
	/* 填充tss */\
{\
/* tss.backlink */	0,\
/* tss.esp0 */	PAGE_SIZE + (long)&init_task,\
/* tss.ss0 */	0x10,\
/* tss.esp1 */	0,\
/* tss.ss1 */	0,\
/* tss.esp2 */	0,\
/* tss.ss2 */	0,\
\
/* 进程与内核使用同一个页目录表 */ \
/* tss.cr3 */	pg_dir,\
/* tss.eip */	0,\
/* tss.eflags */0x3202,\
\
/* tss.eax */	0,\
/* tss.ecx */	0,\
/* tss.edx */	0,\
/* tss.ebx */	0,\
\
/* tss.esp */	4096,\
/* tss.ebp */	0,\
/* tss.esi */	0,\
/* tss.edi */	0,\
/* tss.es */	0x2f,\
/* tss.cs */	0x27,\
/* tss.ss */	0x2f,\
/* tss.ds */	0x2f,\
/* tss.fs */	0x2f,\
/* tss.gs */	0x2f,\
\
/* 需要实现 */ \
/* tss.ldt */	_LDT(0),\
/* tss.trap */	0x80000000,\
\
/* 暂时先留着 */ \
/* tss.iobase */ 0,\
},\
}


//TODO 此处有bug，如果这样写需要INIT_TASK是一个宏，反正不能是一个变量
static union task_union init_task={INIT_TASK,};



#endif



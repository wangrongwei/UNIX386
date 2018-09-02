
#ifndef __TASK_STRUCT_H__
#define __TASK_STRUCT_H__

#include "descriptor.h"


/* 在32位编译器下int 占4个字节 */

/* tss_struct是一个进程的tss描述符 */
struct tss_struct{
	unsigned int backlink;
	unsigned int esp0;
	unsigned int ss0;
	unsigned int esp1;
	unsigned int ss1;
	unsigned int esp2;
	unsigned int ss2;

	unsigned int cr3;
	unsigned int eip;
	unsigned int flags;

	unsigned int eax;
	unsigned int ecx;
	unsigned int edx;
	unsigned int ebx;
	unsigned int esp;
	unsigned int ebp;
	unsigned int esi;
	unsigned int edi;
	unsigned int es;
	unsigned int cs;
	unsigned int ss;
	unsigned int ds;
	unsigned int fs;
	unsigned int gs;
	unsigned int ldt;
	unsigned int trap;
	unsigned int iobase;

};

/* task_struct：进程的描述符 */
struct task_struct{
	int state;
	long counter;
	long priority;
	long signal;


	long blocked;
	int exit_code;
	unsigned long start_code,end_code,end_data,brk,start_stack;
	long pid,father,pgrp,session,leader;
	unsigned short uid,euid,suid;
	unsigned short gid,egid,sgid;leader;

	long alarm;
	long utime,stime,cutime,cstime,start_time;

	int tty;

	/* 接下来需要三个结构体代码该进程执行的对象 */
	// 目前先不设置

	/* 该进程的局部段描述符 */
	struct ldt_struct_t ldt[3];
	/* 该进程的tss */
	struct tss_struct tss;

}




#endif



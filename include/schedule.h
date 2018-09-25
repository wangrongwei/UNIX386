
#ifndef __SCHEDULE_H__
#define __SCHEDULE_H__

/*
 * 定义与调度相关的变量
 *
 */


#define FIRST_TSS_INGDT 4
#define FIRST_LDT_INGDT 5

// 将tss或者ldt相对于第一个tss或者第一个ldt进行编码
// 可以不直接编码，需要后续完善
#define _TSS(n)  (n)
#define _LDT(n) (n)

/* 在kernel.asm文件中，与_page_dir是相同的含义 */
#define pg_dir 0

#endif




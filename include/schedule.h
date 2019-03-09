
#ifndef __SCHEDULE_H__
#define __SCHEDULE_H__

/*
 * 定义与调度相关的变量
 *
 */

static void schedule_init(void);
static void reschedule(void);

/* 在kernel.asm文件中，与_page_dir是相同的含义 */
#define pg_dir (0)

#endif




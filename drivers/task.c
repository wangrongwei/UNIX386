
/*
 * 初始化task_tables，支持多任务间切换
 */

#include "task_struct.h"
#include "task.h"

//struct task_struct task_tables[NR_TASKS] = {&(init_task.task),};

/*
 * 创建内核线程接口
 */
void kernel_thread_create(int (*fn)(void *),void *args)
{
	/* TODO 构建进程 */
	return;
}


/* 与tty相关的进程 */
void task_tty()
{
	/* 初始化tty */

	/* 读写tty */
	while(1);
}



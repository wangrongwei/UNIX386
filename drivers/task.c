
/*
 * 初始化task_tables，支持多任务间切换
 */

#include <task_struct.h>

//struct task_struct task_tables[NR_TASKS] = {&(init_task.task),};


/* 与tty相关的进程 */
void task_tty()
{
	/* 初始化tty */

	/* 读写tty */
	while(1);
}




/*
 * 系统调用表头文件
 *	简单实现sys_setup() sys_exit() sys_open() sys_read()等
 *		完整的内核支持的系统调用较多，本内核简单设置
 */


#define __NR_setup	0
#define __NR_exit	1
#define __NR_fork	2
#define __NR_read	3
#define __NR_write	4
#define __NR_open	5
#define __NR_close	6


/* 定义函数指针，代表各系统调用函数 */
typedef int (*syscall_ptr)();

extern int sys_setup();
extern int sys_exit();
extern int sys_fork();
extern int sys_read();
extern int sys_write();
extern int sys_open();
extern int sys_close();

/*
 * 先使用这7个系统调用，system_call_table会被kernel.asm中调用
 */
syscall_ptr system_call_table[]={sys_setup,sys_exit,sys_fork,
sys_read,sys_write,sys_open,sys_close};





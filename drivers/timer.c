/*
 * 说明：定义时钟驱动相关函数
 *
 */
#include <timer.h>
#include <debug.h>
#include <interrupt.h>


/*
 * 时钟测试函数
 */
void timer_callback(pt_regs *regs)
{
	static unsigned int tick = 0;
	tick++;
}

/*
 * 正式的时钟中断
 *		供进程实现
 */
void timer_interrupt(pt_regs *regs)
{
	long tick = 0;
	tick++;
}



/*
 * 时钟初始化驱动
 * 	frequency:时钟频率，越大，中断产生越频繁
 */
void init_timer(unsigned int frequency)
{
	printk("initial timer,set timer interrupt......\n");
	unsigned char low=0,high=0;
	unsigned int div=0;
	register_interrupt_handler(IRQ0,timer_interrupt);

	// 设置时钟模式：模式3
	outb(0x43,0x36);

	// 将frequency拆分成两部分写到0x40端口
	div = 1193180 / frequency;
	low = (unsigned char)div & 0xff;
	high = (unsigned char)(div >> 8) & 0xff;
	outb(0x40,low);
	outb(0x40,high);

	// 开启时钟中断
	outb();
	
}





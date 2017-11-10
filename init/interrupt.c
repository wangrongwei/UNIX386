/*
 * 说明：将中断的相关部分分开
 *
 */
#include <interrupt.h>

/*
 * 中断服务程序
 * 函数说明：识别到底是发生了说明中断（通过regs->int_no），然后
 *	     执行相应的中断服务程序。
 * interrupt_handlers[n]是一个数组函数，n代表了具体的中断服务程
 * 序，函数参数为regs。
 */
void isr_handler(pt_regs *regs)
{
	if(interrupt_handlers[regs->int_no]){
		interrupt_handlers[regs->int_no](regs);
	}
	else{
		printk("unhandle interrupt_handler:%d\n",regs->int_no);
	}
}

void irq_handler(pt_regs *regs)
{
	// 从32号开始，为用户自定义中断
	// 单片只能处理八级中断
	// 因此大于40是由从片处理
	if(regs->int_no > 40){
		// 发送重设信号给从片
		outb(0xa0,0x20);
	}
	// 发送重设信号给主片
	outb(0x20,0x20);
	if(interrupt_handlers[regs->int_no]){
		interrupt_handlers[regs->int_no](regs);
	}
}
/*
 * 函数说明：向中断数组函数加入具体的中断服务程序（相当于注册）
 */
void register_interrupt_handler(unsigned char n,interrupt_handler_t h)
{
	interrupt_handlers[n] = h;
	//h->int_no = n;
}




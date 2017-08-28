/*
 * 说明：将中断的相关部分分开
 *
 */
#include <interrupt.h>


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

void register_interrupt_handler(unsigned char n,interrupt_handler_t h)
{
	interrupt_handlers[n] = h;
	//h->int_no = n;
}




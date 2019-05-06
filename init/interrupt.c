/*
 * 说明：将中断的相关部分分开
 *
 */
#include <interrupt.h>
#include <debug.h>
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
#ifdef _DEBUG
		printk("ds:%d\n",regs->ds);		// 用于保存用户的数据段描述符
		printk("edi:%d\n",regs->edi);		// 从 edi 到 eax 由 pusha 指令压入
		printk("esi:%d\n",regs->esi);
		printk("ebp:%d\n",regs->ebp);
		printk("esp:%d\n",regs->esp);
		printk("ebx:%d\n",regs->ebx);
		printk("edx:%d\n",regs->edx);
		printk("ecx:%d\n",regs->ecx);
		printk("eax:%d\n",regs->eax);
		printk("int_no:%d\n",regs->int_no);	// 中断号
		printk("err_code:%d\n",regs->err_code);	// 错误代码(有中断错误代码的中断会由CPU压入)
		printk("eip:%d\n",regs->eip);		// 以下由处理器自动压入
		printk("cs:%d\n",regs->cs);
		printk("eflags:%d\n",regs->eflags);
		printk("useresp:%d\n",regs->useresp);
		printk("ss:%d\n",regs->ss);
#endif
	}
}

void irq_handler(pt_regs *regs)
{
	/* 
	 * 从32号开始，为用户自定义中断，单片只能处理八级中断，因此大于40是由从片处理 
	 */
	if(regs->int_no > 40){
		/* 发送重设信号给从片 */
		outb(0xa0,0x20);
	}
	/* 发送重设信号给主片 */
	outb(0x20,0x20);
	if(interrupt_handlers[regs->int_no]){
		interrupt_handlers[regs->int_no](regs);
	}
	else{
		printk("unhandle irq_handler:%d\n",regs->int_no);
#ifdef _DEBUG
		printk("ds:%d\n",regs->ds);		// 用于保存用户的数据段描述符
		printk("edi:%d\n",regs->edi);		// 从 edi 到 eax 由 pusha 指令压入
		printk("esi:%d\n",regs->esi);
		printk("ebp:%d\n",regs->ebp);
		printk("esp:%d\n",regs->esp);
		printk("ebx:%d\n",regs->ebx);
		printk("edx:%d\n",regs->edx);
		printk("ecx:%d\n",regs->ecx);
		printk("eax:%d\n",regs->eax);
		printk("int_no:%d\n",regs->int_no);	// 中断号
		printk("err_code:%d\n",regs->err_code);	// 错误代码(有中断错误代码的中断会由CPU压入)
		printk("eip:%d\n",regs->eip);		// 以下由处理器自动压入
		printk("cs:%d\n",regs->cs);
		printk("eflags:%d\n",regs->eflags);
		printk("useresp:%d\n",regs->useresp);
		printk("ss:%d\n",regs->ss);
#endif
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




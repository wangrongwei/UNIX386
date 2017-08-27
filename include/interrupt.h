#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

/*
 * 说明：在初始化gdt表以后,需要开始初始化idt，该头文件定义相关中断处理函数
 * 这些中断处理函数只是填充idt表中的offset项
 *
 */


// 寄存器类型
typedef struct pt_regs_t {
	unsigned int ds;		// 用于保存用户的数据段描述符
	unsigned int edi;		// 从 edi 到 eax 由 pusha 指令压入
	unsigned int esi;
	unsigned int ebp;
	unsigned int esp;
	unsigned int ebx;
	unsigned int edx;
	unsigned int ecx;
	unsigned int eax;
	unsigned int int_no;	// 中断号
	unsigned int err_code;	// 错误代码(有中断错误代码的中断会由CPU压入)
	unsigned int eip;		// 以下由处理器自动压入
	unsigned int cs;
	unsigned int eflags;
	unsigned int useresp;
	unsigned int ss;
} pt_regs;

// 定义中断处理函数指针
typedef void (*interrupt_handler_t)(pt_regs *);

// 注册一个中断处理函数
void register_interrupt_handler(unsigned char n, interrupt_handler_t h);

// 调用中断处理函数
void isr_handler(pt_regs *regs);

interrupt_handler_t interrupt_handlers[256];



// 声明中断处理函数 0-19 属于 CPU 的异常中断
// ISR:中断服务程序(interrupt service routine)
void isr0(); 		// 0 #DE 除 0 异常 
void isr1(); 		// 1 #DB 调试异常 
void isr2(); 		// 2 NMI 
void isr3(); 		// 3 BP 断点异常 
void isr4(); 		// 4 #OF 溢出 
void isr5(); 		// 5 #BR 对数组的引用超出边界 
void isr6(); 		// 6 #UD 无效或未定义的操作码 
void isr7(); 		// 7 #NM 设备不可用(无数学协处理器) 
void isr8(); 		// 8 #DF 双重故障(有错误代码) 
void isr9(); 		// 9 协处理器跨段操作 
void isr10(); 		// 10 #TS 无效TSS(有错误代码) 
void isr11(); 		// 11 #NP 段不存在(有错误代码) 
void isr12(); 		// 12 #SS 栈错误(有错误代码) 
void isr13(); 		// 13 #GP 常规保护(有错误代码) 
void isr14(); 		// 14 #PF 页故障(有错误代码) 
void isr15(); 		// 15 CPU 保留 
void isr16(); 		// 16 #MF 浮点处理单元错误 
void isr17(); 		// 17 #AC 对齐检查 
void isr18(); 		// 18 #MC 机器检查 
void isr19(); 		// 19 #XM SIMD(单指令多数据)浮点异常

// 20-31 Intel 保留
void isr20();
void isr21();
void isr22();
void isr23();
void isr24();
void isr25();
void isr26();
void isr27();
void isr28();
void isr29();
void isr30();
void isr31();

// 32～255 用户自定义异常
void isr255();


void isr_handler(pt_regs *regs)
{
	if(interrupt_handlers[regs->int_no]){
		interrupt_handlers[regs->int_no](regs);
	}
	else{
		printk("unhandle interrupt_handler:%d\n",regs->int_no);
	}
}


#endif


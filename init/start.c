
/*
 * 从汇编跳转到C语言的第一个.c文件
 *	author: Wang<wangrongwei.kernel@gmail.com>
 *	time: 2017/05/13
 */


#include <console.h>
#include <string.h>
#include <descriptor.h>
#include <interrupt.h>
#include <timer.h>
#include <pmm.h>
#include <font.h>

#include <schedule.h>

#include <i386/sys.h>

#include <unistd.h>
#include <task_struct.h>
#include <schedule.h>
#include <buddy.h>
#include <init.h>

extern unsigned char kernel_s[];
extern unsigned char kernel_e[];
extern syscall_ptr system_call_table[];

void outb(unsigned short port,unsigned short value);

unsigned char inb(unsigned short port);

unsigned short inw(unsigned short port);


void logo(void);

/*
 *	kernel.s------>kernel_start()
 *	汇编代码跳入到C语言的第一个函数，需对如下使用C语言重新初始化
 *	
 *	1.初始化全局描述符和中断描述符
 *	2.初始化调色板
 *	3.初始化键盘
 *	4.初始化时钟中断
 * 
 */
void kernel_start()
{
#if 0
	kernel_stack_top = (long)kernel_stack + STACK_SIZE;
	__asm__ __volatile__("mov %0,%%esp\n\t"::"r"(kernel_stack_top));
	__asm__ __volatile__("xor %ebp, %ebp");
#endif
	int i;
	//字符串存在ELF文件的.stab节和.stabstr节（这部分特别大）
	//unsigned char *string = "Hello,welcome to DeeppinkOS\n";
	unsigned int page_addr1 = 0,page_addr2 = 0;
	printk("enter the kernel_start function...\n");
	/* 打印内核版本 */
	printk("UNIX386 v0.0.1\n");
	//unsigned char *input = (unsigned char *)0xb8000;
	//unsigned char color = (0 << 4) | (15 & white);
	//unsigned char *string = "Hello,welcome to DeeppinkOS\n";
	//unsigned int page_addr1=0,page_addr2=0;
	//*input++ = 'H';
	//*input++ = color

	//console_clear();

	//console_puts(string,0,green);
	//printk(string);
	init_gdt();
	/* 初始化中断/异常/系统调用，填充中断描述符 */
	init_idt();
	init_paging();
	/* 初始化调色板 */
	init_palette();
	/* asm volatile("int $0x3"); */
	/* asm volatile("int $0x4"); */

	init_keyboard();
	__asm__ __volatile__("cli"); /* 关闭中断 */
	schedule_init();
	printk("kernel start addr = 0x%08X\n", kernel_s);
	printk("kernel end   addr = 0x%08X\n", kernel_e);
	printk("kernel size = %dKB\n", (kernel_e-kernel_s + 1023) / 1024);

	init_pmm();
	printk("The size of key structure:\n");
	printk("task_struct: %d\n",sizeof(struct task_struct));
	printk("tss_struct: %d\n",sizeof(struct tss_struct));
	printk("buddy_element: %d\n",sizeof(struct buddy_element));
	printk("phy_page_count: %d\n",phy_page_count);
#if 1
	//page_addr1 = pmm_alloc_page();
	//printk("alloc page1 = 0x%08X\n",page_addr1);
	//page_addr2 = pmm_alloc_page();
	//logo();
	/* 其他设备初始化 */
	init_thread();
	init_buddy();
	init();
#endif
	__asm__ __volatile__("sti");
	/* 以下代码类似CPU进入idle */
	while(1){
		//schedule();
		i++;
		if(i == 5000){
			printk("s");
			i = 0;
		}
		__asm__ __volatile__("sti");
	}

}

/*
 * 从端口输出一个字节
 */
inline void outb(unsigned short port,unsigned short value)
{
	__asm__ __volatile__("outb %1,%0"::"dN"(port),"a"(value));
}

/*
 * 从端口读一个字节
 */
inline unsigned char inb(unsigned short port)
{
	unsigned char retval=0;
	__asm__ __volatile__("inb %1,%0":"=a"(retval):"dN"(port));
	return retval;
}

/*
 * 从端口读一个字
 */
inline unsigned short inw(unsigned short port)
{
	unsigned short retval=0;
	/* retval为输出，port为输入 */
	__asm__ __volatile__("inw %1,%0":"=a"(retval):"dN"(port));
	return retval;
}

#if 0
/*
 * 向显存写数据
 *
void write_vram(int address,int data)
{
	asm volatile("MOV [%%ESP+4],%%ECX\n\t"
		     "MOV [%%ESP+8],%%AL\n\t"
		     "MOV %%AL,[%%ECX]\n\t"
		     "RET"
		     :
		     :"c"
		     :"ECX"
		     );

}
*********************************************/
#endif

/*
 * 部分初始化完成以后，开始清屏，再显示
 * DeeppinkOS内核的logo
 */
void logo(void)
{
	int i,j,x,y;
	char pixel;
#if 0
	char font_A[16]={
		0x00,0x18,0x18,0x18,0x18,0x24,0x24,0x24,
		0x24,0x7e,0x42,0x42,0x42,0xe7,0x00,0x00
	};
#endif
	x = y = 0;
	//pixel = font_A[0];
	for(i=0;i<=15;i++){
		pixel = font_A[i];
		for(j=0;j<=7;j++){
			/* 此处对最高位做判断，分别显示不同颜色 */
			if(pixel & 0x80)
				write_vram(0xa0000+j+320*i,1);
			else
				write_vram(0xa0000+j+320*i,0);
			/* pixel向左移动一位 */
			pixel <<= 1;
		}
	}

}




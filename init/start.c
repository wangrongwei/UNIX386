/*
 * 从汇编跳转到C语言的第一个.c文件
 *	author: Alexander.Wang<wangrongwei.kernel@gmail.com>
 *	time: 2017/05/13
 */
#include <console.h>
#include <string.h>
#include <descriptor.h>
#include <interrupt.h>
#include <timer.h>
#include <pmm.h>

extern unsigned char kernel_s[];
extern unsigned char kernel_e[];
void outb(unsigned short port,unsigned short value);

unsigned char inb(unsigned short port);

unsigned short inw(unsigned short port);
/*
 *	kernel.s------>kernel_start()
 */
void kernel_start()
{
	//unsigned char *input = (unsigned char *)0xb8000;
	//unsigned char color = (0 << 4) | (15 & white);
	unsigned char *string = "Hello,welcome to DeeppinkOS\n";
	unsigned int page_addr1=0,page_addr2=0;
	//*input++ = 'H';
	//*input++ = color;

	console_clear();
	//console_puts(string,0,green);
	printk(string);
	//memcpy((void *)0x100000,kernel_main,512);

	/* 跳转到0x100000执行 */
	//asm volatile("MOV EAX,0x100000 \
	//	      JMP EAX");

	init_gdt();
	init_idt();

	//asm volatile("int $0x3");

	//asm volatile("int $0x4");

	//init_timer(200);

	/* 这段代码有bug */
	init_keyboard();
	asm volatile("sti"); // 打开中断
	//while(1){
	//	keyboard_read();
	//}
#if 1
	printk("kernel start addr = 0x%08X\n",kernel_s);
	printk("kernel end   addr = 0x%08X\n",kernel_e);
	printk("kernel size = %dKB\n",(kernel_e-kernel_s + 1023) / 1024);

	init_pmm();
	page_addr1 = pmm_alloc_page();
	printk("alloc page1 = 0x%08X\n",page_addr1);
	page_addr2 = pmm_alloc_page();
	printk("alloc page2 = 0x%08X\n",page_addr2);
	pmm_free_page(page_addr2);
	pmm_free_page(page_addr1);

	while(1){
		keyboard_read();
	}
	//return 0;
#endif

}
/*
 * 从端口输出一个字节
 */
inline void outb(unsigned short port,unsigned short value)
{
	asm volatile("outb %1,%0"::"dN"(port),"a"(value));

}
/*
 * 从端口读一个字节
 */
inline unsigned char inb(unsigned short port)
{
	unsigned char retval=0;
	asm volatile("inb %1,%0":"=a"(retval):"dN"(port));
	return retval;
}
/*
 * 从端口读一个字
 */
inline unsigned short inw(unsigned short port)
{
	unsigned short retval=0;
	asm volatile("inw %1,%0":"=a"(retval):"dN"(port));
	return retval;
}






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

	//*input++ = 'H';
	//*input++ = color;

	console_clear();
	//console_puts(string,0,green);
	printk(string);
	init_gdt();
	init_idt();

	//asm volatile("int $0x3");
	//asm volatile("int $0x4");

	init_timer(200);
	while(1);
	//return 0;
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






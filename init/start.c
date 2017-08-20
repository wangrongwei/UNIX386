#include<console.h>


void outb(unsigned short port,unsigned short value);

unsigned char inb(unsigned short port);

unsigned short inw(unsigned short port);
/*
 *	kernel.s------>kernel_start()
 */
void kernel_start()
{
	unsigned char *input = (unsigned char *)0xb8000;
	unsigned char color = (0 << 4) | (15 & red);


	*input++ = 'H';
	*input++ = color;

	*input++ = 'e';
	*input++ = color;

	*input++ = 'l';
	*input++ = color;

	*input++ = 'l';
	*input++ = color;

	*input++ = 'o';
	*input++ = color;

	while(1);
	//return 0;
}

inline void outb(unsigned short port,unsigned short value)
{
	asm volatile("outb %1,%0"::"dN"(port),"a"(value));

}
inline unsigned char inb(unsigned short port)
{
	unsigned char retval=0;
	asm volatile("inb %1,%0":"=a"(retval):"dN"(port));
	return retval;
}

inline unsigned short inw(unsigned short port)
{
	unsigned short retval=0;
	asm volatile("inw %1,%0":"=a"(retval):"dN"(port));
	return retval;
}




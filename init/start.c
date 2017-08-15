//#include<start.h>


/*
 *	kernel.s------>kernel_start()
 */
int kernel_start()
{
	unsigned char *input = (unsigned char *)0xb8000;
	unsigned char color = (0 << 4) | (15 & 0x0f);

	*input++ = 'H'; *input++ = color;

	*input++ = 'e'; *input++ = color;

	*input++ = 'l'; *input++ = color;

	return 0;

}


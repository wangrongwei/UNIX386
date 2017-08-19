//#include<start.h>


/*
 *	kernel.s------>kernel_start()
 */
void kernel_start()
{
	unsigned char *input = (unsigned char *)0xb8000;
	unsigned char color = (0 << 4) | (15 & 0x0f);

	*input++ = 'H';
	*input++ = color;

	*input++ = 'E';
	*input++ = color;
	//*input++ = 'e';
	//*input++ = color;

	//*input++ = 'l';
	//*input++ = color;
	//int i;
	//char *p;

	//p = (char *) 0xa0000;

	//for (i = 0; i <= 0xffff; i++) {
	//	*(p + i) = i & 0x0f;
	//}
	while(1);


	//return 0;

}


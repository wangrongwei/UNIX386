/*
 * 跟显示相关的函数
 */
#include <console.h>


static unsigned short *video_memory=(unsigned short*)0xb8000;


/*
 * 清屏函数
 * 给每一行写空格即可
 */
void console_clear()
{
	int i;
	// 黑底白字的blank看上去就像清屏了
	unsigned char attribute = (0 << 4) | (15 & white);
	unsigned short blank = (0x20) | (attribute<<8);
	for(i=0;i<80*25;i++){
		video_memory[i] = blank;
	}

}

/*
 * 显示一串字符
 * string 表示要显示的字符串
 * color_b 表示显示字符串的背景颜色
 * color_c 表示显示的字符的颜色
 */
void console_puts(unsigned char *string,unsigned char color_b,unsigned char color_c)
{
	int i=0;
	unsigned char attribute = (color_b << 4)|(15 & color_c);
	//unsigned short character = attribute<<8;
	while(string[i] != '\0'){
		video_memory[i] = string[i] | (attribute << 8);
		i++;
	}

}



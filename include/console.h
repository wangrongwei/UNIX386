#ifndef __CONSOLE_H__
#define __CONSOLE_H__

enum color{
	black = 0,
	blue  = 1,
	green = 2,
	blue_green = 3,
	red   = 4,
	magenta = 5,
	brown = 6,
	white = 7
};

/*
 * 定义两个与屏幕有关的函数-清屏和显示一个字符串
 */
void console_clear();
void console_puts(unsigned char *string,unsigned char color_b,unsigned char color_z);
void console_movecursor(int x,int y);
unsigned short console_readcursor();


#endif




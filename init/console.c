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
	console_movecursor(0,0);

}

/*
 * 显示一串字符
 * string 表示要显示的字符串
 * color_b 表示显示字符串的背景颜色
 * color_c 表示显示的字符的颜色
 */
void console_puts(unsigned char *string,unsigned char color_b,unsigned char color_c)
{
	int i=0,cursor_x=0,cursor_y=0;
	unsigned char attribute = (color_b << 4)|(15 & color_c);
	unsigned short cursor_xy=0;

	cursor_xy = console_readcursor();
	//console_movecursor(cursor_xy%80,cursor_xy/80);
	cursor_x = cursor_xy % 80;
	cursor_y = cursor_xy / 80;
	while(string[i] != '\0'){
		//检查是否有换行符
		do
		{
			if(string[i] == '\n'){
				cursor_y++;   // 纵坐标+1
				cursor_x = 0; // 光标移到最开始
				i++;	      // 不显示换行符
			}
			//if(string[i] == '\0')
			//	break;// 此处的break只能跳出do{}while

		}while(string[i] == '\n');

		// 字符串最后的\0不显示
		if(string[i] != '\0')
		{
			video_memory[cursor_y*80+cursor_x] = string[i] | (attribute << 8);
			i++;
			cursor_x++;

			// 到一行显示完，开始从下一行开始显示
			if(cursor_x >= 79)
				cursor_y++;
		}
		console_movecursor(cursor_x,cursor_y);


	}//结束while(string[i] != '\0')
}
/*
 * 说明：移到光标到坐标 (x,y)
 */
void console_movecursor(int x,int y)
{
	//组合要移动的光标位置
	unsigned short cursor_addr = y*80+x;
	//指明下面是设置光标的行
	outb(0x3d4,14);
	outb(0x3d5,cursor_addr>>8);
	//指明下面是设置光标的列
	outb(0x3d4,15);
	outb(0x3d5,cursor_addr);

}
/*
 * 说明：读取当前光标的位置
 */
unsigned short console_readcursor()
{
	unsigned short cursor_xy;
	unsigned char cursor_low=0,cursor_high=0;
	// 读取光标高八位
	outb(0x3d4,14);
	cursor_high = inb(0x3d5);
	// 读取光标低八位
	outb(0x3d4,15);
	cursor_low = inb(0x3d5);

	cursor_xy = (unsigned short)cursor_high;
	cursor_xy = ((cursor_xy << 8) & 0xff00) | cursor_low;

	// cursor_xy取80的余数就是x值坐标了
	return cursor_xy;

}




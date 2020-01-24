
/*
 * 实现tty
 */

#include "tty.h"
#include "console.h"

TTY tty_table[NR_CONSOLES];
CONSOLE console_table[NR_CONSOLES];


void tty_write(TTY *tty, char *buf, int len)
{
	char *p=buf;
	int i=len;

	while(1){
		//out_char(tty->p_console, *p++);
		/* 暂时用console_puts表示 */
		console_puts(buf,0,red);
		i--;
	}

}




#ifndef _TTY_H
#define _TTY_H

#define NR_CONSOLES 3
#define TTY_IN_BYTES 256

typedef struct tty_struct
{
	unsigned long in_buf[TTY_IN_BYTES];
	unsigned long *inbuf_head;
	unsigned long *inbuf_tail;
	int inbuf_count;

	struct console_struct* p_console;
}TTY;

typedef struct console_struct
{
	unsigned int current_start_addr;
	unsigned int original_addr;
	unsigned int mem_limit;
	unsigned int cursor;
}CONSOLE;

void tty_write(TTY *tty, char *buf, int len);

#endif





/*
 * 初始化键盘相关函数
 */

#include <keyboard.h>
#include <debug.h>
#include <interrupt.h>
#include <keymap.h>
/*
 * 说明：注册键盘中断处理函数
 */
static KB_INPUT kb_in;

void init_keyboard()
{
	printk("keyboard initial,and set keyboard_handler......\n");
	kb_in.count = 0;
	kb_in.p_head = kb_in.p_tail = kb_in.buf;
	register_interrupt_handler(IRQ1,keyboard_handler);

}

/*
 * 说明：键盘中断处理函数
 */
void keyboard_handler(pt_regs *regs)
{
	unsigned char scancode;
	//增加一行注释
	scancode = inb(0x60);
	if(kb_in.count < KB_IN_BYTES){
		*(kb_in.p_head) = scancode;
		kb_in.p_head++;
		// 如果满了，又指向开始
		if(kb_in.p_head == kb_in.buf+KB_IN_BYTES){
			kb_in.p_head = kb_in.buf;
		}

	}
	kb_in.count++;
	//printk("0x%02X,",scancode);

}

/*
 * 读取键盘缓存区kb_in，并打印显示
 */
void keyboard_read()
{
	unsigned char scancode;
	asm volatile("cli");
	if(kb_in.count > 0){
		scancode = *(kb_in.p_tail);
		kb_in.p_tail++;
		kb_in.p_tail++;
		// 如果读到了最后
		if(kb_in.p_tail == kb_in.buf + KB_IN_BYTES){
			kb_in.p_tail = kb_in.buf;
		}
		kb_in.count = kb_in.count - 2;
		//keycode[0] = scancode;
		printk("%c",keymap[scancode*3]);
	}
	asm volatile("sti");
}





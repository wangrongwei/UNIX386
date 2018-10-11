#ifndef __DESCRIPTOR_H__
#define __DESCRIPTOR_H__

/*
 * 文件描述：全局描述符和中断描述符相关结构体定义
 *
 */
#include "debug.h"
#include "console.h"
#include "vargs.h"
#include "interrupt.h"
#include "string.h"

#define GDT_LEN 5
#define IDT_LEN 256

extern load_gdtr(unsigned int *);
extern load_idtr(unsigned int *);

static void init_gdt();
static void init_idt();

// 填充gdt表
static void set_gdt(int num,unsigned int base,unsigned int limit,\
	     unsigned char access,unsigned char G_DB_L_AVL);

// 填充idt表
static void set_idt(int num,unsigned int base,unsigned short sel,\
		    unsigned short flags);

/* 全局段描述符 */
typedef struct gdt_struct_t{
	unsigned short limit0;	     //长度限制15--0 占两个字节
	unsigned short base0;	     //基地址15--0
	unsigned char  base1;	     //基地址23--16
	unsigned char  access;       //P_DVL(2位)_S_Type
	unsigned char  limit1:4;     //长度限制19--16
	unsigned char  G_DB_L_AVL:4; //
	unsigned char  base2;        //基地址31--24
}__attribute__((packed)) gdt_struct_t;

struct gdtr_t{
	unsigned short length; //这个大小代表了gdt表的大小
	unsigned int   base;   //gdt表的基地址
}__attribute__((packed)) gdtr_t;

/* 局部段描述符 */
typedef struct ldt_struct_t{
	unsigned short limit0;	     //长度限制15--0 占两个字节
	unsigned short base0;	     //基地址15--0
	unsigned char  base1;	     //基地址23--16
	unsigned char  access;       //P_DVL(2位)_S_Type
	unsigned char  limit1:4;     //长度限制19--16
	unsigned char  G_DB_L_AVL:4; //
	unsigned char  base2;         //基地址31--24
}__attribute__((packed)) ldt_struct_t;


typedef struct idt_struct_t{
	unsigned short base0;   //中断函数基地址15--0
	unsigned short sel;     //选择段描述符
	unsigned char  zero;    //全是0
	unsigned char  flags;	//相关标志 P_DVL_'E'
	unsigned short base1;	//中断函数基地址31--16

}__attribute__((packed)) idt_struct_t;

struct idtr_t{
	unsigned short length; //这个大小代表了idt表的大小
	unsigned int   base   //gdt表的基地址
}__attribute__((packed)) idtr_t;

gdt_struct_t gdt_list[GDT_LEN];
struct gdtr_t GDTR;

idt_struct_t idt_list[IDT_LEN];
struct idtr_t IDTR;


/*
 *	填充gdt列表
 *	num: 在gdt的位置
 *	base: 填充的段的基地址
 *	limit: 该段的段限长
 *	access: 
 *	G_DB_L_AVL: 权限
 */
static void set_gdt(int num,unsigned int base,unsigned int limit,\
	     unsigned char access,unsigned char G_DB_L_AVL)
{
	gdt_list[num].limit0 = (limit & 0xffff);
	gdt_list[num].base0 = (base & 0xffff);
	gdt_list[num].base1 = (base >> 16) & 0xff;
	gdt_list[num].access = access;
	gdt_list[num].limit1 = (limit >> 16);
	gdt_list[num].G_DB_L_AVL = G_DB_L_AVL;
	gdt_list[num].base2 = (base >> 24);

}

/*
 *	填充idt表
 *	num: 填充的中断项在idt中的位置
 *	base: 
 *	sel:
 *	flags:
 */
static void set_idt(int num,unsigned int base,unsigned short sel,\
		    unsigned short flags)
{
	idt_list[num].base0 = base & 0xffff;
	idt_list[num].base1 = (base >> 16) & 0xffff;
	idt_list[num].sel = sel;
	idt_list[num].zero = 0;
	idt_list[num].flags = flags;

}


// 中断
#define set_int_gate(num,base) set_idt(num,base,0x08,0x8e)
// 系统调用
#define set_system_gate(num,base) set_idt(num,base,0x08,0x8e)
// 陷阱
#define set_trap_gate(num,base) set_idt(num,base,0x08,0x8e)

/*
 * 设置5个全局描述符（包括全0）
 * 使用load_gdtr函数加载到gdtr寄存器
 */
static void init_gdt()
{
	printk("New,update gdt!!!\n");
	// sizeof是编译器内部的宏,不需要定义
	GDTR.length = sizeof(gdt_struct_t)*GDT_LEN - 1;
	GDTR.base = (unsigned int)&gdt_list;

	// 开始设置gdt表中的内容
	set_gdt(0,0,0,0,0);
	set_gdt(1,0,0xfffff,0x9a,0x0c); //代码段
	set_gdt(2,0,0xfffff,0x92,0x0c); //数据段
	set_gdt(3,0,0xfffff,0xfa,0x0c); //用户代码段
	set_gdt(4,0,0xfffff,0xf2,0x0c); //用户数据段

	// 加载gdt地址到gdtr寄存器
	load_gdtr((unsigned int)&GDTR);

}
/*
 * 初始化8259A，设置中断向量表
 */
static void init_idt()
{
	printk("New,load idt!!!\n");
	// 重新映射 IRQ 表
	// 两片级联的 Intel 8259A 芯片
	// 主片端口 0x20 0x21
	// 从片端口 0xA0 0xA1
	
	// 初始化主片、从片
	// 0001 0001
	outb(0x20, 0x11);
	outb(0xA0, 0x11);

	// 设置主片 IRQ 从 0x20(32) 号中断开始
	outb(0x21, 0x20);

	// 设置从片 IRQ 从 0x28(40) 号中断开始
	outb(0xA1, 0x28);
	
	// 设置主片 IR2 引脚连接从片
	outb(0x21, 0x04);

	// 告诉从片输出引脚和主片 IR2 号相连
	outb(0xA1, 0x02);
	
	// 设置主片和从片按照 8086 的方式工作
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	
	// 设置主从片允许中断
	outb(0x21, 0x0);
	outb(0xA1, 0x0);


	IDTR.length = sizeof(idt_struct_t)*256 - 1;
	IDTR.base = (unsigned int)&idt_list;

	bzero((unsigned char *)interrupt_handlers,sizeof(interrupt_handler_t)*256);
	bzero((unsigned char *)idt_list,sizeof(idt_struct_t)*256);

	//此处后续需要优化，将set_idt分成set_int_gate/set_system_gate/set_trap_gate实现
	set_idt(0,(unsigned int)isr0,0x08,0x8e);
	set_idt(1,(unsigned int)isr1,0x08,0x8e);
	set_idt(2,(unsigned int)isr2,0x08,0x8e);
	set_idt(3,(unsigned int)isr3,0x08,0x8e);
	set_idt(4,(unsigned int)isr4,0x08,0x8e);
	set_idt(5,(unsigned int)isr5,0x08,0x8e);

	set_idt(6,(unsigned int)isr6,0x08,0x8e);
	set_idt(7,(unsigned int)isr7,0x08,0x8e);
	set_idt(8,(unsigned int)isr8,0x08,0x8e);
	set_idt(9,(unsigned int)isr9,0x08,0x8e);
	set_idt(10,(unsigned int)isr10,0x08,0x8e);
	set_idt(11,(unsigned int)isr11,0x08,0x8e);
	set_idt(12,(unsigned int)isr12,0x08,0x8e);
	set_idt(13,(unsigned int)isr13,0x08,0x8e);
	set_idt(14,(unsigned int)isr14,0x08,0x8e);
	set_idt(15,(unsigned int)isr15,0x08,0x8e);
	set_idt(16,(unsigned int)isr16,0x08,0x8e);
	set_idt(17,(unsigned int)isr17,0x08,0x8e);
	set_idt(18,(unsigned int)isr18,0x08,0x8e);
	set_idt(19,(unsigned int)isr19,0x08,0x8e);
	set_idt(20,(unsigned int)isr20,0x08,0x8e);
	set_idt(21,(unsigned int)isr21,0x08,0x8e);
	set_idt(22,(unsigned int)isr22,0x08,0x8e);
	set_idt(23,(unsigned int)isr23,0x08,0x8e);
	set_idt(24,(unsigned int)isr24,0x08,0x8e);
	set_idt(25,(unsigned int)isr25,0x08,0x8e);
	set_idt(26,(unsigned int)isr26,0x08,0x8e);
	set_idt(27,(unsigned int)isr27,0x08,0x8e);
	set_idt(28,(unsigned int)isr28,0x08,0x8e);
	set_idt(29,(unsigned int)isr29,0x08,0x8e);
	set_idt(30,(unsigned int)isr30,0x08,0x8e);
	set_idt(31,(unsigned int)isr31,0x08,0x8e);

	set_idt(32,(unsigned int)irq0, 0x08,0x8e);
	set_idt(33,(unsigned int)irq1, 0x08,0x8e);
	set_idt(34,(unsigned int)irq2, 0x08,0x8e);
	set_idt(35,(unsigned int)irq3, 0x08,0x8e);
	set_idt(36,(unsigned int)irq4, 0x08,0x8e);
	set_idt(37,(unsigned int)irq5, 0x08,0x8e);
	set_idt(38,(unsigned int)irq6, 0x08,0x8e);
	set_idt(39,(unsigned int)irq7, 0x08,0x8e);
	set_idt(40,(unsigned int)irq8, 0x08,0x8e);
	set_idt(41,(unsigned int)irq9, 0x08,0x8e);
	set_idt(42,(unsigned int)irq10, 0x08,0x8e);
	set_idt(43,(unsigned int)irq11, 0x08,0x8e);
	set_idt(44,(unsigned int)irq12, 0x08,0x8e);
	set_idt(45,(unsigned int)irq13, 0x08,0x8e);
	set_idt(46,(unsigned int)irq14, 0x08,0x8e);
	set_idt(47,(unsigned int)irq15, 0x08,0x8e);

	// 用于实现系统调用
	set_idt(255,(unsigned int)isr255,0x08,0x8e);
	// 加载idt表地址
	load_idtr((unsigned int)&IDTR);

}



#endif






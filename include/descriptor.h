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

#define GDT_LEN 5
#define IDT_LEN 256

extern load_gdtr(unsigned int *);
extern load_idtr(unsigned int *);

void init_gdt();
void init_idt();

// 填充gdt表
static void set_gdt(int num,unsigned int base,unsigned int limit,\
	     unsigned char access,unsigned char G_DB_L_AVL);

// 填充idt表
static void set_idt(int num,unsigned int base,unsigned short sel,\
		    unsigned short flags);


typedef struct gdt_struct_t{
	unsigned short limit0;	     //长度限制15--0 占两个字节
	unsigned short base0;	     //基地址15--0
	unsigned char  base1;	     //基地址23--16
	unsigned char  access;       //P_DVL(2位)_S_Type
	unsigned char  limit1:4;     //长度限制19--16
	unsigned char  G_DB_L_AVL:4; //
	unsigned char  base2         //基地址31--24
}__attribute__((packed)) gdt_struct_t;

struct gdtr_t{
	unsigned short length; //这个大小代表了gdt表的大小
	unsigned int   base    //gdt表的基地址
}__attribute__((packed)) gdtr_t;

typedef struct idt_struct_t{
	unsigned short base0;   //中断函数基地址15--0
	unsigned short sel;     //选择段描述符
	unsigned char  zero;    //全是0
	unsigned char  flags;	//相关标志 P_DVL_'E'
	unsigned short base1	//中断函数基地址31--16

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
 * 填充gdt列表
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
 * 填充idt表
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

/*
 * 设置5个全局描述符（包括全0）
 * 使用load_gdtr函数加载到gdtr寄存器
 */
void init_gdt()
{
	printk("New,update gdt!!!\n");
	// sizeof是编译器内部的宏定义,不需要定义
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

void init_idt()
{
	printk("New,load idt!!!\n");
	IDTR.length = sizeof(idt_struct_t)*256 - 1;
	IDTR.base = (unsigned int)&idt_list;

	bzero((unsigned char *)interrupt_handlers,sizeof(interrupt_handler_t)*256);
	bzero((unsigned char *)idt_list,sizeof(idt_struct_t)*256);

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

	// 用于实现系统调用
	set_idt(255,(unsigned int)isr255,0x08,0x8e);
	// 加载idt表地址
	load_idtr((unsigned int)&IDTR);

}



#endif






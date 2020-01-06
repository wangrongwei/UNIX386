#ifndef __DESCRIPTOR_H__
#define __DESCRIPTOR_H__

/*
 * 文件描述：全局描述符和中断描述符相关结构体定义
 */
#include "debug.h"
#include "console.h"
#include "vargs.h"
#include "interrupt.h"
#include "string.h"


#define GDT_LEN 256
#define IDT_LEN 256

#define P_UNSET 0x00
#define P_SET 0x80
#define S_SS (0x00) /* 系统段 */
#define S_NSS (0x01 << 4) /* 非系统段 */

#define TI_UNSET (0x00 << 2)
#define TI_SET (0x01  << 2)

#define GDT_TI TI_UNSET
#define LDT_TI TI_SET

/* 全局描述符 */

#define DPL0 0x00
#define DPL1 0x01
#define DPL2 0x02
#define DPL3 0x03

/* RPL只在选择子（selector）中 */
#define RPL0 0x00
#define RPL1 0x01
#define RPL2 0x02
#define RPL3 0x03

#define TYPE_NOTBUSY 0x09
#define TYPE_BUSY 0x0b

#define AVL 0

/* 中断描述符 */



/*
 * P----DPL----S----TYPE
 * S=0：系统段
 * S=1：非系统段
 * 在系统段中，TYPE字段意义不同
 * 在非系统段中，TYPE字段分为代码段和数据段，四位分别为X-R-C-A
 */

/* 以下为TYPE的字段功能 */
/* S=0 */
#define TYPE_TSS_BUSY 0x0B
#define TYPE_TSS_NOTBUSY 0x09
#define TYPE_LDT 0x02

/* 三种门 */
#define TYPE_TASK_GATE 0x05
#define TYPE_CALL_GATE 0x0C 
#define TYPE_INT_GATE 0x0E 
#define TYPE_TRAP_GATE 0x0F 

/* S=1 */
#define TYPE_KERNEL_CS 0x0A
#define TYPE_KERNEL_DS 0x02

#define TYPE_USER_CS 0x0F
#define TYPE_USER_DS 0x07

 
/* 使用选择子的概念 */
#define _KERNEL_CS_SELECTOR ((0x01 << 3) | GDT_TI | RPL0)
#define _KERNEL_DS_SELECTOR ((0x02 << 3) | GDT_TI | RPL0)
#define _USER_CS_SELECTOR ((0x04 << 3) | GDT_TI | RPL3)
#define _USER_DS_SELECTOR ((0x05 << 3) | GDT_TI | RPL3)

extern load_gdtr(unsigned int *);
extern load_idtr(unsigned int *);

static void init_gdt();
static void init_idt();

// 填充gdt表
static void set_gdt(int num,unsigned int base,unsigned int limit,\
	     unsigned char access,unsigned char G_DB_L_AVL);

/* 设置tss与ldt */
static void set_tssldt2_gdt(int num,unsigned int base,char type);
// 填充idt表
static void set_idt(int num,unsigned int base,unsigned short sel,\
		    unsigned short flags);

/* 全局段描述符 */
typedef struct gdt_struct_t{
	unsigned short limit0;	     //长度限制15--0 占两个字节
	unsigned short base0;	     //基地址15--0
	unsigned char  base1;	     //基地址23--16
	unsigned char  access;       //P_DPL(2bits)_S_Type(4bits)，共8位
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
	unsigned char  access;       //P_DPL(2位)_S_Type
	unsigned char  limit1:4;     //长度限制19--16
	unsigned char  G_DB_L_AVL:4; //
	unsigned char  base2;         //基地址31--24
}__attribute__((packed)) ldt_struct_t;

/* 中断门 / 调用门 / 陷阱门 */
typedef struct idt_struct_t{
	unsigned short base0;   //中断函数基地址15--0
	unsigned short sel;     //选择段描述符
	unsigned char  zero;    //全是0
	unsigned char  flags;	//相关标志 P_DPL_TYPE
	unsigned short base1;	//中断函数基地址31--16
}__attribute__((packed)) idt_struct_t;

struct idtr_t{
	unsigned short length; //这个大小代表了idt表的大小
	unsigned int   base;   //gdt表的基地址
}__attribute__((packed)) idtr_t;

gdt_struct_t gdt_list[GDT_LEN];
struct gdtr_t GDTR;

idt_struct_t idt_list[IDT_LEN];
struct idtr_t IDTR;


/*
 * 填充gdt列表
 * num: 在gdt的位置
 * base: 填充的段的基地址
 * limit: 该段的段限长
 * access: 包括段的特权级别、段类型（代码段、数据段或者堆栈段）
 *		对于进程而言，ldt: access=0x82，tss: access=0x89
 * G_DB_L_AVL: 权限
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
 * 填充gdt列表
 * num: 在gdt的位置
 * base: 填充的段的基地址
 * type: 0x89为tss，0x82为ldt
 */
static void set_tssldt2_gdt(int num,unsigned int base,char type)
{
	gdt_list[num].limit0 = (104 & 0xffff);
	gdt_list[num].base0 = (base & 0xffff);
	gdt_list[num].base1 = (base >> 16) & 0xff;
	gdt_list[num].access = type;
	gdt_list[num].limit1 = 0;
	gdt_list[num].G_DB_L_AVL = 0x0c;
	gdt_list[num].base2 = (base >> 24);

}

/*
 * 填充idt表
 * num: 填充的中断项在idt中的位置
 * base: 
 * sel:
 * flags:
 */
static void set_idt(int num,unsigned int base,unsigned short sel,unsigned short flags)
{
	idt_list[num].base0 = base & 0xffff;
	idt_list[num].base1 = (base >> 16) & 0xffff;
	idt_list[num].sel = sel;
	idt_list[num].zero = 0x0;
	idt_list[num].flags = flags;

}


// 中断（权限为0）
#define set_int_gate(num,base) set_idt(num,base,TYPE_KERNEL_CS,P_SET & (~(DPL3 << 5)) | TYPE_INT_GATE)
// 陷阱（权限为0）
#define set_trap_gate(num,base) set_idt(num,base,TYPE_KERNEL_CS,P_SET & (~(DPL3 << 5)) | TYPE_TRAP_GATE)
// 特殊的陷阱（权限为3）--系统调用
#define set_system_gate(num,base) set_idt(num,base,TYPE_KERNEL_CS,P_SET | (DPL3 << 5) | TYPE_TRAP_GATE)

/*
 * 设置5个全局描述符（包括全0）
 * 使用load_gdtr函数加载到gdtr寄存器
 */
static void init_gdt()
{
	int i=0;
	printk("update gdt!\n");
	printk("CS: 0x%x\n",P_SET&(~(DPL3 << 5))|S_NSS|TYPE_KERNEL_CS);
	printk("DS: 0x%x\n",P_SET&(~(DPL3 << 5))|S_NSS|TYPE_KERNEL_DS);
	// sizeof是编译器内部的宏,不需要定义
	GDTR.length = sizeof(gdt_struct_t)*GDT_LEN - 1;
	GDTR.base = (unsigned int)&gdt_list;

	// 开始设置gdt表中的内容
	set_gdt(0,0,0,0,0);
	//set_gdt(1,0,0xfffff,0x9a,0x0c); //内核代码段
	//set_gdt(2,0,0xfffff,0x92,0x0c); //内核数据段
	set_gdt(1, 0, 0xfffff, P_SET&(~(DPL3 << 5))|S_NSS|TYPE_KERNEL_CS, 0x0c); //内核代码段
	set_gdt(2, 0, 0xfffff, P_SET&(~(DPL3 << 5))|S_NSS|TYPE_KERNEL_DS, 0x0c); //内核数据段
	
	set_gdt(3,0,0,0,0);//null
	//set_gdt(4,0,0xfffff,0xfa,0x0c); //用户代码段-------| 进程0的TSS0（任务状态段）
	//set_gdt(5,0,0xfffff,0xf2,0x0c); //用户数据段-------| 进程0的LDT0
	set_gdt(4, 0, 0xfffff, P_SET|(DPL3 << 5)|S_NSS|TYPE_USER_CS, 0x0c); //用户代码段-------| 进程0的TSS0（任务状态段）
	set_gdt(5, 0, 0xfffff, P_SET|(DPL3 << 5)|S_NSS|TYPE_USER_DS, 0x0c); //用户数据段-------| 进程0的LDT0
	/* 后续的段描符初始化为0（与进程相关的代码） */
	for(i=6;i<256;i++){
		set_gdt(i,0,0,0,0);
	}

	// 加载gdt地址到gdtr寄存器
	load_gdtr((unsigned int)&GDTR);
}


/*
 * 初始化8259A，设置中断向量表
 */
static void init_idt()
{
	printk("load idt!\n");
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
	set_trap_gate(0,(unsigned int)isr0);
	set_trap_gate(1,(unsigned int)isr1);
	set_trap_gate(2,(unsigned int)isr2);
	set_trap_gate(3,(unsigned int)isr3);
	set_trap_gate(4,(unsigned int)isr4);
	set_trap_gate(5,(unsigned int)isr5);

	set_trap_gate(6,(unsigned int)isr6);
	set_trap_gate(7,(unsigned int)isr7);
	set_trap_gate(8,(unsigned int)isr8);
	set_trap_gate(9,(unsigned int)isr9);
	set_trap_gate(10,(unsigned int)isr10);
	set_trap_gate(11,(unsigned int)isr11);
	set_trap_gate(12,(unsigned int)isr12);
	set_trap_gate(13,(unsigned int)isr13);
	set_trap_gate(14,(unsigned int)isr14);
	set_trap_gate(15,(unsigned int)isr15);
	set_trap_gate(16,(unsigned int)isr16);
	set_trap_gate(17,(unsigned int)isr17);
	set_trap_gate(18,(unsigned int)isr18);
	set_trap_gate(19,(unsigned int)isr19);
	/* Intel保留 */
	set_trap_gate(20,(unsigned int)isr20);
	set_trap_gate(21,(unsigned int)isr21);
	set_trap_gate(22,(unsigned int)isr22);
	set_trap_gate(23,(unsigned int)isr23);
	set_trap_gate(24,(unsigned int)isr24);
	set_trap_gate(25,(unsigned int)isr25);
	set_trap_gate(26,(unsigned int)isr26);
	set_trap_gate(27,(unsigned int)isr27);
	set_trap_gate(28,(unsigned int)isr28);
	set_trap_gate(29,(unsigned int)isr29);
	set_trap_gate(30,(unsigned int)isr30);
	set_trap_gate(31,(unsigned int)isr31);
	/* 用户定义 */
	set_trap_gate(32,(unsigned int)irq0);
	set_trap_gate(33,(unsigned int)irq1);
	set_trap_gate(34,(unsigned int)irq2);
	set_trap_gate(35,(unsigned int)irq3);
	set_trap_gate(36,(unsigned int)irq4);
	set_trap_gate(37,(unsigned int)irq5);
	set_trap_gate(38,(unsigned int)irq6);
	set_trap_gate(39,(unsigned int)irq7);
	set_trap_gate(40,(unsigned int)irq8);
	set_trap_gate(41,(unsigned int)irq9);
	set_trap_gate(42,(unsigned int)irq10);
	set_trap_gate(43,(unsigned int)irq11);
	set_trap_gate(44,(unsigned int)irq12);
	set_trap_gate(45,(unsigned int)irq13);
	set_trap_gate(46,(unsigned int)irq14);
	set_trap_gate(47,(unsigned int)irq15);

	/* 用于实现系统调用 */
	set_system_gate(255,(unsigned int)isr255);
	/* 加载idt表地址 */
	load_idtr((unsigned int)&IDTR);

}



#endif






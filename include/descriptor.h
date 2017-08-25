#ifndef __DESCRIPTOR_H__
#define __DESCRIPTOR_H__

/*
 * 文件描述：全局描述符和中断描述符相关结构体定义
 *
 */

#define GDT_LEN 5

extern load_gdtr(unsigned int *);
void init_gdt();
void init_idt();

// 填充gdt表
static void set_gdt(int num,unsigned short base,unsigned short limit,\
	     unsigned char access,unsigned char G_DB_L_AVL);

typedef struct gdt_struct_t{
	unsigned short limit0;	     //长度限制15--0 占两个字节
	unsigned short base0;	     //基地址15--0
	unsigned char  base1;	     //基地址23--16
	unsigned char  access;       //P_DVL(2位)_S_Type
	unsigned char  limit1:4;     //长度限制19--16
	unsigned char  G_DB_L_AVL:4  //
	unsigned char  base2;        //基地址31--24
}__attribute__((packed)) gdt_struct_t;

struct gdtr_t{
	unsigned short length; //这个大小代表了gdt表的大小
	unsigned int   base;   //gdt表的基地址
}__attribute__((packed)) gdtr_t;

typedef struct idt_struct_t{
	unsigned short base0;   //中断函数基地址15--0
	unsigned short sel;     //选择段描述符
	unsigned char  zero;    //全是0
	unsigned char  :5;	//空5位
	unsigned char  P_DPL:3;	//相关标志
	unsigned short base1;	//中断函数基地址31--16

}__attribute__((packed)) idt_struct_t;

struct idtr_t{
	unsigned short length; //这个大小代表了idt表的大小
	unsigned int   base;   //gdt表的基地址
}__attribute__((packed)) idtr_t;

gdt_struct_t gdt_list[GDT_LEN];
struct gdtr_t GDTR;

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
 * 设置5个全局描述符（包括全0）
 * 使用load_gdtr函数加载到gdtr寄存器
 */
void init_gdt()
{
	// sizeof是编译器内部的宏定义,不需要定义
	GDTR.limit = sizeof(gdt_struct_t)*GDT_LEN - 1;
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



#endif






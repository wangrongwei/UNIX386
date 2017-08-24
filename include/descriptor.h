#ifndef __DESCRIPTOR_H__
#define __DESCRIPTOR_H__

/*
 * 文件描述：全局描述符和中断描述符相关结构体定义
 *
 */
void init_gdt();
void init_idt();

typedef struct gdt_struct_t{
	unsigned short limit0;	     //长度限制15--0 占两个字节
	unsigned short base0;	     //基地址15--0
	unsigned char  base1;	     //基地址23--16
	unsigned char  access;       //P_DVL(2位)_S_Type
	unsigned char  limit1:4;     //长度限制19--16
	unsigned char  G_DB_L_AVL:4  //
	unsigned char  base2;        //基地址31--24
}__attribute__((packed)) gdt_struct_t;

struct GDTR{
	unsigned short length; //这个大小代表了gdt表的大小
	unsigned int   base;   //gdt表的基地址
}

typedef struct idt_struct_t{
	unsigned short base0;   //中断函数基地址15--0
	unsigned short sel;     //选择段描述符
	unsigned char  zero;    //全是0
	unsigned char  :5;	//空5位
	unsigned char  P_DPL:3;	//相关标志
	unsigned short base1;	//中断函数基地址31--16

}__attribute__((packed)) idt_struct_t;

struct IDTR{
	unsigned short length; //这个大小代表了idt表的大小
	unsigned int   base;   //gdt表的基地址
}



#endif






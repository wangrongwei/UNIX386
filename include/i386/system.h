
/*
 * 在编写内核过程中，需要在C语言中嵌套汇编语言，该头文件定义相关函数
 * 其中必须使用汇编语言实现的与系统相关的函数
 */


/* 
 * 添加进程的tss和ldt到gdt表后的基本函数
 * 采用intel嵌套汇编形式
 */
#define _set_tssldt_gdt(n,addr,type)\
__asm__("movw $104,%1\n\t"\
)



// 将进程的tss和ldt添加到系统全局gdt和ldt上
// 其中0x89代表tss，0x82代表ldt
#define set_tss_gdt(n,addr) _set_tssldt_gdt(((char *) (n)),((int)(addr)),"0x89")
#define set_ldt_gdt(n,addr) _set_tssldt_gdt(((char *) (n)),((int)(addr)),"0x82")












/*
 * 在编写内核过程中，需要在C语言中嵌套汇编语言，该头文件定义相关函数
 * 其中必须使用汇编语言实现的与系统相关的函数
 */


/* 
 * 添加进程的tss和ldt到gdt表后的基本函数
 * 采用intel嵌套汇编形式
 */
#define _set_tssldt_gdt(n,addr,type)\
__asm__ __volatile__("movb $104,%0\n\t" \
	"movw %%ax,%1\n\t" \
	"rorl $16,%%eax\n\t" \
	"movb %%al,%2\n\t" \
	"movb $"type",%3\n\t" \
	"movb $0x00,%4\n\t" \
	"movb %%ah,%5\n\t" \
	"rorl $16,%%eax" \
	:"=m" (*(n)), "=m" (*(n+2)), "=m" (*(n+4)), \
	 "=m" (*(n+5)), "=m" (*(n+6)), "=m" (*(n+7)):"+a" (addr), \
	)

// 将进程的tss和ldt添加到系统全局gdt和ldt上
// 其中0x89代表tss，0x82代表ldt
#define set_tss_gdt(n,addr) _set_tssldt_gdt(((char *) (n)),((int)(addr)),"0x89")
#define set_ldt_gdt(n,addr) _set_tssldt_gdt(((char *) (n)),((int)(addr)),"0x82")





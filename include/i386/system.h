
/*
 * 在编写内核过程中，需要在C语言中嵌套汇编语言，该头文件定义相关函数
 * 其中必须使用汇编语言实现的与系统相关的函数
 */


/* 
 * 添加进程的tss和ldt到gdt表后的基本函数
 * 采用intel嵌套汇编形式
 */

#define _set_tssldt_gdt2(n,addr,type)\
__asm__ __volatile__("movb $104,%0\n\t"\
	"movl %6,%%ecx\n\t"\
	"movw %%cx,%1\n\t" \
	"rorl $16,%%eax\n\t" \
	"movb %%al,%2\n\t" \
	"movb %7,%3\n\t" \
	"movb $0,%4\n\t" \
	"movb %%ah,%5\n\t" \
	"rorl $16,%%eax\n\t"\
	:"=m"(*(n)),"=m"(*(n+2)),"=m"(*(n+4)),"=m"(*(n+5)),"=m"(*(n+6)),"=m"(*(n+7))\
	:"g"(addr),"g"(type)\
	:"memory"\
)


// 在全局表中设置任务状态段/局部表描述符。
// 参数：n - 在全局表中描述符项n 所对应的地址；addr - 状态段/局部表所在内存的基地址。
// type - 描述符中的标志类型字节。
// %0 - eax(地址addr)；%1 - (描述符项n 的地址)；%2 - (描述符项n 的地址偏移2 处)；
// %3 - (描述符项n 的地址偏移4 处)；%4 - (描述符项n 的地址偏移5 处)；
// %5 - (描述符项n 的地址偏移6 处)；%6 - (描述符项n 的地址偏移7 处)；
#define _set_tssldt_gdt(n,addr,type) \
__asm__ ( "movw $104,%1\n\t" \
"movw %%ax,%2\n\t" \        
  "rorl $16,%%eax\n\t" \    
  "movb %%al,%3\n\t" \      
  "movb $" type ",%4\n\t" \ 
  "movb $0x00,%5\n\t" \   
  "movb %%ah,%6\n\t" \ 
  "rorl $16,%%eax" \ 
  ::"g" (addr), "m" (*(n)), "m" (*(n + 2)), "m" (*(n + 4)),\
  "m" (*(n + 5)), "m" (*(n + 6)), "m" (*(n + 7)))

		
// 将进程的tss和ldt添加到系统全局gdt和ldt上
// 其中0x89代表tss，0x82代表ldt
#define set_tss_gdt(n,addr) _set_tssldt_gdt(((char *) (n)),((int)(addr)),"0x89")
#define set_ldt_gdt(n,addr) _set_tssldt_gdt(((char *) (n)),((int)(addr)),"0x82")





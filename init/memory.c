
#include <unistd.h>
#include <pmm.h>
#include <init.h>

#define NR_DIR 32
uint32_t page_dir2[NR_DIR];
uint32_t page_table2[NR_DIR][1024];

/*
 * 重新初始化分页
 */
void init_paging()
{
	/* 0-128M */
	int i,j;
	for(i=0;i<NR_DIR;i++){
		page_dir2[i] = &page_table2[i][0] + 7;
		for(j=0;j<1024;j++){
			page_table2[i][j] = (i*1024+j)*PAGE_SIZE + 7;
		}
	}
	/* 将page_dir2的地址写给cr3 */
	/* 在写CR3前，需要将CR0的PG标志位清0，否则将产生GP */
	__asm__ __volatile__("cli\n\t"	\
			"mov %%cr0, %%eax\n\t" \
			"andl $0x7fffffff, %%eax\n\t" \
			"mov %%eax, %%cr0\n\t" \
			"mov %0, %%cr3"	\
			:: "r"(page_dir2));
}






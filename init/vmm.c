
/*
 * 对分页机制的支持的相关函数
 *
 */

#include <console.h>
#include <debug.h>
#include <vmm.h>


static unsigned int page_dir[1024]; // 1024个页目录项
static unsigned int *page_tlb = PAGE_TLB_START; //页表起始地址在页目录开始地址的4k后

/*
 * 初始化虚拟地址
 */
void init_vmm()
{
	int i=0;
	for(i=0;i<1024;i++){
		page_dir[i] = page_tlb + i*4096;
	}

}







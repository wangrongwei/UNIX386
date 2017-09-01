/*
 * 对分页机制的支持的相关函数
 *
 */
#include <console.h>
#include <debug.h>
#include <vmm.h>

static unsigned int page_dir[1024];
static unsigned int *page_tlb = PAGE_TLB_START;

void init_vmm()
{
	int i=0;
	for(i=0;i<1024;i++){
		page_dir[i] = page_tlb + i*4096;
	}

}







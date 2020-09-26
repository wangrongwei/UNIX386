
/*
 * 物理内存的管理
 * 此程序有问题，主要是还没有理解透在何时分配物理地址，此外虚拟地址的初始化没有健全
 */


#include <pmm.h>
#include <debug.h>
#include <unistd.h>
#include <slab.h>

/* 这个数组存储的就是每一页的开始地址 */
static unsigned int pmm_stack[PAGE_MAX_COUNT+1];
unsigned int pmm_stack_top;

/* phy_page_count仅在init_pmm初始化后，不再变化 */
unsigned int phy_page_count=0;

/*
 * 函数功能：初始化物理地址，系统的物理内存从0x0---0x9fc00,
 * 0x100000---0x7ffe000是RAM区可使用
 */
void init_pmm()
{
	unsigned int page_addr = MAIN_MEMORY_START+1024*1024*2;

	printk("physicial memory init\n");
	pmm_stack_top = 0;
	// 到0x7ffd000,不在往下分配（128M）
	while(page_addr <= (MAIN_MEMORY_END - PAGE_SIZE)){
		if(page_addr <= 0x9e000){
			pmm_free_page(page_addr);
			phy_page_count++;
			page_addr += PAGE_SIZE;
			if(page_addr == 0x9f000){
				page_addr = 0x100000;
			}
		}
		else{
			pmm_free_page(page_addr);
			phy_page_count++;
			page_addr += PAGE_SIZE;
		}
	}

}


/*
 * 将分配的每一页（4KB）开头地址存储到pmm_stack数组
 */
void pmm_free_page(unsigned int p)
{
	pmm_stack[pmm_stack_top++] = p;
	//printk("pmm_stack_top = 0x%08X\n",&pmm_stack_top);
}

/*
 * 分配一个页
 */
uint32_t pmm_alloc_page()
{
	uint32_t page;

	page = pmm_stack[--pmm_stack_top];
	if(pmm_stack_top == 0){
		return NULL;
	}
	//printk("pmm_stack = 0x%08X\n",pmm_stack);
	return page;
}

/*
 * 释放多个连续页
 */
void pmm_free_pages(unsigned int p, int count)
{
	int i;

	for (i=0; i<count; i++) {
		pmm_stack[pmm_stack_top++] = p + 0x1000 * i;
		//printk("pmm_stack_top = 0x%08X\n",&pmm_stack_top);
	}
}

/*
 * 分配多个连续页
 */
uint32_t pmm_alloc_pages(int count)
{
	int i;
	uint32_t page, ret;

	ret = pmm_stack[--pmm_stack_top];
	if(pmm_stack_top == 0){
		/* NOT enough page */
		return NULL;
	}

	for (i=0; i<count-1; i++) {
		page = pmm_stack[--pmm_stack_top];
		if(pmm_stack_top == 0){
			/* NOT enough page */
			return NULL;
		}
	}
	return ret;
}

/* 
 * 按字节分配（暂时未用到flags） 
 */
void *kmalloc(uint32_t size,int flags)
{
	/* 先实现一个简单的分配粒度为粒度的字节的kmalloc */
	/* 不考虑内存碎片 */
	/* TODO 实现普通的kmalloc分配 */
	return basic_allocator(size);
}

/* 释放 */
void kfree()
{


}






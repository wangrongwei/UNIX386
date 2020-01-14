
/*
 * 物理内存的管理
 * 此程序有问题，主要是还没有理解透在何时分配物理地址，此外虚拟地址的初始化没有健全
 */


#include <pmm.h>
#include <debug.h>
#include <unistd.h>


#define PMM_MAX_SIZE 0x100000000 // 512MB
#define PMM_START_ADDR 0x0      // 就从这开始分配每一页
#define PMM_END_ADDR 0xfffff000 // 4G大小
#define PAGE_SIZE 0x1000	// 每一页大小为4KB

/* 总共有多少页 */
#define PAGE_MAX_COUNT (PMM_MAX_SIZE / PAGE_SIZE)

/* 这个数组存储的就是每一页的开始地址 */
static unsigned int pmm_stack[PAGE_MAX_COUNT+1];
static unsigned int pmm_stack_top;
unsigned int phy_page_count=0;

/*
 * 函数功能：初始化物理地址，系统的物理内存从0x0---0x9fc00,
 * 0x100000---0x7ffe000是RAM区可使用
 */
void init_pmm()
{
	printk("physicial memory init\n");
	unsigned int page_addr = PMM_START_ADDR;
	pmm_stack_top = 0;
	// 到0x7ffd000,不在往下分配
	while(page_addr <= (0x007ffe000 - PAGE_SIZE)){
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
	//printk("pmm_stack = 0x%08X\n",pmm_stack);
	return page;
}

/* 按字节分配（暂时未用到flags） */
void *kmalloc(uint32_t size,int flags)
{

}

/* 释放 */
void kfree()
{


}






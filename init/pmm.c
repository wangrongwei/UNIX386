/*
 * 物理内存的管理
 */
#include <pmm.h>

#define PMM_MAX_SIZE 0x20000000 // 512MB
#define PMM_START_ADDR 0x100000 // 就从这开始分配每一页
#define PMM_END_ADDR 0x07ffe000
#define PAGE_SIZE 0x1000	// 每一页大小为4KB

#define PAGE_MAX_COUNT (PMM_MAX_SIZE / PAGE_SIZE)


static unsigned int pmm_stack[PAGE_MAX_COUNT+1];
static unsigned int pmm_stack_top;
unsigned int phy_page_count=0;

void init_pmm()
{
	unsigned int page_addr = PMM_START_ADDR;

	// 到0x07ffd000,不在往下分配
	while(page_addr <= (PMM_END_ADDR - PAGE_SIZE))
	{
		pmm_free_page(page_addr);
		phy_page_count++;
		page_addr +=PAGE_SIZE;

	}


}

/*
 * 将分配的每一页（4KB）开头地址存储到pmm_stack数组
 */
void pmm_free_page(unsigned int p)
{
	pmm_stack[++pmm_stack_top] = p;

}
/*
 * 分配一个页
 */
unsigned int pmm_alloc_page()
{
	unsigned int page;
	page = pmm_stack[pmm_stack_top--];
	return page;
}










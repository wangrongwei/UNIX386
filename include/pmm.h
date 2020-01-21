#ifndef __PMM_H__
#define __PMM_H__

#include <unistd.h>


/* 主内存开始地址 */
#define MAIN_MEMORY_START (1024*512*9)
#define MAIN_MEMORY_END (1024*1024*128)

#define PMM_MAX_SIZE (1024*1024*128) // 512MB
#define PMM_START_ADDR (0)      // 就从这开始分配每一页
#define PMM_END_ADDR (1024*1024*128) // 128M
#define PAGE_SIZE 0x1000	// 每一页大小为4KB

/* 总共有多少页 */
#define PAGE_MAX_COUNT (PMM_MAX_SIZE / PAGE_SIZE)

extern unsigned int phy_page_count;
extern unsigned int pmm_stack_top;

void init_pmm(void);
void pmm_free_page(uint32_t p);
uint32_t pmm_alloc_page(void);

void *kmalloc(uint32_t size, int flags);
void kfree();


#endif




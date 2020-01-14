#ifndef __PMM_H__
#define __PMM_H__

#include <unistd.h>


void init_pmm(void);
void pmm_free_page(uint32_t p);
uint32_t pmm_alloc_page(void);

void *kmalloc(uint32_t size, int flags);
void kfree();


#endif





#ifndef __BUDDY_H__
#define __BUDDY_H__

#include <unistd.h>
#include <memory.h>
#include <list.h>

struct buddy_element{
	uint32_t index; /* 空闲数量 */
	uint32_t *data;
	struct list_head order_list_head;
};

extern struct buddy_element *buddy_order[];

uint32_t buddy_alloc_pages(int count, int flags);
int buddy_alloc_scheme(int count);

#endif



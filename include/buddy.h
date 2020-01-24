
#ifndef __BUDDY_H__
#define __BUDDY_H__

#include <unistd.h>
#include <list.h>

struct buddy_element{
	uint32_t index; /* 空闲数量 */
	uint32_t *data;
	struct list_head order_list_head;
};

uint32_t alloc_pages(int count, int flags);


#endif



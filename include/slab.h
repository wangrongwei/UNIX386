
#ifndef __SLAB_H__
#define __SLAB_H__

#include <unistd.h>

void *basic_allocator(uint32_t count);

typedef struct kmem_cache_struct{
	int a;
}kmem_cache_t;


#endif



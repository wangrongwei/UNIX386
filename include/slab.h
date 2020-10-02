
#ifndef __SLAB_H__
#define __SLAB_H__

#include <unistd.h>

void *basic_allocator(uint32_t count);

typedef struct kmem_cache_struct{
	int a;
} kmem_cache_t;


kmem_cache_t *kmem_cache_create(char *name, 
	size_t size, 
	int align, 
	void (*constructor)(void *, size_t),
	void (*destructor)(void *, size_t));

void kmem_cache_destroy(kmem_cache_t *cp);


void *kmem_cache_alloc(kmem_cache_t *cp, int flags);
void kmem_cache_free(kmem_cache_t *cp, void *buf);

#endif



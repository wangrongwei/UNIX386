
/*
 * slab分配器
 */
#include <slab.h>
#include <pmm.h>
#include <unistd.h>


long BASIC_FREE_START = MAIN_MEMORY_START;

#define BASIC_MEMORY_START MAIN_MEMORY_START
#define BASIC_MEMORY_END (MAIN_MEMORY_START+1024*1024*2)

/*
 * basic allocator（在伙伴系统未初始化前负责内存管理）
 */
void *basic_allocator(uint32_t size)
{
	/* 从主内存中取出2M用于basic allocator管理 */
	long free = BASIC_FREE_START;
	BASIC_FREE_START += size;
	if(BASIC_FREE_START >= BASIC_MEMORY_END){
		return NULL;
	}
	return free;
}

/*
 * TODO 构建kmem_cache
 */
kmem_cache_t *kmem_cache_create(char *name, size_t size, int align, 
	void (*constructor)(void *, size_t),
	void (*destructor)(void *, size_t))
{
	return NULL;
}


/*
 * TODO 释放kmem_cache
 */
void kmem_cache_destroy(kmem_cache_t *cp)
{
	return;
}

/*
 * TODO 从一个kmem_cache中分配一个object
 */
void *kmem_cache_alloc(kmem_cache_t *cp, int flags)
{
	return NULL;
}

/*
 * TODO 与kmem_cache_alloc动作相反
 */
void kmem_cache_free(kmem_cache_t *cp, void *buf)
{
	return;
}



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




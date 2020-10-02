

#ifndef __MEMORY_H__
#define __MEMORY_H__


/*
 * TODO 分配器，对basic，slab等分配器做统一管理
 */
struct memory_allocator_t {
	char *name;

};

/* 物理地址 */
typedef uint32_t phys_addr_t;

/* 虚拟地址 */
typedef uint32_t virt_addr_t;



#endif



/*
 * 伙伴系统
 */

#include <buddy.h>
#include <unistd.h>
#include <list.h>


#define MAX_ORDER 11

//uint32_t pmm_alloc_page()
//phy_page_count


struct buddy_element{
	uint32_t index; /* 空闲数量 */
	uint32_t *data;
	struct list_head order_list_head;
}

struct buddy_element buddy_order[MAX_ORDER];

/*
 * 初始化buddy system
 */
void init_buddy()
{
	int i,j;
	struct buddy_element *pr_be=NULL,*pr_prev=NULL,*pr_next=NULL;
	uint32_t average = phy_page_count;
	for(i=0;i<MAX_ORDER;i++){
		for(j=0;j<average;j++){
			pr_be = (struct buddy_element *)basic_allocator(sizeof(struct buddy_element));
			pr_be->index = j;
			pr_be->data = (uint32_t *)alloc_pages(1<<i, 0);
			if(pr_prev != NULL){
				pr_prev->order_list_head.next = pr_be;
				pr_be->order_list_head.prev = pr_prev;
			}
			pr_prev = pr_be;
		}
	}
	

}

/* 分配页 */
uint32_t alloc_pages(int count, int flags)
{
	int nr_pages = count;
	uint32_t pr = pmm_alloc_page();
	/* 更新栈顶 */
	pmm_stack_top -= count-1;
	return pr;
}





/*
 * 伙伴系统
 */

#include <buddy.h>
#include <unistd.h>
#include <pmm.h>
#include <list.h>


#define MAX_ORDER 5

//uint32_t pmm_alloc_page()
static uint32_t average = 20;
static uint32_t nr_free_pages;
static uint32_t nr_pages;

struct *buddy_element buddy_order[MAX_ORDER];
static int nr_buddyfree_order[MAX_ORDER];

/*
 * 初始化buddy system
 */
void init_buddy()
{
	/* 一个简单的伙伴系统 */
	int i,j;
	struct buddy_element *pr_be=NULL, *pr_prev=NULL, *pr_next=NULL;

	printk("init buddy\n");
	/* phy_page_count代表当前页总数 */
	nr_pages = phy_page_count;
	nr_free_pages  = phy_page_count;

	/*
	 * 为buddy系统order数组赋值
	 */
	for (i=0; i<MAX_ORDER; i++) {
		for (j=0; j<average; j++) {
			pr_be = (struct buddy_element *)basic_allocator(sizeof(struct buddy_element));
			if (pr_be == NULL) {
				printk("error: 1\n");
				return;
			}
			pr_be->index = j;
			pr_be->data = (uint32_t *)pmm_alloc_pages(1<<i);
			if (pr_be->data == NULL) {
				printk("error: 2\n");
				return;
			}

			if (pr_prev != NULL) {
				pr_prev->order_list_head.next = pr_be;
				pr_be->order_list_head.prev = pr_prev;
			} else {
				/* first element */
				buddy_order[i] = pr_be;
			}
			pr_prev = pr_be;
			nr_free_pages = nr_pages - (1 << i);
		}
		nr_buddyfree_order[j] = average;
	}
}

/*
 * 按照buddy策略，为count数量页选择合适的order分配
 *
 */
int buddy_alloc_scheme(int count)
{
	int i;
	int index = 0;

	/*
	 * EASY scheme
	 */
	for (i=0; i<MAX_ORDER; i++) {
		if (nr_buddyfree_order[i] == 0) {
			printk("NO free order\n");
			continue;
		}
		if (count < (1<<i)) {
			index = i;
			nr_buddyfree_order[i]--;
			break;
		}
	}

	if (i == MAX_ORDER) {
		printk("NOT enough free pages\n");
		/* 睡眠等待或者释放其他页 */
	}

	return index;
}

/* 
 * buddy系统向上提供的分配接口
 */
uint32_t buddy_alloc_pages(int count, int flags)
{
	int pages = count;
	uint32_t ret;
	int index;
	struct *buddy_element be;

	/* buddy_order[MAX_ORDER]; */
	/*
	 * 选择合适的order，并从buddy_order分配
	 */
	index = buddy_alloc_scheme(pages);

	/* There is a serious bug, NEED to FIX later. */
	be = buddy_order[index];
	buddy_order[index] = be->order_list_head->next;

	return ret = (phys_addr_t)be->data;
}



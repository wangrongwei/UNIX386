
#ifndef __INIT_H__
#define __INIT_H__

/*
 * 系统初始化函数放在此处
 */

#include <unistd.h>

extern uint32_t page_dir2[];
extern uint32_t page_table2[][1024];

void init_paging();


#endif



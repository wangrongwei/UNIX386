#ifndef __VMM_H__
#define __VMM_H__


#define PAGE_DIR_START 0x0
// 页表在页目录的后4k地址处
#define PAGE_TLB_START (PAGE_DIR_START + 0x1000)

void init_vmm();





#endif



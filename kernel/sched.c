
/*
 * 关于进程调度相关实现
 */


#include "schedule.h"
#include <task_struct.h>
#include <i386/sys.h>
#include <i386/system.h>
#include <descriptor.h>
#include "unistd.h"
#include <debug.h>
#include <interrupt.h>


extern tss_struct;
extern gdt_struct_t gdt_list[];
extern syscall_ptr system_call_table[];

#define PAGE_SIZE 4096

union task_union* task_tables[NR_TASKS] = {&init_task,};

/* 在kernel.asm需要用到 */
long user_stack[PAGE_SIZE >> 2]={0};
char kernel_stack[PAGE_SIZE << 1] __attribute__ ((aligned(16)));

long *_stack_top = &user_stack[PAGE_SIZE >> 2];
long kernel_stack_top = (long)kernel_stack + STACK_SIZE;

/* 一个全局指针，指向当前正在执行的进程的task_struct */
struct task_struct *current = &(init_task.task); 

/* 所有可以选择的调度器 */
struct scheduler scheduler_array[] {
	{"primary_sched", schedule},
	{NULL, NULL}
};

/*
 * 调度初始化，启动进程0
 */
void schedule_init(void)
{
	unsigned int base,limit;
	printk("scheduler init!\n");
	/* 在gdt表后边加上进程0的tss和ldt */
	//set_tssldt2_gdt(FIRST_TASKTSS_INDEX, &(init_task.task.tss), 0xe9);
	//set_tssldt2_gdt(FIRST_TASKLDT_INDEX, &(init_task.task.ldt), 0xe2);
	base = &(init_task.task.tss);
	limit = &(init_task.task.tss) + sizeof(init_task.task.tss);
	set_tssldt2_gdt(FIRST_TASKTSS_INDEX, base, limit, P_SET | (DPL3 << 5) | TYPE_TSS_NOTBUSY);
	base = &(init_task.task.ldt);
	limit = &(init_task.task.ldt) + sizeof(init_task.task.ldt);
	set_tssldt2_gdt(FIRST_TASKLDT_INDEX, base, limit, P_SET | (DPL3 << 5) | TYPE_LDT);
	__asm__ __volatile__("pushfl ; andl $0xffffbfff,(%esp) ; popfl");
	/* 将tss挂接到TR寄存器 */
	ltr(0);
	/* 将LDT挂接到LDTR寄存器 */
	lldt(0);

	/* 初始化时钟 */
	init_timer(HZ);

	/* 设置系统调度总入口 */
	set_system_gate(0x80,&system_call);
	printk("scheduler initial end...\n");

}

void move_to_user_mode(void)
{
	/* 从ring0转换到ring3 */
	printk("move to user mode: ring0->ring3\n"); 
	__asm__ __volatile__("cli\n\t"\
		"mov $0x2b,%%ax\n\t"\
		"mov %%ax,%%ds\n\t" \
		"mov %%ax,%%es\n\t" \
		"mov %%ax,%%fs\n\t" \
		"mov %%ax,%%gs\n\t" \
		"movl %%esp,%%eax\n\t"\
		"pushl $0x23\n\t" 	/* 压入ss */\
		"pushl %%eax\n\t" 	/* 压入sp */\
		"pushfl\n\t" 		/* 压入eflags */\
		"pushl $0x23\n\t"	/* 压入cs */\
		"pushl $1f\n\t"		/* 压入ip */\
		"iret\n"\
		"1:\t"\
		 :::"ax");

}

void init()
{
	long ss,sp,eflags,cs,ip;
	ss = init_task.task.tss.ss;
	sp = init_task.task.tss.esp0;
	eflags = init_task.task.tss.eflags;
	cs = init_task.task.tss.cs;
	ip = init_task.task.tss.eip;
	__asm__ __volatile__("cli\n\t"\
		"pushl %0\n\t"		/* 压入ss */\
		"pushl %1\n\t" 		/* 压入sp */\
		"pushl %2\n\t" 		/* 压入eflags */\
		"pushl %3\n\t"		/* 压入cs */\
		"mov %4, %%ecx\n\t"		/* 压入ip */\
		"sti\n\t"	\
		"jmp *%%ecx\n"		/* 压入ip */\
		 ::"r"(ss),"r"(sp),"r"(eflags),"r"(cs),"r"(ip));	

}

/*
 * 重新调度（时间片用完或者current.state==0）
 */
void reschedule(void)
{
	/* 重新设置current指针 */
	current->state = TASK_INTERRUPTIBLE;
	if(current->pid == 0){
		current = &(task_tables[1]->task);
	}
	else{
		current = &(task_tables[0]->task);
		
	}
	current->state = TASK_RUNNING;
	return;
}

/*
 * 根据调度策略，选择合适的task_struct作为下一个current
 */
void schedule(void)
{
	unsigned int eip,esp,ebp;
	unsigned int base,limit;
	struct task_struct *prev,*next;
	int n,pid;

	__asm__ __volatile__("mov %%esp, %0":"=r"(esp));
	__asm__ __volatile__("mov %%ebp, %0":"=r"(ebp));
	prev = current;
	if(current->pid == 0){
		current = &task_tables[1]->task;
	}
	else{
		current = &task_tables[0]->task;
	}
	next = current;
	current->state = TASK_RUNNING;
	pid = current->pid;
	if(current != NULL){
		/* 在gdt表后边加上进程0的tss和ldt */
		base = &(current->tss);
		limit = &(current->tss) + sizeof(current->tss);
		set_tssldt2_gdt(FIRST_TASKTSS_INDEX, base, limit, P_SET | (DPL3 << 5) | TYPE_TSS_NOTBUSY);

		base = &(current->ldt);
		limit = &(current->ldt) + sizeof(current->ldt);
		set_tssldt2_gdt(FIRST_TASKLDT_INDEX, base, limit, P_SET | (DPL3 << 5) | TYPE_LDT);
		
		//eip = current->tss.eip;
		//current->tss.esp = esp;
		//current->tss.ebp = ebp;
		/* 进程切换 */
		switch_to(prev,next,prev);
	}
}

/*
 * 保存上下文
 */
void save_context(pt_regs *regs)
{
	current->tss.edi = regs->edi;	// 从 edi 到 eax 由 pusha 指令压入
	current->tss.esi = regs->esi;
	current->tss.ebp = regs->ebp;
	current->tss.esp = regs->esp;
	current->tss.ebx = regs->ebx;
	current->tss.eflags = regs->eflags;
}


 /*
  * 切换到task_[n]，首先需要检测n不是当前current，否则不做任何事；
  * 如果task_[n]用到math co-processor，需要清空TS-flag；
  */
struct task_struct fastcall * __switch_to(struct task_struct *prev, struct task_struct *next) 
{
	struct {long a,b;} __tmp;
	unsigned int eip,esp,ebp;
	esp = next->tss.esp0;
	ebp = next->tss.ebp;
	eip = next->tss.eip;
	__asm__ __volatile__("cli\n\t" \
		"mov %0, %%ecx\n\t" \
		"mov %1, %%esp\n\t" \
		"mov %2, %%ebp\n\t" \
		"mov %3, %%eax\n\t" \
		"mov $0x12345, %%eax\n\t" \
		"sti\n\t" \
		"jmp *%%ecx\t" \
		:: "r"(eip), "r"(esp), "r"(ebp), "r"(current->tss.cr3));
	return prev;
}



/*
 * 准备init0
 */
void init0_ready(void)
{
	/* 设置栈 */
	struct task_struct *init0_point = &(task_tables[0]->task);
	init0_point->tss.esp0 = (long)task_tables + PAGE_SIZE;
	init0_point->tss.eip = (long)thread_cpu_idle;
	init0_point->tss.eflags = 0x3202; /* 设置IOPL=3 */
}

/*
 * 初始化开始状态下的两个线程
 */
void init_thread(void)
{
	/* 初始化init0 */
	init0_ready();
#if 1
	/* 分配一个task_struct结构体 */
	union task_union *idle_point = (union task_union *)kmalloc(sizeof(union task_union), 0);
	if(idle_point == NULL){
		panic("init_thread");
	}
	idle_point->task = task_tables[0]->task;
	task_tables[1] = idle_point;
	idle_point->task.pid = 1;
	idle_point->task.tss.esp0 = &task_tables[1] + PAGE_SIZE;
	idle_point->task.tss.eip = (long)thread_init1;
	idle_point->task.tss.eflags = 0x3202; /* 设置IOPL=3 */
#endif
}


/* 初始化一个函数作为init进程程序体 */
void thread_init1(void)
{
	static int i=0,j=0;
	while(1){
		cli();
		i++;
		if((i % 3000) == 0){
			printk("1 ");
		}
		if(i == 10000){
			i = 0;
			j++;
			
			current->state = TASK_INTERRUPTIBLE;
			schedule();
			
		}
		sti();
	};
}


/* 
 * CPU空闲状态下运行的内核线程 
 */
void thread_cpu_idle(void)
{
	static int i=0,j=0;
	while(1){
		cli();
		i++;
		if((i % 3000) == 0){
			printk("0 ");
		}
		if(i == 10000){
			i = 0;
			j++;
			
			current->state = TASK_INTERRUPTIBLE;
			schedule();
			
		}
		sti();
	};
}


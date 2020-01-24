/*
 *      kernel.ld -- 针对 kernel 格式所写的链接脚本
 *
 * 首先，我们声明了内核程序的入口地址是符号 "start"，这个脚本告诉 ld 程序 如何构造我们的内核映像文件。
 * 然后，我们声明了第一个段 .text 段（代码段）以及它的起始地址 0x100000（1MB）。
 * 接着是 已初始化数据段 .data 和 未初始化数据段 .bss 以及它们采用的4096的页对齐方式。
 * Linux GCC 增加了额外的数据段 .rodata，这是一个只读的已初始化数据段，放置常量什么的。
 * 简单起见，我们把和 .data 段放在一起好了。
 *
 * 修改<2017/08/23>
 *      由于kernel.bin是一个elf文件，所以这里的0x9000地址指明的是_start的加载地址
 * 但是在生成的kernel.bin文件里前面还有这个elf文件的属性，尽管把这个.bin文件拷贝到
 * 0x8000，但是可执行段是从0x9000开始
 * 
 * 修改<2017/11/17>
 * (已更改，使用objcopy删掉了包括elf头在内的信息，可直接从0x8000执行)
 *
 * 修改<2017/11/18>
 *      删掉.stab节和.stabstr节（这部分特别大）
 *       .stab :
 *       {
 *               *(.stab)
 *               . = ALIGN(4);
 *       }
 *       .stabstr :
 *       {
 *               *(.stabstr)
 *               . = ALIGN(4);
 *       }
 *
 */

ENTRY(_start)
SECTIONS
{
	. = 0x5000;
        PROVIDE(kernel_s = .);
	.text :
	{
		*(.text)
		. = ALIGN(4);
	}
	.data :
	{
		*(.data)
                *(.rodata)
		. = ALIGN(4);
	}
	.bss :
	{
		*(.bss)
		. = ALIGN(4);
	}
        PROVIDE(kernel_e = .);

}


;	deeppinkos
;-------------------------------------------------------------------------------
;                                      BIOS内存映射
;-------------------------------------------------------------------------------
;	TAB=4			SP=0x7c00		0x8000=拷贝到启动区
;-------------------------------------------------------------------------------
;	|中断向量表|		|;代码存储位置|	......	| 512|
;	|	  |		|	     |	......	|    |
;-------------------------------------------------------------------------------
;								0x8200=后边的程序
;	在某种意义上理解，我们的程序只需要在磁盘的开始512字节就行
;	然后硬件上自动去读这512，然后执行这段程序

; 1-> 读取磁盘后边扇区的数据
; 2-> 在bootsecond.nas中添加LCD支持
; 3-> 初始化PIC
; 4-> 打开A20，进入保护模式
; 5-> 设置CR0的PE和PG
; 6-> 更新D、E、F、G、S，其中数代表的是第几个GDT

GLOBAL  _start
GLOBAL	myprintf
BOTPAK	EQU		0x00280000
DSKCAC	EQU		0x00100000
DSKCAC0	EQU		0x00008000

;BOOT_INFO信息
CYLS		EQU	0x0ff0
LEDS		EQU	0x0ff1
LCDMODE		EQU	0x0ff2  ;
SCREENX		EQU	0x0ff4  ;	x
SCREENY		EQU	0x0ff6  ;	y
LCDRAM		EQU	0x0ff8  ; 图像缓冲区的开始地址
[bits 16]
_start:
	ORG	0x8200
	MOV	AH,0x0e
	MOV	AL,'1'
	INT	0x10

[bits 32]
	LGDT	[GDTR0]
	MOV	EAX,CR0
	AND	EAX,0x7fffffff
	OR	EAX,0x00000001
	MOV	CR0,EAX       ;打开段级保护，不开分页机制

	JMP	0x8:PM_MODE
PM_MODE:
	MOV	AX,8
	MOV	DS,AX
	MOV	ES,AX
	MOV	FS,AX
	MOV	GS,AX
	MOV	SS,AX
	
	;MOV	BYTE [GS:dword 0xb8002],'1'	

	JMP	$
	;KERNEL_ADDR
	
	ALIGNB	16
;
;进入保护模式后，不再按照CS*16+IP取指令执行，需要按照向全局描述符
;	具体可参考《linux内核设计的艺术》
;
GDT0:
	RESB	8
	DW	0xffff,0x0000,0x9200,0x00cf ;---段基地址 0x00cf取00，0x9200取00，0x0000取全部===0x00000000
	DW	0xffff,0x0000,0x9a28,0x0047 ;---段基地址 0x0047取00，0x9a28取28，0x0000取全部===0x00280000
	DW	0

GDTR0:
	DW	23
	DD	GDT0




	
		



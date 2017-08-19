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
[bits 32]
GLOBAL  _start
GLOBAL	myprintf
EXTERN  kernel_start

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


_start:
        ;INT     0x10
	MOV     ESP,0x00007000
        MOV     EBP,ESP
        ;AND     ESP,0FFFFFFF0H
        ;MOV     AH,0x0f
        ;MOV     AL,'V'
        ;MOV     BYTE [0xb8008],'V'
        MOV     EAX,kernel_start
        JMP     EAX
        ;SUB     EAX,0x8080
        ;CALL    EAX

;stop:
;        JMP     stop
;stack:
 ;       times   32768 db 0
;glb_mboot_ptr:
;	resb	4

;STACK_TOP equ $-stack-1





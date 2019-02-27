;					Deeppinkos
;-------------------------------------------------------------------------------
;                                      BIOS内存映射
;-------------------------------------------------------------------------------
;	TAB=4			SP=0x7c00		0x8000=拷贝到启动区
;-------------------------------------------------------------------------------
;	|中断向量表|		|代码存储位置|	......	| 512|
;	|	  |	|	     |	......	|    |
;-------------------------------------------------------------------------------
;								0x8200=后边的程序
;	在某种意义上理解，我们的程序只需要在磁盘的开始512字节就行
;	然后硬件上自动去读这512，然后执行这段程序

; 1-> 读取磁盘后边扇区的数据
; 2-> 在bootsecond.nas中添加LCD支持
; 3-> 初始化PIC
; 4-> 打开A20，进入保护模式
; 5-> 设置CR0的PE和PG
; 6-> 跳转到0地址，即执行该程序
; 7-> 更新D、E、F、G、S，其中数代表的是第几个GDT
; 8-> 设置页保护机制
; 9-> 跳转到kernel_start函数

[bits 32]
global  _start
global	myprintf
global  load_gdtr
global  load_idtr
global  write_vram

extern  kernel_start
extern  _stack_top

;BOOT_INFO信息
CYLS		EQU	0x0ff0
LEDS		EQU	0x0ff1
LCDMODE		EQU	0x0ff2  ;
SCREENX		EQU	0x0ff4  ;	x
SCREENY		EQU	0x0ff6  ;	y
LCDRAM		EQU	0x0ff8  ; 图像缓冲区的开始地址
;页信息
;只有16M，因此只需要4个页目录
_page_dir       EQU     0x0000
_page_tab0      EQU     0x1000
_page_tab1      EQU     0x2000
_page_tab2      EQU     0x3000
_page_tab3      EQU     0x4000


;采用Intel汇编格式
_start:
	MOV	EAX,0x00000010
	MOV	DS,AX
	MOV	ES,AX
	MOV	FS,AX
	MOV	GS,AX
	MOV	SS,AX
; 设置页目录表和页目录
        CALL    setup_paging
; 开始打开分页机制
        XOR     EAX,EAX
        MOV     EAX,CR3 ;将_page_dir地址0x0000写给CR3
        MOV     EAX,CR0
        OR      EAX,0x80000000
        MOV     CR0,EAX ;PG位置1

        ;MOV     EAX,0x8000
        ;INT     0x10
	MOV     ESP,_stack_top
        MOV     EBP,ESP
        ;AND     ESP,0FFFFFFF0H
        ;MOV     AH,0x0f
        ;MOV     AL,'V'
        ;MOV     BYTE [0xb8008],'V'
; 跳转到C语言部分
        MOV     EAX,kernel_start
        JMP     EAX
        ;SUB     EAX,0x8080
        ;CALL    EAX
;--------------------------------------------
;--------------------------------------------
;设置页目录表和页目录
;       将页目录表和页目录放在内存的开始地址
;--------------------------------------------
setup_paging:
        MOV     ECX,1024*5
        XOR     EAX,EAX
        XOR     EDI,EDI
        CLD;EDI往加的方向执行
        REP     STOSD ;每次移动4个字节,直到ECX==0(目的是将这5K内存清0)
        ;页目录表只用到了前面16字节
        ;7：表示 U/S R/W P都为1
        MOV     dword [_page_dir],_page_tab0+7
        MOV     dword [_page_dir+4],_page_tab1+7
        MOV     dword [_page_dir+8],_page_tab2+7
        MOV     dword [_page_dir+12],_page_tab3+7

        MOV     EAX,0xFFF007 ;16M-4K位置
        MOV     EDI,_page_tab3+4092;
        STD;EDI往减的方向执行
abcd:   STOSD
        SUB     EAX,0x1000
        JGE     abcd

        RET
; 加载gdtr地址需要，后面函数调用
load_gdtr:
        MOV     EAX,[ESP+4]
        LGDT    [EAX]
        MOV     AX,0x10
        MOV     DS,AX
        MOV     ES,AX
        MOV     FS,AX
        MOV     GS,AX
        MOV     SS,AX

        JMP     0x08:.flush
.flush:
        ret

load_idtr:
        MOV     EAX,[ESP+4]
        LIDT    [EAX]
        ret
write_vram:
	MOV ECX,[ESP+4]
	MOV AL,[ESP+8]
	MOV [ECX],AL
	RET

; 定义两个构造中断处理函数的宏(有的中断有错误代码，有的没有)
; 用于没有错误代码的中断
%macro ISR_NOERRCODE 1
[GLOBAL isr%1]
isr%1:
	cli      ; 首先关闭中断
	push 0   ; push 无效的中断错误代码(起到占位作用，便于所有isr函数统一清栈)
	push %1  ; push 中断号
	jmp isr_common_stub
%endmacro

; 用于有错误代码的中断
%macro ISR_ERRCODE 1
[GLOBAL isr%1]
isr%1:
	cli                         ; 关闭中断
	push %1                     ; push 中断号
	jmp isr_common_stub
%endmacro

; 定义中断处理函数
ISR_NOERRCODE  0 	; 0 #DE 除 0 异常
ISR_NOERRCODE  1 	; 1 #DB 调试异常
ISR_NOERRCODE  2 	; 2 NMI
ISR_NOERRCODE  3 	; 3 BP 断点异常
ISR_NOERRCODE  4 	; 4 #OF 溢出
ISR_NOERRCODE  5 	; 5 #BR 对数组的引用超出边界
ISR_NOERRCODE  6 	; 6 #UD 无效或未定义的操作码
ISR_NOERRCODE  7 	; 7 #NM 设备不可用(无数学协处理器)
ISR_ERRCODE    8 	; 8 #DF 双重故障(有错误代码)
ISR_NOERRCODE  9 	; 9 协处理器跨段操作
ISR_ERRCODE   10 	; 10 #TS 无效TSS(有错误代码)
ISR_ERRCODE   11 	; 11 #NP 段不存在(有错误代码)
ISR_ERRCODE   12 	; 12 #SS 栈错误(有错误代码)
ISR_ERRCODE   13 	; 13 #GP 常规保护(有错误代码)
ISR_ERRCODE   14 	; 14 #PF 页故障(有错误代码)
ISR_NOERRCODE 15 	; 15 CPU 保留
ISR_NOERRCODE 16 	; 16 #MF 浮点处理单元错误
ISR_ERRCODE   17 	; 17 #AC 对齐检查
ISR_NOERRCODE 18 	; 18 #MC 机器检查
ISR_NOERRCODE 19 	; 19 #XM SIMD(单指令多数据)浮点异常

; 20~31 Intel 保留
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31
; 32～255 用户自定义
ISR_NOERRCODE 255

[GLOBAL isr_common_stub]
[EXTERN isr_handler]
; 中断服务程序
isr_common_stub:
	pusha            ; Pushes edi, esi, ebp, esp, ebx, edx, ecx, eax
	mov ax, ds
	push eax         ; 保存数据段描述符
	
	mov ax, 0x10     ; 加载内核数据段描述符表
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	
	push esp	   ; 此时的 esp 寄存器的值等价于 pt_regs 结构体的指针
	call isr_handler   ; 在 C 语言代码里
	add esp, 4 	   ; 清除压入的参数
	
	pop ebx            ; 恢复原来的数据段描述符
	mov ds, bx
	mov es, bx
	mov fs, bx
	mov gs, bx
	mov ss, bx
	
	popa               ; Pops edi, esi, ebp, esp, ebx, edx, ecx, eax
	add esp, 8         ; 清理栈里的 error code 和 ISR
	iret
.end:



; 构造中断请求的宏
%macro IRQ 2
[GLOBAL irq%1]
irq%1:
	cli
	push 0
	push %2
	jmp irq_common_stub
%endmacro

IRQ   0,    32 	; 电脑系统计时器
IRQ   1,    33 	; 键盘
IRQ   2,    34 	; 与 IRQ9 相接，MPU-401 MD 使用
IRQ   3,    35 	; 串口设备
IRQ   4,    36 	; 串口设备
IRQ   5,    37 	; 建议声卡使用
IRQ   6,    38 	; 软驱传输控制使用
IRQ   7,    39 	; 打印机传输控制使用
IRQ   8,    40 	; 即时时钟
IRQ   9,    41 	; 与 IRQ2 相接，可设定给其他硬件
IRQ  10,    42 	; 建议网卡使用
IRQ  11,    43 	; 建议 AGP 显卡使用
IRQ  12,    44 	; 接 PS/2 鼠标，也可设定给其他硬件
IRQ  13,    45 	; 协处理器使用
IRQ  14,    46 	; IDE0 传输控制使用
IRQ  15,    47 	; IDE1 传输控制使用

[GLOBAL irq_common_stub]
[EXTERN irq_handler]
irq_common_stub:
	pusha                    ; pushes edi, esi, ebp, esp, ebx, edx, ecx, eax
	
	mov ax, ds
	push eax                 ; 保存数据段描述符
	
	mov ax, 0x10  		 ; 加载内核数据段描述符
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	
	push esp
	call irq_handler
	add esp, 4
	
	pop ebx                   ; 恢复原来的数据段描述符
	mov ds, bx
	mov es, bx
	mov fs, bx
	mov gs, bx
	mov ss, bx
	
	popa                     ; Pops edi,esi,ebp...
	add esp, 8     		 ; 清理压栈的 错误代码 和 ISR 编号
	iret          		 ; 出栈 CS, EIP, EFLAGS, SS, ESP
.end:





; =============================================================================
;				sys_call
; 目前系统调用的函数定义和system_call_table数组在include/i386/sys.h下                                
; =============================================================================
; 系统调用入口函数
[GLOBAL sys_call]
sys_call:
; =============================================================================
;                                   save
; =============================================================================
	pushad          ;  \
        push    ds      ;  |
        push    es      ;  | 保存原寄存器值
        push    fs      ;  |
        push    gs      ; /

	;; 注意，从这里开始，一直到 `mov esp, StackTop'，中间坚决不能用 push/pop 指令，
	;; 因为当前 esp 指向 proc_table 里的某个位置，push 会破坏掉进程表，导致灾难性后果！

	mov	esi, edx	; 保存 edx，因为 edx 里保存了系统调用的参数
				;（没用栈，而是用了另一个寄存器 esi）
	mov	dx, ss
	mov	ds, dx
	mov	es, dx
	mov	fs, dx

	mov	edx, esi	; 恢复 edx

        mov     esi, esp                    ;esi = 进程表起始地址

	; 下面这段代码！
        inc     dword [k_reenter]           ;k_reenter++;
        cmp     dword [k_reenter], 0        ;if(k_reenter == 0)
        jne     .1                          ;{
        mov     esp, StackTop               ;  mov esp, StackTop <--切换到内核栈
        push    restart                     ;  push restart
        jmp     [esi + RETADR - P_STACKBASE];  return;
.1:                                         ;} else { 已经在内核栈，不需要再切换
        push    restart_reenter             ;  push restart_reenter
        jmp     [esi + RETADR - P_STACKBASE];  return;
                                            ;}

        sti
	push	esi

	push	dword [p_proc_ready]
	push	edx
	push	ecx
	push	ebx
        call    [system_call_table + eax * 4]
	add	esp, 4 * 4

	pop	esi
        mov     [esi + EAXREG - P_STACKBASE], eax
        cli

        ret


;
; 保留此处
;stop:
;        JMP     stop
;stack:
 ;       times   32768 db 0
;glb_mboot_ptr:
;	resb	4

;STACK_TOP equ $-stack-1





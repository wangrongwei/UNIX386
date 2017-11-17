;	Deeppinkos
;	TAB=4	SP=0x7c00		0x8000=拷贝到启动区
;-------------------------------------------------------------------------------
;|中断向量表|	|;代码存储位置 |......	|512|
;|	    |	|	       |......  |   |
;-------------------------------------------------------------------------------
;						0x8200=后边的程序
;在某种意义上理解，我们的程序只需要在磁盘的开始512字节就行
;然后硬件上自动去读这512，然后执行这段程序

;目前在boot.asm里边读取了180kb程序，超过这个数字，需要更改boot.asm程序

;1-> 读取磁盘后边扇区的数据
;2-> 在bootsecond.nas中添加LCD支持
;3-> 初始化PIC
;4-> 打开A20，进入保护模式
;5-> 跳转到_start
;6-> 设置堆栈，跳转到kernel_start函数
;7->

[bits 16]
CYLS		EQU	10	;读10个柱面
BOOT_ADDR	EQU	0x7C00
KERNEL_ADDR	EQU	0xC0000000
INITSEG         EQU     0x9000

LCDMODE		EQU	0x0ff2  ;
SCREENX		EQU	0x0ff4  ;	x
SCREENY		EQU	0x0ff6  ;	y
LCDRAM		EQU	0x0ff8  ; 图像缓冲区的开始地址

	ORG	BOOT_ADDR	;必须是这个地址
;初始化段SS=0 DS=0 ES=0 SP指向程序加载的地址
;因为程序加载的地址是0x7c00，所以我们的段地址必须是0，
;不然地址就不是0x7c00了
	MOV	AX,CS
	MOV	SS,AX
	MOV	SP,BOOT_ADDR
	MOV	DS,AX
	MOV	ES,AX
;
;The second stage
;	Now ,load kernel from img
;
	MOV	SI,msg
putloop:
	MOV	AL,[SI]
	ADD	SI,1
	CMP	AL,0
	JE	Read_Sector
	MOV	AH,0x0e
	MOV	BX,0x0f
	INT	0x10		;执行BIOS中断0x10
	JMP	putloop

Read_Sector:
	MOV	SI,msg_1	;打开成功要显示字符
Read:
	MOV	AL,[SI]
	ADD	SI,1
	CMP	AL,0
	JE	Read_Ok
	MOV	AH,0x0e
	MOV	BX,0x0f
	INT	0x10		;执行BIOS中断0x10
	JMP	Read
;下面开始读磁盘程序数据
;代码借鉴《30天...》川和秀实
;ES:BX	代表缓冲器地址
Read_Ok:
        MOV	AX,0x0800	;原则上来说把启动区也要拷贝过来，就改成0x0800
	MOV	ES,AX
	MOV	BX,0x00
	MOV	CH,0		;柱面0
	MOV	DH,0		;磁头0
	MOV	CL,2		;扇区2
readloop:
	MOV	SI,0
retry:
	MOV	AH,0x02		;AH=0x02:读扇区
	MOV	AL,1		;读扇区数
	MOV	BX,0
	MOV	DL,0x00		;代表读的软盘
	INT	0x13		;产生读中断
	JNC	next		;跳转，代表这个扇区读成功
	ADD	SI,1		;SI+1(执行这代表读取失败)
	CMP	SI,5		;SI与5比较(再尝试读取5次)
	JAE	error		;SI >=	5，则跳转到error
	MOV	AH,0x00
	MOV	DL,0x00
	INT	0x13		;开始读取软盘程序
	JMP	retry
next:
	MOV	AX,ES		;缓冲地址往后0x200
	ADD	AX,0x0020
	MOV	ES,AX		;ADD ES,0x020这个指令没有
	ADD	CL,1		;读后一个扇区
	CMP	CL,18		;CL与18比较
	JBE	readloop	;如果CL <=18 则跳转
	MOV	CL,1		;||||
	ADD	DH,1		;||||	磁头指向下一个，又开始读下一个第一个扇区
	CMP	DH,2		;||||
	JB	readloop	;DH < 2 跳转
	MOV	DH,0
	ADD	CH,1		;---------最终改变读到柱面
	CMP	CH,CYLS
	JB	readloop	;CH < CYLS跳转
;10*2*18*512=
	MOV	[0x0ff0],CH

;
;  打印成功读取状态
;	换显示坐标
copy_start:
        CLD
        MOV     AX,0x07c0       ;源地址
        MOV     DS,AX
        MOV     AX,0x9000       ;目的地址
        MOV     ES,AX

        MOV     CX,256  ;表示复制512Byte
        SUB     SI,SI           ;DS:SI--->ES:DI
        SUB     DI,DI
        REP     MOVSW   ;在linux-0.1.1中使用rep movw的intel的格式
        JMP     INITSEG:(copy_end-0x7c00)
copy_end:
        MOV     AX,CS
        MOV     DS,AX
        MOV     ES,AX
        MOV     SS,AX
        MOV     SP,0xFC00

	MOV	AH,0x02
	MOV	BX,0x0f
	MOV	DX,0x0e16
	INT	0x10

	MOV	SI,(msg_2-0x7c00);打开成功要显示字符
print_loop:
	MOV	AL,[SI]
	ADD	SI,1
	CMP	AL,0
	JE	goto_PM
	MOV	AH,0x0e
	MOV	BX,0x0f
	INT	0x10		;执行BIOS中断0x10

	JMP	print_loop;
;
;The third stage
;	goto PM mode
;
goto_PM:
	MOV	AL,0x03
	MOV	AH,0x00
	INT	0x10

	;MOV	BYTE [LCDMODE],8
	;MOV	WORD [SCREENX],320
	;MOV	WORD [SCREENY],200
	;MOV	DWORD [LCDRAM],0x000a0000

	MOV	AL,0XFF
	OUT	0x21,AL
	NOP
	OUT	0xa1,AL

	CLI
;
; OPEN A20
;
	CALL	waitkbd_8042
	MOV	AL,0xd1      ;写命令
	OUT	0x64,AL
	CALL	waitkbd_8042
	MOV	AL,0xdf
	OUT	0x60,AL
	CALL	waitkbd_8042 ;打开A20


	;MOV	AH,0x0e
	;MOV	AL,'O'
	;INT	0x10

	;MOV	AH,0x0e
	;MOV	AL,'S'
	;INT	0x10

        ;jmp     $
        CLI
	LGDT	[GDTR0]

        IN      AL,92h
        OR      AL,0x02
	OUT     92h,AL

        MOV	EAX,CR0
	AND	EAX,0x7fffffff
	OR	AL,1
	MOV	CR0,EAX       ;打开段级保护，不开分页机制

        JMP	dword 0x08:0  ;跳转到0x0地址（第二部分移到到0x0地址）
;
; 这一部分移到kernel.asm里边
;
;[bits 32]
;PM_MODE:
;	MOV	EAX,0x00000010
;	MOV	DS,AX
;	MOV	ES,AX
;	MOV	FS,AX
;	MOV	GS,AX
;	MOV	SS,AX

        ;MOV     EAX,0x0000018
        ;MOV     GS,EAX

;       MOV     EAX,0x8000
;       JMP     EAX;dword 0x08:0x8200
;
;	显示需要的相关字符串
;

waitkbd_8042:
	IN	AL,0x64
	AND	AL,0x02    ;输入缓冲区是否满了？
	JNZ	waitkbd_8042 ;Yes---跳转
	RET

;
;进入保护模式后，不再按照CS*16+IP取指令执行，需要按照向全局描述符
;	具体可参考《linux内核设计的艺术》
;

GDT0:
	DW      0x0000,0x0000,0x0000,0x0000
        ;---代码段基地址 0x0047取00，0x9a28取28，0x0000取全部===0x00280000
	DW	0xffff,0x0000,0x9a00,0x00cf
        ;---数据段基地址 0x00cf取00，0x9200取00，0x0000取全部===0x00000000
	DW	0xffff,0x0000,0x9200,0x00cf
        DW      0xffff,0x8000,0xf20b,0x000f
        ;为tss准备的
	DW      0x0000,0x0000,0x0000,0x0000
        ;为idt准备的
	DW      0x0000,0x0000,0x0000,0x0000
        ;DW      0xffff,0x8000,0xf20b,0x000f
GDT0_LEN EQU $-GDT0
GDTR0:
	DW	GDT0_LEN-1
	DD	GDT0

error:
	MOV	SI,msg_error;	打开失败要显示字符
error_loop:
	MOV	AL,[SI]
	ADD	SI,1
	CMP	AL,0
	JE	fin_error
	MOV	AH,0x0e
	MOV	BX,0x0f
	INT	0x10
	JMP	error_loop

;表示最后读取扇区不成功，最后执行到这里
fin_error:
	HLT
	JMP	fin_error

msg:
	DB	0x0a,	0x0a	;换行
	DB	"Welcome to DeeppinkOS:"
	DB	0x0a				;换行
	DB	0
msg_1:
	DB	0x0a,	0x0a	;换行
	DB	"Read Sectors..."
	DB	0x0a				;换行
	DB	0
msg_2:
	DB	"Read Completely!!"
	DB	0x0a				;换行
	DB	0

msg_error:
	DB	"Load	error"
	DB	0x0a				;换行
	DB	0

	times	510-($-$$) db 0
	DW	0xaa55



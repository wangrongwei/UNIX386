;	Deeppinkos
;	TAB=4	SP=0x7c00		0x8000=拷贝到启动区
;-------------------------------------------------------------------------------
;|中断向量表|	|;代码存储位置 |......	|512|
;|	    |	|	       |......  |   |
;-------------------------------------------------------------------------------
;						0x8200=后边的程序
;在某种意义上理解，我们的程序只需要在磁盘的开始512字节就行
;然后硬件上自动去读这512，然后执行这段程序

;1-> 读取磁盘后边扇区的数据
;2-> 在bootsecond.nas中添加LCD支持
;3-> 初始化PIC
;4-> 打开A20，进入保护模式



CYLS		EQU	10	;读10个柱面
BOOT_ADDR	EQU	0x7C00
KERNEL_ADDR	EQU	0xC0000000

LCDMODE		EQU	0x0ff2  ;
SCREENX		EQU	0x0ff4  ;	x
SCREENY		EQU	0x0ff6  ;	y
LCDRAM		EQU	0x0ff8  ; 图像缓冲区的开始地址

	ORG	BOOT_ADDR	;必须是这个地址

;初始化段SS=0 DS=0 ES=0 SP指向程序加载的地址
;因为程序加载的地址是0x7c00，所以我们的段地址必须是0，
;不然地址就不是0x7c00了
	MOV	AX,0
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
	JMP	Read;
;下面开始读磁盘程序数据
;代码借鉴《30天...》川和秀实
;ES:BX	代表缓冲器地址
Read_Ok:
	MOV	AX,0x0820	;原则上来说把启动区也要拷贝过来，就改成0x0800
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
	ADD	SI,1		;SIに1を足す
	CMP	SI,5		;SIと5を比較
	JAE	error		;SI >=	5だったらerrorへ
	MOV	AH,0x00
	MOV	DL,0x00		;Aドライブ
	INT	0x13		;ドライブのリセット
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
	MOV	[0x0ff0],CH	;IPLがどこまで読んだのかをメモ

;
;  打印成功读取状态
;	换显示坐标	
		
	MOV	AH,0x02
	MOV	BX,0x0f
	MOV	DX,0x0e16
	INT	0x10

	MOV	SI,msg_2	;打开成功要显示字符
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
	MOV	AL,0x13
	MOV	AH,0x00
	INT	0x10

	MOV	BYTE [LCDMODE],8
	MOV	WORD [SCREENX],320
	MOV	WORD [SCREENY],200
	MOV	DWORD [LCDRAM],0x000a0000
	
	MOV	AL,0XFF
	OUT	0x21,AL
	NOP
	OUT	0xa1,AL

	;MOV	AH,0x0e
	;MOV	AL,'!'
	;INT	0x10

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


	MOV	AH,0x0e
	MOV	AL,'O'
	INT	0x10

        JMP     0x8200
;
;	显示需要的相关字符串
;

waitkbd_8042:
	IN	AL,0x64
	AND	AL,0x02    ;输入缓冲区是否满了？
	JNZ	waitkbd_8042 ;Yes---跳转
	RET

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



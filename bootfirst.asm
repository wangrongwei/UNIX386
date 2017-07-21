;	wrwos
;	TAB=4								SP=0x7c00														0x8000=拷贝到启动区
;----------------------------------------------------------------------------------------------
;|中断向量表|							|;代码存储位置	|	 ......			|	512|								
;|						|							|								 |	......		 |		|
;----------------------------------------------------------------------------------------------
;																														 0x8200=后边的程序
;	在某种意义上理解，我们的程序只需要在磁盘的开始512字节就行
;	然后硬件上自动去读这512，然后执行这段程序

;	1->	读取磁盘后边扇区的数据 
;	2->	在bootsecond.nas中添加LCD支持
;	3->	初始化PIC
; 4-> 打开A20，进入保护模式

CYLS	EQU	 10				 ;读10个柱面
		ORG		0x7c00		 ;必须是这个地址

		JMP		entry
		DB		0x90
		DB		"lollipop"  ; 必须是8个字符,按照FAT12格式，少于8个在 
		DW		512				  ; 下边使用DB 0xXX填充 
		DB		1				
		DW		1				
		DB		2				
		DW		224			
		DW		2880			
		DB		0xf0			
		DW		9				
		DW		18				
		DW		2			
		DD		0				
		DD		2880			
		DB		0,0,0x29		
		DD		0xffffffff		
		DB		"HARIBOTEOS "	
		DB		"FAT12   "		
		RESB	18					

;	初始化段SS=0 DS=0	ES=0 SP指向程序加载的地址
;	因为程序加载的地址是0x7c00，所以我们的段地址必须是0，
;	不然地址就不是0x7c00了

entry:
		MOV		AX,0		
		MOV		SS,AX
		MOV		SP,0x7c00
		MOV		DS,AX
		MOV		ES,AX

		MOV		SI,msg
putloop:
		MOV		AL,[SI]
		ADD		SI,1			
		CMP		AL,0
		JE		Read_Sector
		MOV		AH,0x0e			
		MOV		BX,0x0f			
		INT		0x10			;执行BIOS中断0x10
		JMP		putloop
		
Read_Sector:			
		MOV		SI,msg_1;	打开成功要显示字符
Read:
		MOV		AL,[SI]
		ADD		SI,1			
		CMP		AL,0
		JE		Read_Ok
		MOV		AH,0x0e			
		MOV		BX,0x0f			
		INT		0x10			;执行BIOS中断0x10
		JMP		Read;	
;	下面开始读磁盘程序数据
;	代码借鉴	《30天...》川和秀实
;	ES:BX	代表缓冲器地址
Read_Ok:
		MOV		AX,0x0820	;原则上来说把启动区也要拷贝过来，就改成0x0800
		MOV		ES,AX
		MOV		BX,0x00
		MOV		CH,0			;	柱面0
		MOV		DH,0			;	磁头0
		MOV		CL,2			;	扇区2
readloop:
		MOV		SI,0	
retry:
		MOV		AH,0x02		;	AH=0x02	:	读扇区
		MOV		AL,1			;	读扇区数 
		MOV		BX,0
		MOV		DL,0x00		;	代表读的软盘
		INT		0x13			;	产生读中断
		JNC		next			;	跳转，代表这个扇区读成功
		ADD		SI,1			;	SIに1を足す
		CMP		SI,5			;	SIと5を比較
		JAE		error			;	SI >=	5	だったらerrorへ
		MOV		AH,0x00
		MOV		DL,0x00			;	Aドライブ
		INT		0x13			;	ドライブのリセット
		JMP		retry
next:
		MOV		AX,ES			;	缓冲地址往后0x200
		ADD		AX,0x0020
		MOV		ES,AX			;	ADD	ES,0x020这个指令没有
		ADD		CL,1			;	读后一个扇区
		CMP		CL,18			;	CL与18比较
		JBE		readloop	;	如果CL <=	18 则跳转
		MOV		CL,1			;||||
		ADD		DH,1			;||||	磁头指向下一个，又开始读下一个第一个扇区
		CMP		DH,2			;||||
		JB		readloop	;	DH < 2 跳转
		MOV		DH,0
		ADD		CH,1			;---------最终改变读到柱面
		CMP		CH,CYLS
		JB		readloop	;	CH < CYLS	跳转
;		10*2*18*512=
		MOV		[0x0ff0],CH		;	IPLがどこまで読んだのかをメモ
		JMP		0xc200
		
error:		 
		MOV		SI,msg_error;	打开成功要显示字符
error_loop:
		MOV		AL,[SI]
		ADD		SI,1			
		CMP		AL,0
		JE		fin_error
		MOV		AH,0x0e			
		MOV		BX,0x0f			
		INT		0x10
		JMP		error_loop	
		
;	表示最后读取扇区不成功，最后执行到这里			
fin_error:
		HLT						
		JMP		fin_error		

msg:
		DB		0x0a,	0x0a	;换行	
		DB		"hello,world"
		DB		0x0a				;换行
		DB		0
msg_1:
		DB		"Read	more Sector..."
		DB		0x0a				;换行
		DB		0
msg_error:
		DB		"Load	error"
		DB		0x0a				;换行
		DB		0

		;times	0x7dfe-$	db	0	
		times	510-($-$$)	db	0
		DW		0xaa55
		
;		DB		0xf0,	0xff,	0xff,	0x00,	0x00,	0x00,	0x00,	0x00
;		RESB	4600
;		DB		0xf0,	0xff,	0xff,	0x00,	0x00,	0x00,	0x00,	0x00
;		RESB	1469432




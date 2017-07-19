;	wrwos
;-----------------------------------------------------------------------------------------------
;                                      BIOS内存映射
;-----------------------------------------------------------------------------------------------
;	TAB=4								SP=0x7c00														0x8000=拷贝到启动区
;	----------------------------------------------------------------------------------------------
;	|中断向量表|						 |;代码存储位置	 |	......		 | 512|								 
;	|						 |						 |								|	 ......			|		 |
;	----------------------------------------------------------------------------------------------
;	   																													 0x8200=后边的程序
;	在某种意义上理解，我们的程序只需要在磁盘的开始512字节就行
;	然后硬件上自动去读这512，然后执行这段程序

;	1->	读取磁盘后边扇区的数据 
;	2->	在bootsecond.nas中添加LCD支持
; 3->	初始化PIC
; 4-> 打开A20，进入保护模式
; 5-> 设置CR0的PE和PG
; 6-> 更新D、E、F、G、S，其中数代表的是第几个GDT
extern	main
GLOBAL	myprintf
BOTPAK	EQU		0x00280000		
DSKCAC	EQU		0x00100000		
DSKCAC0	EQU		0x00008000		

;BOOT_INFO信息
CYLS		 EQU		 0x0ff0			
LEDS		 EQU		 0x0ff1
LCDMODE	 EQU		 0x0ff2		;
SCREENX	 EQU		 0x0ff4		;	x	
SCREENY	 EQU		 0x0ff6		;	y
LCDRAM	 EQU		 0x0ff8		; 图像缓冲区的开始地址

		ORG		0xc200			;	必须是这个地址
	
;		MOV		SI,msg_sys	;	打开成功要显示字符
;		display_loop:
;		MOV		AL,[SI]
;		ADD		SI,1			
;		CMP		AL,0
;		JE		fin
;		MOV		AH,0x0e			
;		MOV		BX,0x0f			
;		INT		0x10
;		JMP		display_loop
	
                    ; /------------------------------ 							
		MOV		AL,0x13		;/-------------------------------
		MOV		AH,0x00		;\  设定VIDEO模式640*480	256色
		INT		0x10			; \------------------------------
	
		MOV		BYTE	[LCDMODE],8
		MOV		WORD	[SCREENX],320
		mov		WORD	[SCREENY],200
		MOV		DWORD	[LCDRAM],0x000a0000	;	保存一下信息
;
; 初始化PIC，然后关闭中断
;	
		MOV		AL,0XFF
		OUT		0x21,AL
		NOP
		OUT		0xa1,AL
		
		CLI
;
; OPEN A20
;
		CALL	waitkbd_8042
		MOV		AL,0xd1      ;写命令
		OUT		0x64,AL
		CALL	waitkbd_8042
		MOV		AL,0xdf
		OUT		0x60,AL
		CALL	waitkbd_8042 ;打开A20
;
; 开始进入保护模式
;
[INSTRSET "i486p"]
		LGDT	[GDTR0]
		
		MOV		EAX,CR0
		AND		EAX,0x7fffffff
		OR		EAX,0x00000001
		MOV		CR0,EAX       ;打开段级保护，不开分页机制
	  JMP		pipelineflush
pipelineflush:
		MOV		AX,8
		MOV		DS,AX
		MOV		ES,AX
		MOV		FS,AX
		MOV		GS,AX
		MOV		SS,AX
		
		; bootpackの転送

		MOV		ESI,bootpack	; 
		MOV		EDI,BOTPAK		; 0x00280000
		MOV		ECX,512*1024/4
		CALL	memcpy

; ついでにディスクデータも本来の位置へ転送

; まずはブートセクタから

;		MOV		ESI,0x7c00		; 転送元
;		MOV		EDI,DSKCAC		; 転送先 0x00100000
;		MOV		ECX,512/4
;		CALL	memcpy

; 残り全部

;		MOV		ESI,DSKCAC0+512	; 転送元 0x00008000
;		MOV		EDI,DSKCAC+512	; 転送先 0x00100000
;		MOV		ECX,0
;		MOV		CL,BYTE [CYLS]
;		IMUL	ECX,512*18*2/4	; シリンダ数からバイト数/4に変換
;		SUB		ECX,512/4		; IPLの分だけ差し引く
;		CALL	memcpy

; asmheadでしなければいけないことは全部し終わったので、
;	あとはbootpackに任せる

; bootpackの起動

		MOV		EBX,BOTPAK     ;0x00280000
		MOV		ECX,[EBX+16]
		ADD		ECX,3			; ECX += 3;
		SHR		ECX,2			; ECX /= 4;
		JZ		skip			
		MOV		ESI,[EBX+20]	
		ADD		ESI,EBX
		MOV		EDI,[EBX+12]	
		CALL	memcpy
skip:
		MOV		ESP,[EBX+12]	; 堆栈初始值
		JMP		DWORD 2*8:0x0000001b
			
;
;8042 检测键盘控制器
;		
waitkbd_8042:
		IN		AL,0x64
		AND		AL,0x02    ;输入缓冲区是否满了？
		JNZ		waitkbd_8042 ;Yes---跳转
		RET
		
myprintf:	; void myprintf(char* msg, int len);
		MOV		ECX,[ESP+4]		;存放的地址
		MOV		EDX,[ESP+8]	  ;存放的数据
		MOV		EBX,1
		MOV		EAX,4
		INT		0x80
		RET		
		
memcpy:
		MOV		EAX,[ESI]
		ADD		ESI,4
		MOV		[EDI],EAX
		ADD		EDI,4
		SUB		ECX,1
		JNZ		memcpy			
		RET
	
		ALIGNB	16
;
;进入保护模式后，不再按照CS*16+IP取指令执行，需要按照向全局描述符
;	具体可参考《linux内核设计的艺术》
;
GDT0:
		RESB	8
		DW		0xffff,0x0000,0x9200,0x00cf		;---段基地址 0x00cf取00，0x9200取00，0x0000取全部====0x00000000
		DW		0xffff,0x0000,0x9a28,0x0047		;---段基地址 0x0047取00，0x9a28取28，0x0000取全部====0x00280000
		DW		0
		
GDTR0:
		DW		23
		DD		GDT0
		
		ALIGNB	16
bootpack:		
		call main		; 调用C函数
;msg_sys:
;		DB		"JMP 0xc200"
;		DB		0x0a				;换行
;		DB		0
		
		
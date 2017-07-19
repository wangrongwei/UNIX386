TOOLPATH = ../z_tools/
INCPATH  = ../z_tools/haribote/

MAKE     = $(TOOLPATH)make.exe -r
NASK     = $(TOOLPATH)nask.exe
CC1      = $(TOOLPATH)cc1.exe -I$(INCPATH) -Os -Wall -quiet
GAS2NASK = $(TOOLPATH)gas2nask.exe -a
OBJ2BIM  = $(TOOLPATH)obj2bim.exe
BIM2HRB  = $(TOOLPATH)bim2hrb.exe
RULEFILE = $(TOOLPATH)haribote/haribote.rul
EDIMG    = $(TOOLPATH)edimg.exe
IMGTOL   = $(TOOLPATH)imgtol.com
COPY     = copy
DEL      = del


default :
	$(MAKE) img


bootfirst.bin : bootfirst.nas Makefile
	$(NASK) bootfirst.nas bootfirst.bin bootfirst.lst

bootsecond.bin : bootsecond.nas Makefile
	$(NASK) bootsecond.nas bootsecond.bin bootsecond.lst

bootpack.gas : bootpack.c Makefile
	$(CC1) -o bootpack.gas bootpack.c

bootpack.nas : bootpack.gas Makefile
	$(GAS2NASK) bootpack.gas bootpack.nas

bootpack.bin : bootpack.nas Makefile
	$(NASK) bootpack.nas bootpack.bin bootpack.lst
	
#naskfunc.obj : naskfunc.nas Makefile
#	$(NASK) naskfunc.nas naskfunc.obj naskfunc.lst

#bootpack.bim : bootpack.obj naskfunc.obj Makefile
#	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
#		bootpack.obj naskfunc.obj
# 3MB+64KB=3136KB

#bootpack.hrb : bootpack.bim Makefile
#	$(BIM2HRB) bootpack.bim bootpack.hrb 0
	
wrwos.sys : bootsecond.bin bootpack.bin Makefile
	copy /B bootsecond.bin+bootpack.bin wrwos.sys
	
wrwos.img : bootfirst.bin wrwos.sys Makefile
	$(EDIMG)   imgin:../z_tools/fdimg0at.tek \
		wbinimg src:bootfirst.bin len:512 from:0 to:0 \
		copy from:wrwos.sys to:@: \
		imgout:wrwos.img


img :
	$(MAKE) wrwos.img

run :
	$(MAKE) img
	$(COPY) wrwos.img ..\z_tools\qemu\fdimage0.bin
	$(MAKE) -C ../z_tools/qemu

install :
	$(MAKE) img
	$(IMGTOL) w a: wrwos.img

clean :
	-$(DEL) *.bin
	-$(DEL) *.lst
	-$(DEL) *.gas
	-$(DEL) *.obj
	-$(DEL) bootpack.nas
	-$(DEL) bootpack.map
	-$(DEL) bootpack.bim
	-$(DEL) bootpack.hrb
	-$(DEL) wrwos.sys

src_only :
	$(MAKE) clean
	-$(DEL) wrwos.img

##################################################
# Makefile
##################################################

BOOT:=boot.asm
LDR:=kernel.asm
BOOT_BIN:=$(subst .asm,.bin,$(BOOT))
LDR_BIN:=$(subst .asm,.bin,$(LDR))


#TOOLPREFIX := /usr/lib/gcc/x86_64-linux-gnu/5.4.0

#CC = $(TOOLPREFIX)gcc
#AS = $(TOOLPREFIX)gas
#LD = $(TOOLPREFIX)ld


IMG:=deeppink.img

#.PHONY : everything

deeppink.img : $(BOOT_BIN) kernel.bin
	#bootsecond.o bootpack.o
	#ld -s -Ttext 0x30400 -o kernel.bin kernel.o string.o start.o kliba.o
	#ld -m elf_i386 -Ttext-seg=0xC0100000 kernel.o start.o -s -o kernel.bin
	#ld -m elf_i386 -Ttext-seg=0x8200 kernel.o -s -o kernel.bin
	dd if=/dev/zero of=$(IMG) bs=512 count=2880
	dd if=$(BOOT_BIN) of=$(IMG) conv=notrunc
	dd if=$(LDR_BIN) of=$(IMG) seek=1 conv=notrunc

$(BOOT_BIN) : $(BOOT)
	nasm $< -o $@

kernel.bin : $(LDR)
	nasm kernel.asm -o kernel.bin
	#nasm -felf $< -o $@
start.o : start.c
	#-f elf nasm -f -elf $(subst .asm,.o,$(LDR))-o  $<
	#nasm -f elf32 bootsecond.asm -o bootsecond.o
	#nasm -f elf -o naskfunc.o naskfunc.asm
	gcc -m32 -c -fno-builtin -fno-stack-protector -o start.o start.c

.PHONY:qemu
qemu:
	@echo '启动虚拟机...'
	qemu-system-i386  -boot order=a -fda deeppink.img 
clean :
	rm -f $(BOOT_BIN) $(LDR_BIN)








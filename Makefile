##################################################
# Makefile
##################################################

BOOT:=bootfirst.asm
LDR:=bootsecond.asm
BOOT_BIN:=$(subst .asm,.bin,$(BOOT))
LDR_BIN:=$(subst .asm,.bin,$(LDR))


#TOOLPREFIX := /usr/lib/gcc/x86_64-linux-gnu/5.4.0

#CC = $(TOOLPREFIX)gcc
#AS = $(TOOLPREFIX)gas
#LD = $(TOOLPREFIX)ld


IMG:=deeppink.img

#.PHONY : everything

deeppink.img : $(BOOT_BIN) bootsecond.o bootpack.o 
	#ld -s -Ttext 0x30400 -o kernel.bin kernel.o string.o start.o kliba.o
	ld -s -Ttext 0xc200 -m elf_i386 -o bootsecond.bin bootsecond.o bootpack.o
	dd if=/dev/zero of=$(IMG) bs=512 count=10000
	dd if=$(BOOT_BIN) of=$(IMG) conv=notrunc
	dd if=$(LDR_BIN) of=$(IMG) seek=1 conv=notrunc

$(BOOT_BIN) : $(BOOT)
	nasm $< -o $@

bootsecond.o : bootsecond.asm 
	nasm -f elf32 bootsecond.asm -o bootsecond.o

bootpack.o : bootpack.c
	#-f elf nasm -f -elf $(subst .asm,.o,$(LDR))-o  $<
	#nasm -f elf32 bootsecond.asm -o bootsecond.o
	#nasm -f elf -o naskfunc.o naskfunc.asm
	gcc -m32 -c -fno-builtin -o bootpack.o bootpack.c

clean :
	rm -f $(BOOT_BIN) $(LDR_BIN)








##################################################
#		 Makefile
##################################################
BOOT:=boot/boot.asm
LDR:=init/kernel.asm
BOOT_BIN:=$(subst .asm,.bin,$(BOOT))
LDR_BIN:=$(subst .asm,.bin,$(LDR))

C_SOURCES = $(shell find ./init -name "*.c")
C_OBJECTS = $(patsubst %.c,%.o,$(C_SOURCES))

S_SOURCES = $(shell find ./init -name "*.asm")
S_OBJECTS = $(patsubst %.asm,%.o,$(S_SOURCES))


CC = gcc
ASM = nasm
LD = ld

C_FLAGS   = -c -Wall -m32 -ggdb -gstabs+ -nostdinc -fno-builtin -fno-stack-protector -I include

LD_FLAGS  = -Ttext-seg=0x8000 -m elf_i386
#LD_FLAGS  = -T scripts/kernel.ld -m elf_i386
ASM_FLAGS = -felf -g -F stabs

IMG:=deeppink.img


deeppink.img : $(BOOT_BIN) $(S_OBJECTS) $(C_OBJECTS) link
	dd if=/dev/zero of=$(IMG) bs=512 count=2880
	dd if=$(BOOT_BIN) of=$(IMG) conv=notrunc
	dd if=$(LDR_BIN) of=$(IMG) seek=1 conv=notrunc

$(BOOT_BIN) : $(BOOT)
	nasm $< -o $@

#kernel.bin : $(LDR)
#	nasm kernel.asm -o kernel.bin
$(C_OBJECTS):$(C_SOURCES)
	$(CC) $(C_FLAGS) $< -o $@
$(S_OBJECTS):$(S_SOURCES)
	@echo $(S_SOURCES)
	$(ASM) $(ASM_FLAGS) $< -o $@ 
link:
	@echo $(S_SOURCES)
	$(LD) $(LD_FLAGS) $(S_OBJECTS) $(C_OBJECTS) -o init/kernel.bin

.PHONY:qemu
qemu:
	@echo '启动虚拟机...'
	qemu-system-i386  -boot order=a -fda deeppink.img 
.PHONY:clean
clean :
	rm -f $(BOOT_BIN) $(LDR_BIN) $(S_OBJECTS) $(C_OBJECTS)

.PHONY:debug
debug:
	qemu-system-i386 -s -S deeppink.img 

	#qemu-system-i386 -s -S -boot order=a -fda deeppink.img 
.PHONY:bochs
bochs:
	bochs
.PHONY:dis
dis:
	ndisasm ./boot/boot.bin > ./boot/boot.txt
	objdump -d ./init/kernel.bin > ./init/kernel.txt





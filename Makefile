##################################################
#		 Makefile
##################################################
BOOT:=boot/boot.asm
KERNEL:=init/kernel.asm
BOOT_BIN:=$(subst .asm,.bin,$(BOOT))
KERNEL_BIN:=$(subst .asm,.bin,$(KERNEL))

# 寻找当前目录下.c文件
C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst %.c,%.o,$(C_SOURCES))

# 寻找init下的汇编文件（待优化）
S_SOURCES = $(shell find ./init -name "*.asm")
S_OBJECTS = $(patsubst %.asm,%.o,$(S_SOURCES))


CC = gcc
ASM = nasm
LD = ld

C_FLAGS   = -c -Wall -m32 -ggdb -gstabs+ -nostdinc -fno-builtin \
-fno-stack-protector -I include

LD_FLAGS  = -T scripts/kernel.ld -m elf_i386
ASM_FLAGS = -felf -g -F stabs

IMG:=deeppink.img


deeppink.img : $(BOOT_BIN) $(S_OBJECTS) $(C_OBJECTS) link
	dd if=/dev/zero of=$(IMG) bs=512 count=28800
	dd if=$(BOOT_BIN) of=$(IMG) conv=notrunc
	dd if=$(KERNEL_BIN) of=$(IMG) seek=1 conv=notrunc

$(BOOT_BIN) : $(BOOT)
	nasm $< -o $@

.c.o:
	@echo $(C_SOURCES)
	$(CC) $(C_FLAGS) $< -o $@

# 如果在init下不止一个.asm文件，此处要更改
$(S_OBJECTS):$(S_SOURCES)
	@echo $(S_SOURCES)
	$(ASM) $(ASM_FLAGS) $< -o $@
# 目前kernel.bin的加载地址是0x8000，其大小不能越过BIOS的地址0x9fc00
# 当前kernel.bin的大小不能超过621568byte
link:
	@echo 链接生成kernel.bin文件
	$(LD) $(LD_FLAGS) $(S_OBJECTS) $(C_OBJECTS) -o $(KERNEL_BIN)

.PHONY:qemu
qemu:
	@echo '启动虚拟机...'
	qemu-system-i386  -boot order=a -fda deeppink.img
.PHONY:clean
clean :
	rm -f $(BOOT_BIN) $(KERNEL_BIN) $(S_OBJECTS) $(C_OBJECTS)

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





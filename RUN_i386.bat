D:/"Program Files"/qemu/qemu-system-i386.exe ^
	-accel tcg,thread=single		^
	-cpu core2duo					^
	-m 64							^
	-boot order=a -fda deeppink.img	^
	-serial stdio					^
	-smp 1							^
	-usb							^
	-vga std

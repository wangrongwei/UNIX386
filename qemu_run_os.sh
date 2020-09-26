#!/usr/bin/env bash

qemu-system-i386 \
	-hdb hd.img \
	-fda os.img \
	-boot a


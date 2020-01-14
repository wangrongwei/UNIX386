
/*
 *	实现POSIX标准中对内核的规定，参考POSIX文档完善！
 *
 *	author:	Rongwei Wang
 *
 *	标准符号常量与类型
 */

#ifndef __UNISTD_H__
#define __UNISTD_H__


/* 参考POSIX-2001标准 */
#define _POSIX_VERSION 200112L
#define _POSIX2_VERSION 200112L
#define _XOPEN_VERSION 600

#define NULL 0

typedef unsigned int uint32_t
typedef int int32_t
typedef unsigned short uint16_t
typedef short int16_t
typedef unsigned char uint8_t
typedef char int8_t

int fork(void);
static int pause(void);

int open(const char* filename,int flag,...);
int close(int fd);


#define container_of(ptr, type, member) ({\
	const typeof((type *)0->member)*__mptr = (ptr);	  \
	(type *)((char *)__mptr - offsetof(type, member)) \
		})

#endif




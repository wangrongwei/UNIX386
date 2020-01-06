
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

static int fork(void);
static int pause(void);

int open(const char* filename,int flag,...);
int close(int fd);

#endif




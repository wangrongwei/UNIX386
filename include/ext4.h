
#ifndef __EXT4_H__
#define __EXT4_H__

/*
 * support VFS
 */

enum filetype {
	TXT = 0,
};

struct node {
	char *name;
	char *path;
	struct node *parent;
	struct node *child;
};

struct file {
	struct node *self;
	
};

void mkdir();
void mkfile();

void mknode();

#endif


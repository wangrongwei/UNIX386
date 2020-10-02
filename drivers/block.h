
#include <unistd.h>
#include <list.h>

struct file_operation {
	int (*write)();
	int (*read)();
	int (*ioctl)();
};

struct device {
	char *name;
	struct file_operation *ops;
	struct list_head lnode;
};

struct dlist {
	struct device *dev;
	struct dlist* prev;
	struct dlist* next;
};

void register_device(struct device *dev);
void init_block_device();
void floppy_init();
void hd_init();


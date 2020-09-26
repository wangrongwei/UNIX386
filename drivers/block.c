
#include <unistd.h>
#include <block.h>

static struct dlist device_list;

/*
 * 初始化块设备
 */
void init_block_device()
{
	device_list.dev = NULL;
	floppy_init();
	hd_init();
}

void register_device(struct device *dev)
{
	int i;
	struct device *devtmp;
	struct dlist *dev_cur = NULL, dev_next = NULL;

	dev_cur = device_list;
	do {
		if (dev_next != NULL) {
			dev_cur = dev_next;
		}
		devtmp = dev_cur->dev;
		dev_next = dev_cur->next;
	}while(devtmp != NULL);

	dev_cur->dev = dev;
}


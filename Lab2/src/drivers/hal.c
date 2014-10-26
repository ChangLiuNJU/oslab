#include "kernel.h"
#include "drivers.h"
#define NR_DEV 64

static Device dev_pool[NR_DEV];
static list_head free, devices;


/**
 *	init_hal - init free queue and device queue
 *	add all device to free queue 
*/
void init_hal(void) {
	int i = 0;
	INIT_LIST_HEAD(&free);
	for (i = 0; i < NR_DEV; i ++) {
		list_add_tail(&dev_pool[i].list, &free);
	}
	INIT_LIST_HEAD(&devices);
}

/**
	hal_register - Delete a dev from free queue.
	Add it to devices queue after 
	@name, @pid, @dev_id are assigned.
*/
void hal_register(const char *name, pid_t pid, int dev_id) {
	lock();
	if (list_empty(&free)) {
		panic("no room for more device");
	}
	Device *dev = list_entry(free.next, Device, list);
	list_del(&dev->list);
	dev->name = name;
	dev->pid = pid;
	dev->dev_id = dev_id;
	list_add_tail(&dev->list, &devices);
	printf("++++++++++++++++++++ tty2.dev_id:%d\n", dev_pool[1].dev_id);
	printf("-------------------- tty2.dev_id:%d\n", hal_get("tty2")->dev_id);
	unlock();
}

/**
	hal_list - Print all devices infomation.
*/
void hal_list(void) {
	lock();
	list_head *it;
	printf("listing all registed devices:\n");
	list_for_each(it, &devices) {
		Device *dev = list_entry(it, Device, list);
		printf("%s #%d, #%d\n", dev->name, dev->pid, dev->dev_id);
	}
	unlock();
}

/**
	hal_get	- Get Device by name.
*/
Device *hal_get(const char *name) {
	lock();
	list_head *it;
	list_for_each(it, &devices) {
		Device *dev = list_entry(it, Device, list);
		if (strcmp(dev->name, name) == 0) {
			printf("DEV_GET:-----%s %s\n", dev->name, name);
			unlock();
			return dev;
		}
	}
	unlock();
	return NULL;
}
/**
	dev_rw - send a write or read message to the 
	target device. 
*/
static size_t
dev_rw(int type, Device *dev, off_t offset, void *buf, size_t count) {
	Message m;
	DevMessage *devm = (DevMessage*)&m;
	assert((sizeof(DevMessage)) <= (sizeof(Message))); // Message结构体不能定义得太小
	

	devm->header.src = current->pid;
	devm->header.dst = dev->pid;
	devm->header.type = type;
	devm->dev_id = dev->dev_id;
	devm->offset = offset;
	devm->buf = buf;
	devm->count = count;
	send(current->pid, dev->pid, &m);
	receive(dev->pid, &m);
	return m.type;
}

size_t
dev_read(Device *dev, off_t offset, void *buf, size_t count) {
	return dev_rw(MSG_DEVRD, dev, offset, buf, count);
}

size_t
dev_write(Device *dev, off_t offset, void *buf, size_t count) {
	return dev_rw(MSG_DEVWR, dev, offset, buf, count);
}


#ifndef __MOD_HAL_H__
#define __MOD_HAL_H__

#include "kernel.h"

#define MSG_HWINTR -1	/* Hardware Interrupte */
#define MSG_DEVRD  1	/* Read bytes from device */
#define MSG_DEVWR  2	/* Write bytes from device */

typedef struct DevMessage {
	MsgHead header; // header与Message的头部定义保持一致即可(src, dst, type)
	union {
		int dev_id;		//for device
		int file_name; 	//for file i/o
	};
	off_t offset;
	void *buf;
	size_t count;
} DevMessage;

typedef struct Device {
	const char *name;
	pid_t pid;
	int dev_id;
	
	list_head list;
} Device;

void init_hal(void);
size_t dev_read(Device *dev, off_t offset, void *buf, size_t count);
size_t dev_write(Device *dev, off_t offset, void *buf, size_t count);

void hal_register(const char *name, pid_t pid, int dev_id);
Device *hal_get(const char *name);
void hal_list(void);

#endif

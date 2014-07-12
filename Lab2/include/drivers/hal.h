#ifndef __MOD_HAL_H__
#define __MOD_HAL_H__

#include "common.h"
#include "lib.h"
#include "process.h"

#define MSG_DEVRD  1
#define MSG_DEVWR  2

typedef struct DevMessage {
	pid_t src, dst;
	union {
		int type;
		int ret;
	};
	int dev_id;
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

size_t dev_read(Device *dev, off_t offset, void *buf, size_t count);
size_t dev_write(Device *dev, off_t offset, void *buf, size_t count);

void hal_register(const char *name, pid_t pid, int dev_id);
Device *hal_get(const char *name);
void hal_list(void);

#endif
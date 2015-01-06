#include "kernel.h"
#include "drivers.h"
#include "x86.h"
#include "test1.h"

const char *ramdiskname = "ramdisk";

uint8_t file[NR_MAX_FILE][NR_FILE_SIZE];
uint8_t *disk = (void*)file;

pid_t RAMDISK;

void init_ramdisk(void) {
	memcpy(file[0], test1, test1_len);
	RAMDISK = create_kthread(ramdiskd)->pid;
}
void ram_read(DevMessage* m) {
	memcpy(m->buf, disk + m->offset, m->count);
}

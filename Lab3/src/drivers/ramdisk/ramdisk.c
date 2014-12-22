#include "kernel.h"
#include "drivers.h"
#include "x86.h"

const char *ramdiskname = "ramdisk";

uint8_t file[NR_MAX_FILE][NR_FILE_SIZE] = {
	{100, 15, 0x56, 0x78},		// the first file '0'
	{"Hello World!\n"},				// the second file '1'
	{0x7f, 0x45, 0x4c, 0x46}	// the third file '2'
};
uint8_t *disk = (void*)file;

pid_t RAMDISK;

void init_ramdisk(void) {
	RAMDISK = create_kthread(ramdiskd)->pid;
}
void ram_read(DevMessage* m) {
	memcpy(m->buf, disk + m->offset, m->count);
}

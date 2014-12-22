#include "drivers.h"
// #include "kernel.h"

void ramdiskd(void) {
	Message m;

	lock();
	hal_register(ramdiskname, RAMDISK, 0);
	unlock();

	while (1) {
		receive(ANY, &m);
		switch (m.type) {
			case MSG_DEV_READ:
				ram_read((DevMessage*)&m);
				break;
		}
	}
}
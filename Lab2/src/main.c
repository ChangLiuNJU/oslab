#include "common.h"
#include "x86.h"
#include "device.h"
#include "process.h"
#include "lib.h"
#include "drivers.h"

void
echo() {
	static int tty = 1;
	char name[] = "tty*", buf[256];
	Device *dev;
	lock();
	name[3] = '0' + (tty ++);
	unlock();
	while (1) {
		dev = hal_get(name);
		if (dev != NULL) {
			dev_write(dev, 0, name, 4);
			dev_write(dev, 0, "# ", 2);
			int i, nread = dev_read(dev, 0, buf, 255);
			buf[nread] = 0;
			for (i = 0; i < nread; i ++) {
				if (buf[i] >= 'a' && buf[i] <= 'z') {
					buf[i] += 'A' - 'a';
				}
			}
			dev_write(dev, 0, "Got: ", 5);
			dev_write(dev, 0, buf, nread);
			dev_write(dev, 0, "\n", 1);
		} else {
			 printf("%s\n", name);
		}
	}
	}

void
test() {
	int i;
	for (i = 0; i < NR_TTY; i ++) {
		create_kthread(echo);
	}
}

void 
entry(void) {
	init_timer();
	init_idt();
	init_intr();
	init_serial();
	init_idle_thread();	
	
	init_hal();
	init_tty();
	test();

	enable_interrupt();
	printf("------Enable Interrupt------\n");

	while (1) {
		wait_for_interrupt();
	}
	assert(0);
}



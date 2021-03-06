#include "kernel.h"
#include "drivers.h"
#include "x86.h"

pid_t TTY;

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
			 // printf("%s\n", name);
		}
	}
}

void init_tty(void) {
	add_irq_handle(1, send_keymsg); // 在irq1时，调用send_keymsg函数
	init_console();
		
	TTY = create_kthread(ttyd)->pid;
}

void init_tty_echo(void) {
	int i;
	for (i = 0; i < NR_TTY; i ++) {
		create_kthread(echo);
	}
}
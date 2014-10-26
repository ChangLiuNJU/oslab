#include "common.h"
#include "x86.h"
#include "device.h"
#include "process.h"
#include "lib.h"
#include "drivers.h"

int pidA, pidB, pidC, pidE, pidD;
void A () { 
	Message m1, m2;
	m1.src = current->pid;
	int x = 0;
	while(1) {
		if(x % 10000000 == 0) {
			printf("a"); 
			send(pidA, pidE, &m1);
			receive(pidE, &m2);
		}
		x ++;
	}
}
void B () { 
	Message m1, m2;
	m1.src = current->pid;
	int x = 0;
	receive(pidE, &m2);
	while(1) {
		if(x % 10000000 == 0) {
			printf("b"); 
			send(pidB,pidE, &m1);
			receive(pidE, &m2);
		}
		x ++;
	}
}
void C () { 
	Message m1, m2;
	m1.src = current->pid;
	int x = 0;
	receive(pidE, &m2);
	while(1) {
		if(x % 10000000 == 0) {
			printf("c"); 
			send(pidC, pidE, &m1);
			receive(pidE, &m2);
		}
		x ++;
	}
}
void D () { 
	Message m1, m2;
	m1.src = current->pid;
	receive(pidE, &m2);
	int x = 0;
	while(1) {
		if(x % 10000000 == 0) {
			printf("d"); 
			send(pidD, pidE, &m1);
			receive(pidE, &m2);
		}
		x ++;
	}
}
 
void E () {
	Message m1, m2;
	m2.src = current->pid;
	char c = ' ';
	while(1) {
		receive(ANY, &m1);
		if(m1.src == pidA) {c = '|'; m2.dst = pidB; }
		else if(m1.src == pidB) {c = '/'; m2.dst = pidC;}
		else if(m1.src == pidC) {c = '-'; m2.dst = pidD;}
		else if(m1.src == pidD) {c = '\\';m2.dst = pidA;}
		else assert(0);
 
		printf("\033[s\033[1000;1000H%c\033[u", c);
		send(pidE, m2.dst, &m2);
	}
 
}

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



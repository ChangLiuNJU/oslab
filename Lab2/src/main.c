#include "common.h"
#include "x86.h"
#include "device.h"
#include "process.h"
#include "lib.h"

pid_t pidA, pidB, pidC, pidD, pidE;


void A () { 
	Message m1, m2;
	m1.src = current->pid;
	int x = 0;
	while(1) {
		if(x % 100000 == 0) {
			printf("a"); 
			send(pidE, &m1);
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
		if(x % 100000 == 0) {
			printf("b"); 
			send(pidE, &m1);
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
		if(x % 100000 == 0) {
			printf("c"); 
			send(pidE, &m1);
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
		if(x % 100000 == 0) {
			printf("d"); 
			send(pidE, &m1);
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
		send(m2.dst, &m2);
	}
 
}

void
entry(void) {
	init_timer();
	init_idt();
	init_intr();
	init_serial();
	init_idle_thread();	
	
	// init_hal();
	// init_tty();

	enable_interrupt();

	// test_setup();
	pidA = create_kthread(A)->pid;
	pidB = create_kthread(B)->pid;
	pidC = create_kthread(C)->pid;
	pidD = create_kthread(D)->pid;
	pidE = create_kthread(E)->pid;

	// create_kthread(a);
	// create_kthread(b);
	// create_kthread(c);
	// create_kthread(d);

	while (1) {
		wait_for_interrupt();
	}
	assert(0);
}



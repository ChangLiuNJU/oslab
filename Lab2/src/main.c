#include "common.h"
#include "x86.h"
#include "device.h"
#include "process.h"
#include "lib.h"

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
	// pidA = create_kthread(A)->pid;
	// pidB = create_kthread(B)->pid;
	// pidC = create_kthread(C)->pid;
	// pidD = create_kthread(D)->pid;
	// pidE = create_kthread(E)->pid;

	while (1) {
		wait_for_interrupt();
	}
	assert(0);
}



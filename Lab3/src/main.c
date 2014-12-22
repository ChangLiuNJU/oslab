#include "common.h"
#include "x86.h"
#include "device.h"
#include "process.h"
#include "lib.h"
#include "drivers.h"

void 
entry(void) {
	
	init_page();
	init_segment();

	init_timer();
	init_idt();
	init_intr();
	init_serial();
	init_idle_thread();	
	
	init_hal();
	init_tty();
	init_tty_echo();

	enable_interrupt();

	while (1) {
		wait_for_interrupt();
	}
	assert(0);
}



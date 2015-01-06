#include "common.h"
#include "x86.h"
#include "device.h"
#include "process.h"
#include "lib.h"
#include "drivers.h"
#include "file.h"

// uint8_t buf[10];

void 
entry(void) {
	
	init_page();
	init_segment();

	init_timer();
	init_idt();
	init_intr();			//Initial interrupt
	init_serial();
	init_idle_thread();	
	
	init_hal();
	init_tty();
	init_tty_echo();

	init_ramdisk();
	init_file();

	// do_read(0, buf, 0, 1);
	enable_interrupt();

	while (1) {
		wait_for_interrupt();
		// printf("%d\n", buf[0]);
	}
	assert(0);
}



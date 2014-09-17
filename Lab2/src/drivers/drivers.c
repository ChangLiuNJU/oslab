#include "drivers.h"
#include "device.h"

void init_hal();
void init_tty();
void test();

void init_driver() {
	init_hal();
	init_timer();
	init_tty();
	//

	test();
}
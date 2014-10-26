#include "kernel.h"
#include "drivers.h"
#include "x86.h"

pid_t TTY;

void init_tty(void) {
	add_irq_handle(1, send_keymsg); // 在irq1时，调用send_keymsg函数
	init_console();
		
	TTY = create_kthread(ttyd)->pid;
}


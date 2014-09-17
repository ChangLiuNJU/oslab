#include "x86.h"
#include "device.h"
#include "process.h"
#include "lib.h"

#define NR_IRQ_HANDLE 32
#define NR_HARD_INTR 16

typedef struct IRQ_t {
	void (*routine)(void);
	struct IRQ_t *next;
} IRQ_t;

static IRQ_t handle_pool[NR_IRQ_HANDLE];
static IRQ_t *handles[NR_HARD_INTR];
static int handle_count = 0;
int need_sched;

void 
add_irq_handle(int irq, void (*func)(void)) {
	IRQ_t *ptr;
	assert(irq < NR_HARD_INTR);
	if (handle_count > NR_IRQ_HANDLE) {
		abort("Too many irq registrations!", -1);
	}
	ptr = &handle_pool[handle_count ++];
	ptr->routine = func;
	ptr->next = handles[irq];
	handles[irq] = ptr;
}

void
irq_handle(struct TrapFrame *tf) {
	
	current->tf = tf;	//save the current trap frame 

	need_sched = FALSE;

	if (tf->irq == 1000) {
		int irq_id = tf->irq - 1000;
		assert(irq_id < NR_HARD_INTR);
		IRQ_t *f = handles[irq_id];

		while (f != NULL) {
			f->routine();
			f = f->next;
		}
		// printf("!\n");
	} else if (tf->irq == 1001) {
		// assert(0);
		uint32_t code = in_byte(0x60);
		uint32_t val = in_byte(0x61);
		out_byte(0x61, val | 0x80);
		out_byte(0x61, val);
		putchar('0' + code / 100);
		putchar('0' + code / 10 % 10);
		putchar('0' + code % 10);
		putchar('\n');
	} else if (tf->irq == 0x80) {
		need_sched = TRUE;
		// schedule();	
	}
	else {
		printf("%d\n", tf->irq);
		assert(0);
	}
	if (need_sched == TRUE) {
		schedule();
	}
}


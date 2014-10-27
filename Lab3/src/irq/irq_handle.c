#include "x86.h"
#include "device.h"
#include "process.h"
#include "lib.h"
#include "drivers.h"

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

	if (tf->irq >= 1000) {
		int irq_id = tf->irq - 1000;
		assert(irq_id < NR_HARD_INTR);
		IRQ_t *f = handles[irq_id];
		while (f != NULL) {
			f->routine();
			f = f->next;
		}
	} else if (tf->irq == 0x80) {
		need_sched = TRUE;
	}
	else {
		printf("%d\n", tf->irq);
		assert(0);
	}
	if (need_sched == TRUE) {
		schedule();
	}
}


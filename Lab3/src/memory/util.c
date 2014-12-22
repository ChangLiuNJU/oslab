#include "x86/memory.h"

void
make_invalid_pde(PDE *p) {
	p->val = 0;
}
void
make_invalid_pte(PTE *p) {
	p->val = 0;
}

void
make_pde(PDE *p, void *addr) {
	p->val = 0;
	p->page_frame = ((uint32_t)addr) >> 12;
	p->present = 1;
	p->read_write = 1;
	p->user_supervisor = 1;
}

void
make_pte(PTE *p, void *addr) {
	p->val = 0;
	p->page_frame = ((uint32_t)addr) >> 12;
	p->present = 1;
	p->read_write = 1;
	p->user_supervisor = 1;
}
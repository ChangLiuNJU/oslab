#include "x86.h"
#include "lib.h"
#include "kernel.h"

static bool page_bitmap[NR_PAGES];

void mm(void) {
	memset(page_bitmap, 0, sizeof(page_bitmap));
	Message m;
	while (1) {
		receive(ANY, &m);
		switch(m.type) {
			case MSG_REQ_PAGE:
					
				break;
		}
	}
}
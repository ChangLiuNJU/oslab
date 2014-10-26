#include "process.h"
#include "lib.h"

void 
schedule(void) {
	// printf("schedule start\n");
	// printf("%d %d %d %d\n", PCBs[2].state, PCBs[3].state, PCBs[4].state, PCBs[5].state );
	if (!list_empty(&readyq_h) && (current->state == READY)) {
		struct list_head *tmp;
		tmp = current->state_list.next;
		if (tmp == &readyq_h) {
			tmp = tmp->next;
		}
		current = list_entry(tmp, struct PCB, state_list);
	}
	else {
		current = &PCBs[0];
	}
	// printf("schedule end\n");
}
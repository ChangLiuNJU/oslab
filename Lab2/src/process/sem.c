#include "lib.h"
#include "process.h"

void
init_sem(Semaphore *sem, int value) {
	sem->count = value;
	INIT_LIST_HEAD(&sem->queue);
}

void
P(Semaphore *sem) {
	lock();
	sem->count --;
	if (sem->count < 0) {
		list_add_tail(&current->semq, &sem->queue);
		sleep(); // sleep!
	}
	unlock();
}

void
V(Semaphore *sem) {
	lock();
	sem->count ++;
	if (sem->count <= 0) {
		assert(!list_empty(&sem->queue));
		PCB *p = list_entry(sem->queue.next, PCB, semq);
		list_del(sem->queue.next);
		wakeup(p);
	}
	unlock();
}

void 
send(pid_t src, pid_t dst, Message *m) {
	// printf("send start\n");
	P(&(PCBs[dst].mutex));
	V(&(PCBs[dst].msgsemANY));
	if (src >= 0) {
		V(&(PCBs[dst].msgsem[src]));
	}
	PCB *pcb = &PCBs[dst];	//get PCB by id

	int i = 0;
	for (i = 0; i < NR_MSGS; i++) {
		if (pcb->msgq[i].src == INVALID_MSG_SRC) {
			break;
		}
	}
	assert(i != NR_MSGS);
	pcb->msgq[i] = *m;
	pcb->msgq[i].src = src;
	pcb->msgq[i].dst = dst;

	V(&PCBs[dst].mutex);
	// printf("send end\n");
}

void 
receive(pid_t src, Message *m) {
	// printf("receive start\n");
	if (src == ANY) {
		P(&(current->msgsemANY));
		P(&(current->mutex));

		int i = 0;
		for (i = 0; i < NR_MSGS; i++) {
			if (current->msgq[i].src != INVALID_MSG_SRC) {
				break;
			}
		}		
		assert(i != NR_MSGS);
		*m = current->msgq[i];
		if (src >= 0) {
			P(&(current->msgsem[m->src]));
		}
		current->msgq[i].src = INVALID_MSG_SRC;
		V(&(current->mutex));
		// printf("receive end1\n");
		return;
	} else {
		P(&(current->msgsem[src]));
		P(&(current->msgsemANY));
		P(&(current->mutex));

		int i = 0;
		for (i = 0; i < NR_MSGS; i++) {
			if (current->msgq[i].src == src) {
				break;
			}
		}
		assert(i != NR_MSGS);
		*m = current->msgq[i];
		current->msgq[i].src = INVALID_MSG_SRC;
		V(&(current->mutex));
		// printf("receive end2\n");
		return;
	}
	assert(0);
}
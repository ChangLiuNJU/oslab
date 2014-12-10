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
	P(&(PCBs[dst].mutex));
	V(&(PCBs[dst].msgsemANY));
	V(&(PCBs[dst].msgsem[src]));

	PCB *pcb = &PCBs[dst];	//get PCB by id

	//get a empty msg index in msgq
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
}

void 
receive(pid_t src, Message *m) {
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
		return;
	}
	assert(0);
}
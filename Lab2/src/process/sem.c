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
		// list_add(&sem->queue, &current->semq);
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
send(pid_t dst, Message *m) {
	lock();
	PCB *pcb = &PCBs[dst];	//get PCB by id
	m->dst = dst;
	list_add(&m->list, &pcb->msgq);
	if (pcb->state == SLEEP) {
		wakeup(pcb);
	}
	unlock();
}

void 
receive(pid_t src, Message *m) {
	lock();
	if (list_empty(&current->msgq)) {	//sleep if the message queue is empty.
		unlock();
		sleep();
	}
	list_head 	*pos;
	Message   	*tmp;

	list_for_each(pos, &(current->msgq)) {
		tmp = list_entry(pos, Message, list);
		if (src == ANY || tmp->src == src) {
			*m = *tmp;
			list_del(pos);
			unlock();
			asm volatile("int $0x80");
			return;
		}
	}
	unlock();
	sleep();		//Message unfounded.

}
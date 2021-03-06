#include "process.h"

PCB PCBs[NR_PCBS];
PCB *current;
LIST_HEAD(readyq_h);
LIST_HEAD(freeq_h);
LIST_HEAD(blockq_h);
uint16_t pcbs_avl = 0;	//pcbs available index

void 
init_idle_thread() {
	PCB *pcb = &(PCBs[0]);
	current = pcb;
	current->pid = 0;
	current->lock_depth = 0;
	current->state = READY;

	current->mutex.count = 1;
	INIT_LIST_HEAD(&(current->mutex.queue));
	current->msgsemANY.count = 0;
	INIT_LIST_HEAD(&(current->msgsemANY.queue));
	int i = 0;
	for (i = 0; i < NR_PCBS; i++) {
		current->msgsem[i].count = 0;
		INIT_LIST_HEAD(&(current->msgsem[i].queue));
	}
	for (i = 0; i < NR_MSGS; i++) {
		current->msgq[i].src = INVALID_MSG_SRC;
	}
	list_add(&(pcb->state_list), &readyq_h);
	pcbs_avl = 1;
}

PCB *
create_kthread(void (*entry)(void)) {
	
	//allocate a pcb
	assert(pcbs_avl < NR_PCBS);	
	PCB *pcb  = &(PCBs[pcbs_avl]);
	pcb->pid 		 = pcbs_avl;	//allocate pid
	pcbs_avl ++;
	pcb->lock_depth  = 0;			//set lock depth
	
	INIT_LIST_HEAD(&(pcb->semq));
	pcb->mutex.count = 1;
	INIT_LIST_HEAD(&(pcb->mutex.queue));
	pcb->msgsemANY.count = 0;
	INIT_LIST_HEAD(&(pcb->msgsemANY.queue));
	int i = 0;
	for (i = 0; i < NR_PCBS; i++) {
		pcb->msgsem[i].count = 0;
		INIT_LIST_HEAD(&(pcb->msgsem[i].queue));
	}
	for (i = 0; i < NR_MSGS; i++) {
		pcb->msgq[i].src = INVALID_MSG_SRC;
	}

	//set the trapframe
	pcb->tf 		 = (struct TrapFrame*)(pcb->kstack + KSTACK_SIZE) - 1; 	

	pcb->tf->edi = 0;
	pcb->tf->esi = 0;
	pcb->tf->ebp = 0;
	pcb->tf->xxx = 0;
	pcb->tf->ebx = 0;
	pcb->tf->edx = 0;
	pcb->tf->ecx = 0;
	pcb->tf->eax = 0;
	pcb->tf->irq = 0;

	// pcb->tf->err = 0;
	pcb->tf->eip 	= (uint32_t)entry;
	pcb->tf->cs 	= 1 << 3;
	pcb->tf->eflags = 0x00000202;

	//add to ready queue
	pcb->state = READY;
	list_add(&(pcb->state_list), &readyq_h);
	return pcb;
}
void
lock() {
	current->lock_depth++;
	asm volatile("cli");
}
void
unlock() {
	current->lock_depth--;
	if (current->lock_depth == 0) {
		asm volatile("sti");
	}
}
void
sleep() {
	lock();
	if (current->state == READY) {
		current->state = SLEEP;
		list_del(&current->state_list);
		list_add(&current->state_list, &blockq_h); 
	}
	unlock();
	asm volatile("int $0x80");
}
void 
wakeup(struct PCB* pcb) {
	lock();
	if (pcb->state == SLEEP) {
		pcb->state = READY;
		list_del(&pcb->state_list);
		list_add(&pcb->state_list, &readyq_h);
	}
	unlock();
}
#include "process.h"

PCB*
create_uthread() {
	assert(pcbs_avl < NR_PCBS);
	PCB *pcb = &(PCBs[pcbs_avl]);
	pcb->pid = pcbs_avl;
	pcbs_avl ++;
	pcb->lock_depth = 0;

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
	pcb->tf = (struct TrapFrame*)(pcb->kstack + KSTACK_SIZE) - 1; 	

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
	// pcb->tf->eip 	= (uint32_t)entry;
	pcb->tf->cs 	= 1 << 3;
	pcb->tf->eflags = 0x00000202;

	//add to ready queue
	pcb->state = READY;
	list_add(&(pcb->state_list), &readyq_h);
	return pcb;
}

void pm(void) {
	// PCB* p = create_uthread()

	// do_read(0, buf, 0 , 512);

	
}
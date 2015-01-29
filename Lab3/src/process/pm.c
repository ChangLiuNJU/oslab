#include "process.h"
#include "file.h"
#include "kernel.h"

pid_t PM;

PCB*
create_uthread(int file_name) {
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

	//load elf header from file[file_name]
	uint8_t buf[52];
	do_read(file_name, buf, 0, 52);
	struct ELFHeader *elf;
	elf = (struct ELFHeader*)buf;
	printf("%x\n", elf->entry);
	
	pcb->tf->eip = elf->entry;
	/* Load each program segment */
	struct ProgramHeader *ph, *eph;
	ph = (struct ProgramHeader*)((char *)elf + elf->phoff);
	eph = ph + elf->phnum;

	for (; ph < eph; ph ++) {
		//mm
	} 

	pcb->tf->cs 	= 1 << 3;
	pcb->tf->eflags = 0x00000202;

	//add to ready queue
	pcb->state = READY;
	// list_add(&(pcb->state_list), &readyq_h);
	return pcb;
}

void pm(void) {
	create_uthread(0);
	while (1) {
		;
	}
}
void init_pm(void) {
	PM = create_kthread(pm)->pid;
}
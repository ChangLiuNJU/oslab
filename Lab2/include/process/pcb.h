#ifndef __PCB_H__
#define __PCB_H__

#include "x86.h"
#include "lib.h"

#define KSTACK_SIZE 4096	//kernel stack size
#define NR_PCBS 	512		//max pcb numbers
#define SLEEP		1
#define READY    	0

/* PCB struct */
typedef struct PCB {
	struct TrapFrame 	*tf;
	pid_t	 			pid;
	volatile long		state;
	uint32_t 			lock_depth;
	list_head 			state_list;				//state list
	list_head 			semq;					//Semaphore queue for PV
	list_head			msgq;					//message queue
	uint8_t 			kstack[KSTACK_SIZE];
} PCB;

extern PCB 			PCBs[NR_PCBS];
extern PCB 			*current;		//current pcb
extern list_head	readyq_h, blockq_h;
extern uint16_t 	pcbs_avl; 	//pcbs available index

void init_idle_thread(void);
PCB *create_kthread(void (*entry)(void));
void sleep(void);
void wakeup(struct PCB *t);
void lock(void);
void unlock(void);

#endif

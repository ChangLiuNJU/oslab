#ifndef __SEM_H__
#define __SEM_H__
#define ANY -1
#define MSG_SZ 512

#include "lib.h"
#include "common.h"

typedef struct Semaphore {
	int count;
	list_head queue;
} Semaphore;

typedef struct Message {
	pid_t src, dst;
	union {
		int type;
		int ret;
	};
	union {
		int i[5];
		struct 
		{
			pid_t 	req_pid;
			int 	dev_id;
			void 	*buf;
			int 	offset;
			size_t 	len;
		};
	};
	list_head list;
} Message;

void init_sem(Semaphore *sem, int value);
void P(Semaphore *sem);
void V(Semaphore *sem);
void send(pid_t dst, Message *m);
void receive(pid_t src, Message *m);

#endif
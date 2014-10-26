#ifndef __SEM_H__
#define __SEM_H__
#define ANY -1
#define MSG_SZ 512
#define INVALID_MSG_SRC -2 

#include "lib.h"
#include "common.h"

typedef struct Semaphore {
	int count;
	list_head queue;
} Semaphore;

typedef struct MsgHead {
	pid_t src, dst;
	union {
		int type;
		int ret;
	};
} MsgHead;

typedef struct Message {
	pid_t src, dst;
	union {
		int type;
		int ret;
	};
	char payload[20];
} Message;

void init_sem(Semaphore *sem, int value);
void P(Semaphore *sem);
void V(Semaphore *sem);
void send(pid_t src, pid_t dst, Message *m);
void receive(pid_t src, Message *m);

#endif
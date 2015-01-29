#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "process/pcb.h"
#include "process/schedule.h"
#include "process/sem.h"

extern pid_t PM;
void init_pm(void);

#endif
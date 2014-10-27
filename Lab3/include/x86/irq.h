#ifndef __IRQ_H__
#define __IRQ_H__

extern int need_sched; //need schedule

/* 中断处理相关函数 */
void init_idt(void);
void init_intr(void);
void add_irq_handle(int irq, void (*func)(void));

#endif

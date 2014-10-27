#ifndef __TIMER_H__
#define __TIMER_H__

#define PORT_TIME 0x40
#define PORT_RTC  0x70	//Real-time Clock
#define FREQ_8253 1193182

#define HZ 100

struct Time {
	int year, month, day;
	int hour, minute, second;
};
typedef struct Time Time;
extern long jiffy;

void get_time(Time *tm);
void init_timer(void);

void update_sched(void);
void update_jiffy(void);

#endif

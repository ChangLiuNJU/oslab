#ifndef __FILE_H__
#define __FILE_H__

#include "kernel.h"

#define MSG_DO_READ 4

extern pid_t FM;

void do_read(int file_name, uint8_t *buf, off_t offset, size_t len);
void init_file(void);
void fm(void);	//File Management

#endif
#include "file.h"
#include "drivers.h"

pid_t FILE;

void do_read(int file_name, uint8_t *buf, off_t offset, size_t len) {
	Message m;
	DevMessage *msg;
	msg = (DevMessage*)&m;
	msg->file_name = file_name;
	msg->offset = offset;
	msg->buf = buf; 
	msg->count = len;
	m.type = MSG_DO_READ;
	send(current->pid, FILE, &m);
}

void init_file(void) {
	FILE = create_kthread(fm)->pid;
}

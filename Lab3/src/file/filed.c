#include "drivers.h"
#include "file.h"

void filed(void) {
	Message m;
	while (1) {
		receive(ANY, &m);
		switch (m.type) {
			DevMessage *msg;
			case MSG_DO_READ:
				msg = (DevMessage*)&m;
				msg->header.type = MSG_DEV_READ;
				msg->offset += msg->file_name * NR_FILE_SIZE;
				send(current->pid, RAMDISK, &m);
				break;
		} 
	} 
}

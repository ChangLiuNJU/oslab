/*	File Manager 
 *	
 *
*/
#include "drivers.h"
#include "file.h"

void fm(void) {
	Message m;
	while (1) {
		receive(ANY, &m);
		switch (m.type) {
			DevMessage *msg;
			/* When receive an MSG_DO_READ request
			   , forward it as an MSG_DEV_READ message
			   to RAMDISK */
			case MSG_DO_READ:
				msg = (DevMessage*)&m;
				msg->header.type = MSG_DEV_READ;
				msg->offset += msg->file_name * NR_FILE_SIZE;
				send(current->pid, RAMDISK, &m);
				Message tmp;
				receive(RAMDISK, &tmp);
				send(current->pid, m.src, &tmp);
				break;
			/* When receive an MSG_DEVRD_DONE from RAMDISK
			   forward it to it's source */
			// case MSG_DEVRD_DONE:
			// 	msg = (DevMessage*)&m;
			// 	m.type = MSG_READ_DONE;
			// 	send(current->pid, m.src, &m);
		} 
	} 
}

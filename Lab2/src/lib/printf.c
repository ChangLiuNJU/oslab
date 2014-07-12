#include "device.h"
#define POINTER_SIZE sizeof(void *)

void printf(const char *format, ...) {
	int offset = 0;
	void *arg  = (void *)&format + POINTER_SIZE;	// Charles
													// Get the adress of the first argument. 
													// CAUTION: &format is a char ** variable."&format + 4" 
													// actually gets the 16th bytes after format's address without (void *).
	while (*(format + offset) != '\0') {
		if (*(format + offset) == '%') {
			offset++;
			switch (*(format + offset)) {
				case 'c': {
					char *tmp_arg = (char *)arg;
					putchar(*tmp_arg);
					arg += POINTER_SIZE;
					break;
				}
				case 'x': {
					unsigned int *tmp_arg = (unsigned int *)arg;	//The unsigned type solve the negative case.
					char buf[30];
					char *p = buf + sizeof(buf) - 1;
					*p = '\0';
					do {
						*--p = (*tmp_arg % 16 < 10) ? ('0' + *tmp_arg % 16) : ('a' + *tmp_arg % 16 - 10);
					} while (*tmp_arg /= 16);
					while (*p != '\0') {
						putchar(*p);
						p++;
					}
					arg += POINTER_SIZE;
					break;
				}
				case 'd': {
					int *tmp_arg = (int *)arg; 
					if (*tmp_arg < 0) {		//Solve the negative case.
						putchar('-');
						*tmp_arg = 0 - *tmp_arg;
					}
					char buf[30];
					char *p = buf + sizeof(buf) - 1;
					*p = '\0';
					do {
						*--p = '0' + *tmp_arg % 10;
					} while (*tmp_arg /= 10);
					while (*p != '\0') {
						putchar(*p);
						p++; 
					}
					arg += POINTER_SIZE;
					break;
				}
				case 's': {
					char **tmp_arg = (char**)arg;
					while (**tmp_arg != '\0') {
						putchar(**tmp_arg);
						(*tmp_arg) ++;
					}
					arg += POINTER_SIZE;
					break;
				}
				case '%': {
					putchar('%');
					break;
				}
			}
		}
		else {
			putchar(*(format + offset));
		}
		offset++;	
	}
}

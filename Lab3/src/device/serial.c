#include "x86.h"
#include "device.h"

void
init_serial(void) {
	out_byte(SERIAL_PORT + 1, 0x00);	//Disable all interrupts
	out_byte(SERIAL_PORT + 3, 0x80);	//Enable DALB (set baud rate divisor)
	out_byte(SERIAL_PORT + 0, 0x01);	//Set divisor to 3 (lo byte) 38400 baud
	out_byte(SERIAL_PORT + 1, 0x00);	//				(hi byte)
	out_byte(SERIAL_PORT + 3, 0x03);	//8 bits, no parity one stop bit
	out_byte(SERIAL_PORT + 2, 0xC7);	//Enable FIFO, clear them, with 14-byte threshold
	out_byte(SERIAL_PORT + 4, 0x0B);	//IRQS enabled, RTS/DSR set
}

static inline int
serial_idle(void) {
	return (in_byte(SERIAL_PORT + 5) & 0x20) != 0;
}

void
putchar(char ch) {
	while (serial_idle() != TRUE);
	out_byte(SERIAL_PORT, ch);
}

//Link: http://wiki.osdev.org/Serial_Ports


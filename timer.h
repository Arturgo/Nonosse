#ifndef TIMER_H
#define TIMER_H

#include "terminal.h"
#include "cpu_asm.h"

extern int timer_irq();

// TIMER_DIVIDER est le diviseur de la fréquence de base du PIC
const unsigned short TIMER_DIVIDER = (1 << 15);

void setup_timer() {
	port_out(0x43, 0x36);
	
	port_out(0x40, TIMER_DIVIDER & 0xFF);
	port_out(0x40, (TIMER_DIVIDER >> 8) & 0xFF);
}

void timer_handler(void) {
	port_out(0x20, 0x20);
	
	// on nous a rendu la main pour faire du réordonnancement
}

#endif

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "cpu_asm.h"
#include "terminal.h"

extern int keyboard_irq();

void setup_keyboard() {
	// eventuelle configuration appelee au demarrage de l'OS
}

void keyboard_handler(void) {
	port_out(0x20, 0x20);

	unsigned char event_code = port_in(0x60);
	terminal_writestring("IT WORKS !!!!\n");
	
	// l'octet event_code contient le code de la touche appuyée
}

#endif

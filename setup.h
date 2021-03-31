#ifndef SETUP_H
#define SETUP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "terminal.h"
#include "keyboard.h"
#include "timer.h"

void setup() {
	setup_terminal();
	setup_keyboard();
	setup_timer();	
}

#endif

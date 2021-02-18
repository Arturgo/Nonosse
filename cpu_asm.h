#ifndef PORTS_H
#define PORTS_H

void port_out(unsigned short port, unsigned char data);
unsigned char port_in(unsigned short port);
void load_idt(void* idt);

extern int irq0();
extern int irq1();
extern int irq2();
extern int irq3();
extern int irq4();
extern int irq5();
extern int irq6();
extern int irq7();
extern int irq8();
extern int irq9();
extern int irq10();
extern int irq11();
extern int irq12();
extern int irq13();
extern int irq14();
extern int irq15();

void irq0_handler(void) {
	port_out(0x20, 0x20);
}

void irq2_handler(void) {
	port_out(0x20, 0x20);
}

void irq3_handler(void) {
	port_out(0x20, 0x20);
}

void irq4_handler(void) {
	port_out(0x20, 0x20);
}

void irq5_handler(void) {
	port_out(0x20, 0x20);
}

void irq6_handler(void) {
	port_out(0x20, 0x20);
}

void irq7_handler(void) {
	port_out(0x20, 0x20);
}

void irq8_handler(void) {
	port_out(0xA0, 0x20);
	port_out(0x20, 0x20);          
}

void irq9_handler(void) {
	port_out(0xA0, 0x20);
	port_out(0x20, 0x20);
}

void irq10_handler(void) {
	port_out(0xA0, 0x20);
	port_out(0x20, 0x20);
}

void irq11_handler(void) {
	port_out(0xA0, 0x20);
	port_out(0x20, 0x20);
}

void irq12_handler(void) {
	port_out(0xA0, 0x20);
	port_out(0x20, 0x20);
}

void irq13_handler(void) {
	port_out(0xA0, 0x20);
	port_out(0x20, 0x20);
}

void irq14_handler(void) {
	port_out(0xA0, 0x20);
	port_out(0x20, 0x20);
}

void irq15_handler(void) {
	port_out(0xA0, 0x20);
	port_out(0x20, 0x20);
}

#endif

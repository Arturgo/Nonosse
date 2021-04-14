#include "setup.h"
#include "multiboot.h"

typedef struct {
	unsigned short offset0_15;
	unsigned short select;
	unsigned char zero;
	unsigned char type;
	unsigned short offset16_31;
} __attribute__ ((packed)) idt_entry;

typedef struct {
	unsigned short limit;
	unsigned int base;
} __attribute__((packed)) idt_ptr_t;

idt_ptr_t idt_ptr;
idt_entry idt[256];

void edit_idt_entry(unsigned short select, unsigned int offset, unsigned char type, idt_entry *desc) {
	desc->offset0_15 = (offset & 0xffff);
	desc->select = select;
	desc->zero = 0;
	desc->type = type;
	desc->offset16_31 = (offset >> 16) & 0xffff;
}

void map_interrupts() {
	// https://en.wikibooks.org/wiki/X86_Assembly/Programmable_Interrupt_Controller
	//program PIC, default is 8 for PIC1	
	port_out(0x20, 0x11);
	port_out(0xA0, 0x11);
	
	port_out(0x21, 0x20);
	port_out(0xA1, 0x28);
	
	port_out(0x21, 0x04);
	port_out(0xA1, 0x02);
	
	port_out(0x21, 0x01);
	port_out(0xA1, 0x01);
	
	port_out(0x21, 0x0);
	port_out(0xA1, 0x0);
	
	edit_idt_entry(0x08, (unsigned int)timer_irq, 0x8E, &idt[32]);
	edit_idt_entry(0x08, (unsigned int)keyboard_irq, 0x8E, &idt[33]);
	edit_idt_entry(0x08, (unsigned int)irq2, 0x8E, &idt[34]);
	edit_idt_entry(0x08, (unsigned int)irq3, 0x8E, &idt[35]);
	edit_idt_entry(0x08, (unsigned int)irq4, 0x8E, &idt[36]);
	edit_idt_entry(0x08, (unsigned int)irq5, 0x8E, &idt[37]);
	edit_idt_entry(0x08, (unsigned int)irq6, 0x8E, &idt[38]);
	edit_idt_entry(0x08, (unsigned int)irq7, 0x8E, &idt[39]);
	
	edit_idt_entry(0x08, (unsigned int)irq8, 0x8E, &idt[40]);
	edit_idt_entry(0x08, (unsigned int)irq9, 0x8E, &idt[41]);
	edit_idt_entry(0x08, (unsigned int)irq10, 0x8E, &idt[42]);
	edit_idt_entry(0x08, (unsigned int)irq11, 0x8E, &idt[43]);
	edit_idt_entry(0x08, (unsigned int)irq12, 0x8E, &idt[44]);
	edit_idt_entry(0x08, (unsigned int)irq13, 0x8E, &idt[45]);
	edit_idt_entry(0x08, (unsigned int)irq14, 0x8E, &idt[46]);
	edit_idt_entry(0x08, (unsigned int)irq15, 0x8E, &idt[47]);
	
	idt_ptr.limit = sizeof(idt_entry) * 256 - 1;
	idt_ptr.base = (unsigned int)&idt;
	
	load_idt(&idt_ptr);
}

void kernel_main() {
	setup();
	
	map_interrupts();
 
	terminal_writestring("Salut les amis !\nComment ca va ?");
	
	terminal_move_cursor(16);
}


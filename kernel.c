#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "cpu_asm.h"
 
/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};
 
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}
 
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}
 
size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}
 
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
 
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;
 
void terminal_initialize(void) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}
 
void terminal_setcolor(uint8_t color) 
{
	terminal_color = color;
}
 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}
 
void terminal_putchar(char c) 
{
	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
}
 
void terminal_write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}
 
void terminal_writestring(const char* data) 
{
	terminal_write(data, strlen(data));
}

void terminal_move_cursor(unsigned short pos) {
	port_out(0x3D4, 14);
	port_out(0x3D5,    ((pos >> 8) & 0x00FF));
	port_out(0x3D4, 15);
	port_out(0x3D5,    pos & 0x00FF);
}

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
	desc->offset16_31 = (offset & 0xffff0000) >> 16;
}

void map_interrupts() {
	terminal_writestring("Init interrupts");
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
	
	edit_idt_entry(0x08, (unsigned int)irq0, 0x8E, &idt[32]);
	edit_idt_entry(0x08, (unsigned int)irq1, 0x8E, &idt[33]);
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
	
	idt_ptr.limit = 8 * 256;
	idt_ptr.base = (unsigned int)&idt;
	
	load_idt(&idt_ptr);
}

void irq1_handler(void) {
	port_out(0x20, 0x20);
	terminal_writestring("[X]");
	unsigned char x = port_in(0x60);
}



void kernel_main(void) {
	terminal_initialize();
	
	map_interrupts();
 
	terminal_writestring("Salut les amis !");
	
	terminal_writestring("W.");
	
	terminal_move_cursor(1);
	
	/*int a = 3 / 0;
	if(a == 0)
		terminal_move_cursor(3);*/
}

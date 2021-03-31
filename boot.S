# Très largement inspiré de https://wiki.osdev.org/Higher_Half_x86_Bare_Bones

.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

#include "conversion.h"
	
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss_stack
.align 16
stack_bottom:
.skip 16384 
stack_top:

.section .bss_boot_pages
.align 4096
boot_page_directory:
	.skip 4096
boot_page_table:
	.skip 4096 * 1024

.section .lower_half_boot , "ax"
.global _start
.type _start, @function
_start:
	lea TO_PHYS(boot_page_table), %edi
	lea 0x003, %eax
	lea START_VIRT >> 12, %ecx

1:
	mov %eax, (%edi)

	add $0x1000, %eax
	add $4, %edi
	dec    %ecx
	jnz     1b

	lea 0x003, %edx
	lea (START_VIRT >> 12), %ecx

2:
	mov %edx, (%edi)

	add $0x1000, %edx
	add $4, %edi
	dec    %ecx
	jnz     2b

	lea TO_PHYS(boot_page_directory), %edi
	lea TO_PHYS(boot_page_table) + 0x003, %edx
	lea 0x400, %ecx

3:
	mov %edx, (%edi)

	add $0x1000, %edx
	add $4, %edi
	dec    %ecx
	jnz     3b

	lea TO_PHYS(boot_page_directory), %ecx
	movl %ecx, %cr3
	movl %cr0, %ecx
	orl $0xC0010000, %ecx
	movl %ecx, %cr0

	lea _higher_half, %ecx
	jmp *%ecx

.size _start, . - _start

.section .text
.global _higher_half
.type _higher_half, @function
_higher_half:

	lea TO_PHYS(boot_page_directory), %edi
	lea START_VIRT >> 22, %ecx

4:
	movl $0, (%edi)

	add $4, %edi
	dec    %ecx
	jnz     4b

	mov $stack_top, %esp

	jmp load_gdt
  gdt:
  gdt_null:
  .quad 0

  gdt_code:
  .word 0xFFFF
  .word 0

  .byte 0
  .byte 0x9A
  .byte 0xCF
  .byte 0

  gdt_data:
  .word 0xFFFF
  .word 0
  .byte 0
  .byte 0x92
  .byte 0xCF
  .byte 0

  gdt_end:

  gdt_desc:
  .word gdt_end - gdt - 1
  .long gdt

  load_gdt:
    cli
    lgdt gdt_desc


	call kernel_main

	cli
1:	hlt
	jmp 1b

.size _higher_half, . - _higher_half



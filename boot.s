.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)
 
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
 
.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:
 
.section .text
.global _start
.type _start, @function
_start:
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
1:	hlt
	jmp 1b

.size _start, . - _start

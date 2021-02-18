.global port_out
port_out:
	movb 8(%esp), %al
	movw 4(%esp), %dx
	out %al, %dx
	ret

.global port_in
port_in:
	movw 4(%esp), %dx
	in %dx, %al
	ret

.global load_idt
load_idt:
	movl 4(%esp), %eax
	lidt (%eax)
	sti
	ret

.global timer_irq
timer_irq:
	pusha
	call timer_handler
	popa
	iret

.global keyboard_irq
keyboard_irq:
	pusha
	call keyboard_handler
	popa
	iret

.global irq2
irq2:
	pusha
	call irq2_handler
	popa
	iret

.global irq3
irq3:
	pusha
	call irq3_handler
	popa
	iret

.global irq4
irq4:
	pusha
	call irq4_handler
	popa
	iret

.global irq5
irq5:
	pusha
	call irq5_handler
	popa
	iret

.global irq6
irq6:
	pusha
	call irq6_handler
	popa
	iret

.global irq7
irq7:
	pusha
	call irq7_handler
	popa
	iret

.global irq8
irq8:
	pusha
	call irq8_handler
	popa
	iret

.global irq9
irq9:
	pusha
	call irq9_handler
	popa
	iret

.global irq10
irq10:
	pusha
	call irq10_handler
	popa
	iret

.global irq11
irq11:
	pusha
	call irq11_handler
	popa
	iret

.global irq12
irq12:
	pusha
	call irq12_handler
	popa
	iret

.global irq13
irq13:
	pusha
	call irq13_handler
	popa
	iret

.global irq14
irq14:
	pusha
	call irq14_handler
	popa
	iret

.global irq15
irq15:
	pusha
	call irq15_handler
	popa
	iret	
	
	

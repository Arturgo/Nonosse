ENTRY(_start)

START_VIRT = 0xC0000000;

SECTIONS
{
	. = 1M;

	.boot : ALIGN(4k) {
		*(.multiboot)
		*(.lower_half_boot)
		. = ALIGN(4k);
	}
	. = . + START_VIRT;
	.text : AT(ADDR(.text) - START_VIRT) ALIGN(4k) {
		*(.text)
	}

	.rodata : AT(ADDR(.rodata) - START_VIRT) ALIGN(4k) {
		*(.rodata)
	}

	.data : AT(ADDR(.data) - START_VIRT) ALIGN(4K) {
		*(.data)
	}

	.bss : AT(ADDR(.bss) - START_VIRT) {
		*(.bss_boot_pages)
		*(.bss_stack)
		*(COMMON)
	}

}


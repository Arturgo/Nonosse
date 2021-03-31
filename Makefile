CROSS        := i686-elf
WARN_FLAGS   := -O2 -W -Wall -Wextra
FLAGS        := -ffreestanding
ASFLAGS      := $(FLAGS) $(WARN_FLAGS)
CFLAGS       := $(FLAGS) $(WARN_FLAGS) -std=gnu99
CXXFLAGS     := $(FLAGS) $(WARN_FLAGS) -std=g++11 -fno-exceptions -fno-rtti
LDFLAGS      := $(FLAGS) -nostdlib -lgcc

CC := $(CROSS)-gcc
CXX := $(CROSS)-g++

all: boot

%.o: %.S
	$(CC) -c $(ASFLAGS) -o $@ $<

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

%.o: %.cc
	$(CXX) -c $(CXXFLAGS) -o $@ $<

clean:
	rm -rf nonosse.iso _isodir nonosse.bin *.o *~

%.bin: linker.ld 
	$(CC) -o $@ -T $+ $(LDFLAGS) 
	@grub-file --is-x86-multiboot $@ || {			\
	  printf "grub-file: error: %s is not multiboot\n" $@;	\
	  exit 1;						\
	}

nonosse.bin: boot.o cpu_asm.o kernel.o

check-multiboot: nonosse.bin
	if grub-file --is-x86-multiboot nonosse.bin; then	\
	  echo multiboot confirmed;			\
	else						\
	  echo the file is not multiboot;		\
	fi

nonosse.iso: grub.cfg nonosse.bin
	mkdir -p _isodir/boot/grub
	cp nonosse.bin _isodir/boot/
	cp grub.cfg _isodir/boot/grub/
	grub-mkrescue -o nonosse.iso _isodir

boot-cdrom: nonosse.iso
	qemu-system-i386 -cdrom nonosse.iso

boot: nonosse.bin
	qemu-system-i386 -kernel nonosse.bin

bochs: nonosse.iso
	bochs -f bochsrc

.PRECIOUS: %.bin %.iso
.PHONY: all clean boot bochs boot-cdrom


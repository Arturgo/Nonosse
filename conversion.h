#ifndef KERNEL__CONVERSION_H
#define KERNEL__CONVERSION_H 1

#define START_VIRT 0xC0000000
#define TO_PHYS(x) ((x) - START_VIRT)
#define TO_VIRT(x) ((x) + START_VIRT)

#endif

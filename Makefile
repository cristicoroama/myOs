CC      = aarch64-linux-gnu-gcc
AS      = aarch64-linux-gnu-as
LD      = aarch64-linux-gnu-ld
OBJCOPY = aarch64-linux-gnu-objcopy

CFLAGS  = -ffreestanding -nostdinc -nostdlib -nostartfiles -Iinclude
LDFLAGS = -nostdlib -nostartfiles

SRC_C   = kernel/uart.c kernel/printf.c kernel/irq.c kernel/mem.c kernel/kernel.c
SRC_S   = boot/start.S boot/irq.S

OBJ     = $(SRC_S:.S=.o) $(SRC_C:.c=.o)

all: kernel8.img

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel8.elf: $(OBJ)
	$(LD) $(LDFLAGS) -T linker.ld -o $@ $(OBJ)

kernel8.img: kernel8.elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -f $(OBJ) kernel8.elf kernel8.img

qemu:
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -serial stdio -display none

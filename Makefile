CC      = aarch64-linux-gnu-gcc
LD      = aarch64-linux-gnu-ld
OBJCOPY = aarch64-linux-gnu-objcopy

CFLAGS  = -ffreestanding -nostdinc -nostdlib -nostartfiles -Iinclude -Wno-int-to-pointer-cast
LDFLAGS = -nostdlib

SRC_C   = kernel/uart.c \
          kernel/printf.c \
          kernel/string.c \
          kernel/math.c \
          kernel/irq.c \
          kernel/mem.c \
          kernel/timer.c \
          kernel/gpio.c \
          kernel/mmu.c \
          kernel/syscall.c \
          kernel/sched.c \
          kernel/fs.c \
          kernel/process.c \
          kernel/keyboard.c \
          kernel/font.c \
          kernel/display.c \
          kernel/shell.c \
          kernel/kernel.c

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

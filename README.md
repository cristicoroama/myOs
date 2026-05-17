# MyOS — Bare Metal OS pentru Raspberry Pi 3B+

Un sistem de operare scris de la zero in C si Assembly AArch64.

## Structura

```
myos/
├── boot/
│   └── start.S       # Entry point Assembly
├── kernel/
│   ├── kernel.c      # Kernel principal
│   └── uart.c        # Driver UART PL011
├── include/
│   └── uart.h        # Header UART
├── linker.ld         # Linker script
└── Makefile          # Build system
```

## Cum compilezi

```bash
sudo apt install gcc-aarch64-linux-gnu qemu-system-aarch64 make
make
```

## Cum testezi in QEMU

```bash
make qemu
```

## Cum pui pe RPi 3B+

1. Formateaza un card SD in FAT32
2. Descarca firmware RPi: `bootcode.bin`, `start.elf`, `fixup.dat`
3. Copiaza `kernel8.img` pe card
4. Conecteaza un cablu USB-to-Serial la GPIO 14 (TX) si 15 (RX)
5. Porneste RPi si deschide un terminal serial la 115200 baud

## Etape de dezvoltare

- [x] Boot (start.S)
- [x] UART Driver
- [ ] Printf propriu
- [ ] Interrupts
- [ ] Memory Management
- [ ] Scheduler
- [ ] Shell

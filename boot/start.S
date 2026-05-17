.section ".text.boot"
.global _start

_start:
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbz     x1, core0

park:
    wfe
    b       park

core0:
    mov     x0, #(1 << 31)
    msr     hcr_el2, x0

    mov     x0, #0x3c5
    msr     spsr_el2, x0

    adr     x0, el1_entry
    msr     elr_el2, x0

    eret

el1_entry:
    ldr     x1, =_start
    mov     sp, x1

    ldr     x1, =__bss_start
    ldr     x2, =__bss_end

bss_loop:
    cmp     x1, x2
    bge     bss_done
    str     xzr, [x1], #8
    b       bss_loop

bss_done:
    bl      kernel_main

hang:
    b       hang

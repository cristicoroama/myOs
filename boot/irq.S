.section ".text"
.global irq_init_vectors

.macro HANDLER label
    .align 7
    b \label
.endm

.align 11
vector_table:
    HANDLER el1_sync
    HANDLER el1_irq
    HANDLER el1_fiq
    HANDLER el1_error
    HANDLER el1_sync
    HANDLER el1_irq
    HANDLER el1_fiq
    HANDLER el1_error
    HANDLER el1_sync
    HANDLER el1_irq
    HANDLER el1_fiq
    HANDLER el1_error
    HANDLER el1_sync
    HANDLER el1_irq
    HANDLER el1_fiq
    HANDLER el1_error

irq_init_vectors:
    adr     x0, vector_table
    msr     vbar_el1, x0
    ret

el1_sync:
    bl      handle_sync
    eret

el1_irq:
    bl      handle_irq
    eret

el1_fiq:
    bl      handle_fiq
    eret

el1_error:
    bl      handle_error
    eret

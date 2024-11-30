.global _start2
.global CurrCpuContex

.macro PSUH_SYSTEM_REGS

    str x30, [sp, #-8]!
    stp x28, x29, [sp, #-16]!     // x28  x29
    stp x26, x27, [sp, #-16]!     // x26  x27
    stp x24, x25, [sp, #-16]!     // x24  x25
    stp x22, x23, [sp, #-16]!     // x22  x23
    stp x20, x21, [sp, #-16]!     // x20  x21
    stp x18, x19, [sp, #-16]!     // x18  x19
    stp x16, x17, [sp, #-16]!     // x16  x17
    stp x14, x15, [sp, #-16]!     // x14  x15
    stp x12, x13, [sp, #-16]!     // x12  x13
    stp x10, x11, [sp, #-16]!     // x10  x11
    stp x8, x9, [sp, #-16]!       // x8  x9
    stp x6, x7, [sp, #-16]!       // x6  x7
    stp x4, x5, [sp, #-16]!       // x4  x5
    stp x2, x3, [sp, #-16]!       // x2  x3
    stp x0, x1, [sp, #-16]!       // x0  x1//saved 0x also kann benutzt werden

    mrs x0, sp_el0              //  elr_el1
    str x0, [sp, #-8]!
    mrs x0, elr_el1              //  elr_el1
    str x0, [sp, #-8]!
    mrs x0, spsr_el1             //  spsr_el1
    str x0, [sp, #-8]!
    mrs x0, far_el1              //  far_el1
    str x0, [sp, #-8]!

.endm

.macro RESTORE_SYSTEM_REGS
    ldr x0, [sp], #8             //  far_el1  
    msr far_el1, x0
    ldr x0, [sp], #8             //  spsr_el1  
    msr spsr_el1, x0
    ldr x0, [sp], #8             //  elr_el1  
    msr elr_el1, x0    
    ldr x0, [sp], #8             //  elr_el1  
    msr sp_el0, x0  

    //restore all registers from stack
    ldp x0, x1, [sp], #16        //  x0  x1  
    ldp x2, x3, [sp], #16        //  x2  x3  
    ldp x4, x5, [sp], #16        //  x4  x5  
    ldp x6, x7, [sp], #16        //  x6  x7  
    ldp x8, x9, [sp], #16        //  x8  x9  
    ldp x10, x11, [sp], #16      //  x10  x11  
    ldp x12, x13, [sp], #16
    ldp x14, x15, [sp], #16
    ldp x16, x17, [sp], #16
    ldp x18, x19, [sp], #16
    ldp x20, x21, [sp], #16
    ldp x22, x23, [sp], #16
    ldp x24, x25, [sp], #16
    ldp x26, x27, [sp], #16
    ldp x28, x29, [sp], #16      //  x28  x29 
    ldr x30, [sp], #8

.endm

.macro LOAD_FROM_CONTEXT_TO_CPU
    LDR X20, =CurrCpuContex
    LDR X21, = 0xffffff8000000000//convert to kernel adress
    add X20 , X21, x20 // in x0 kernel adress of cpuContext

    ldr x0, [X20, #0]       
    msr far_el1, x0

    ldr x0, [X20, #8]           
    msr spsr_el1, x0

    ldr x0, [X20, #16]            
    msr elr_el1, x0

    ldr x0, [X20, #24]            
    msr sp_el0, x0

    ldp x0, x1, [X20, #32]
    ldp x2, x3, [X20, #48]
    ldp x4, x5, [X20, #64]
    ldp x6, x7, [X20, #80]
    ldp x8, x9, [X20, #96]
    ldp x10, x11, [X20, #112]
    ldp x12, x13, [X20, #128]
    ldp x14, x15, [X20, #144]
    ldp x16, x17, [X20, #160]
    ldp x18, x19, [X20, #176]
    ldp x22, x23, [X20, #208]
    ldp x24, x25, [X20, #224]
    ldp x26, x27, [X20, #240]
    ldp x28, x29, [X20, #256]
    ldr x30, [X20, #272]   // X30 einzeln laden
    ldp x20, x21, [X20, #192]  //am ende x20 CPUCONTEXSTRUCT drinnen (darf davor nicht überschrieben werden)

.endm

.macro REGS_FROM_STACK_TO_CPUCONTEXT

    LDR X20, =CurrCpuContex
    LDR X21, = 0xffffff8000000000//convert to kernel adress
    add X20 , X21, x20 // in x0 kernel adress of cpuContext

    //x0-x30
    LDR X9, [SP, #0x00]       // far
    STR X9, [X20, #0x00]      
    
    LDR X9, [SP, #0x08]       // spsel
    STR X9, [X20, #0x08]       

    LDR X9, [SP, #0x10]       // elr
    STR X9, [X20, #0x10]       

    LDR X9, [SP, #0x18]       // sp
    STR X9, [X20, #0x18]      

    LDR X9, [SP, #0x20]       // 0
    STR X9, [X20, #0x20]      

    LDR X9, [SP, #0x28]       // 1
    STR X9, [X20, #0x28]       

    LDR X9, [SP, #0x30]       // 2
    STR X9, [X20, #0x30]       

    LDR X9, [SP, #0x38]       // 3
    STR X9, [X20, #0x38]      

    LDR X9, [SP, #0x40]       // 4
    STR X9, [X20, #0x40]       

    LDR X9, [SP, #0x48]       // 5
    STR X9, [X20, #0x48]      

    LDR X9, [SP, #0x50]       // 6
    STR X9, [X20, #0x50]      

    LDR X9, [SP, #0x58]       // 7
    STR X9, [X20, #0x58]       

    LDR X9, [SP, #0x60]       // 8
    STR X9, [X20, #0x60]      

    LDR X9, [SP, #0x68]       // 9
    STR X9, [X20, #0x68]      

    LDR X9, [SP, #0x70]       // 10
    STR X9, [X20, #0x70]       

    LDR X9, [SP, #0x78]       // 11
    STR X9, [X20, #0x78]     

    LDR X9, [SP, #0x80]       // 12
    STR X9, [X20, #0x80]      

    LDR X9, [SP, #0x88]       // 13
    STR X9, [X20, #0x88]      

    LDR X9, [SP, #0x90]       // 14
    STR X9, [X20, #0x90]       

    LDR X9, [SP, #0x98]       // 15
    STR X9, [X20, #0x98]       

    LDR X9, [SP, #0xA0]       // 16
    STR X9, [X20, #0xA0]     

    LDR X9, [SP, #0xA8]       // 17
    STR X9, [X20, #0xA8]       

    LDR X9, [SP, #0xB0]       // 18
    STR X9, [X20, #0xB0]       

    LDR X9, [SP, #0xB8]       // 19
    STR X9, [X20, #0xB8]       

    LDR X9, [SP, #0xC0]       // 20
    STR X9, [X20, #0xC0]       

    LDR X9, [SP, #0xC8]       // 21
    STR X9, [X20, #0xC8]       

    LDR X9, [SP, #0xD0]       // 22
    STR X9, [X20, #0xD0]       

    LDR X9, [SP, #0xD8]       // 23
    STR X9, [X20, #0xD8]      

    LDR X9, [SP, #0xE0]       // 24
    STR X9, [X20, #0xE0]      

    LDR X9, [SP, #0xE8]       // 25
    STR X9, [X20, #0xE8]       

    LDR X9, [SP, #0xf0]       // 26
    STR X9, [X20, #0xf0]      

    LDR X9, [SP, #0xf8]       // 27
    STR X9, [X20, #0xf8]      

    LDR X9, [SP, #0x100]       // 28
    STR X9, [X20, #0x100]      

    LDR X9, [SP, #0x108]       // 29
    STR X9, [X20, #0x108]     

    LDR X9, [SP, #0x110]       // 30
    STR X9, [X20, #0x110]      

.endm

_start2:
    ldr x1, = 0xffffff8000000000 
    ldr x0, = _stack_end    // Setze Stack-Ende-Adresse
    add x0 , x1, x0
    mov sp, x0             // Initialisiere Stack-Pointer
    ldr     x2, =vector_table 
    add x2 , x1, x2
    msr     vbar_el1, x2
    
    b kernelMain             // jump main
    b hang
hang:
    b hang

.text
.global vector_table
.balign 2048
vector_table:
        b save_context_and_handle_exception
    .balign 0x80
        b irq_handler
    .balign 0x80
        b save_context_and_handle_exception
    .balign 0x80
        b save_context_and_handle_exception

    .balign 0x80
        b save_context_and_handle_exception
    .balign 0x80
        b irq_handler
    .balign 0x80
        b save_context_and_handle_exception
    .balign 0x80
        b save_context_and_handle_exception
    .balign 0x80

        b save_context_and_handle_exception
    .balign 0x80
        b irq_handler
    .balign 0x80
        b save_context_and_handle_exception
    .balign 0x80
        b save_context_and_handle_exception
    .balign 0x80

        b save_context_and_handle_exception
    .balign 0x80
        b irq_handler
    .balign 0x80
        b save_context_and_handle_exception
    .balign 0x80
        b save_context_and_handle_exception




save_context_and_handle_exception:
    //push all registers to stack
    PSUH_SYSTEM_REGS
    REGS_FROM_STACK_TO_CPUCONTEXT

    //MSR DAIFClr, #0b0010 //demaskiere interrupt

    mov     x0, #1
    mrs     x1, esr_el1
    mrs     x2, elr_el1
    mrs     x3, spsr_el1
    mrs     x4, far_el1
    mrs     x5, SP_el0
    bl exc_handler
    
    //restoreAll sysRegs
    RESTORE_SYSTEM_REGS

    eret

irq_handler:
    //push all registers to stack
    PSUH_SYSTEM_REGS

    //schiebe den kompletten stack auf den cpuContext
    REGS_FROM_STACK_TO_CPUCONTEXT
    
    // Lese die IRQ-ID aus GICC_IAR
    LDR x26, =0xffffff8008010000
    LDR x25, [x26, #0x00C]

    //setze Timer auf neuen wert
    MRS X0, CNTVCT_EL0
    ldr x1 , =0xf0000
    ADD X1, X0, x1
    MSR CNTV_CVAL_EL0, X1

    mov     x0, #2
    mrs     x1, esr_el1
    mrs     x2, elr_el1
    mrs     x3, spsr_el1
    mrs     x4, far_el1
    mrs     x5, SP_el0
    bl exc_handler

    STR x25, [x26, #0x010]

    //RESTORE_SYSTEM_REGS
    LOAD_FROM_CONTEXT_TO_CPU
    eret


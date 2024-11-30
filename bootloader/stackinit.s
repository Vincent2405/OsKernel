.global _start
_start:
    ldr x0, =_stack_end    // Setze Stack-Ende-Adresse
    mov sp, x0             // Initialisiere Stack-Pointer
    
    bl main             // Rufe main() auf

hang:
    b hang

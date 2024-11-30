
ENTRY(_start)
SECTIONS {
    /* Bootloader Code */
    . = 0x40200000;  /* Startadresse des Bootloaders */
    
    .text : {
        *(.text)        /* Bootloader-Code */
    }

    .data : {
        *(.data)        /* Bootloader-Daten */
    }

    
    .bss : {
        *(.bss)
    }

    .stack : {
        . = ALIGN(8);
        _stack_start = .;         /* Startadresse des Stacks */
        . = . + 0x1000;           /* Größe des Stacks (z. B. 4 KB) */
        _stack_end = .;           /* Ende des Stacks */
    }

    /* Kernel-Image in eine eigene Section */
    . = ALIGN(0x200000); 
    _kernelIm =.+ 0xf000;
    .kernel_image : {
        . = . + 0xf000;          /* Größe des Stacks (z. B. 4 KB) */
    }
    _kernelEnd =. ;
    . = ALIGN(4096); 
    _KernelPageTable = .; 

    _end = .;
}


ENTRY(_start2)
SECTIONS
{

    . = 0x000000;
    .text : {
        *(.text)        /* Bootloader-Code */
    }

    .data : {
        /* ... */
        _stack_start = .;         /* Startadresse des Stacks */
        . = . + 0xf0000;           /* Größe des Stacks (z. B. 4 KB) */
        _stack_end = .;           /* Ende des Stacks */
    }

    .bss (NOLOAD) : {
        . = ALIGN(16);
        __bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        __bss_end = .;
    }
    
    . = ALIGN(1024*1024*2);

    _kernelPageTabelStart = .;

    .kernelPageTabel : {
        . = . + 0x9000;
        _kernelPageTabelEnd = .;
    }
    
    . = ALIGN(0x1000);

    _UserAppImStart =.+ 0xf000;
    .userapp_image : {
        . = . + 0xf000;          /* Größe des Stacks (z. B. 4 KB) */
    }
    _UserAppImEnd =. ;

    . = . + 0xf000;

    . = ALIGN(0x1000);

     _UserAppImStart2 =.+ 0xf000;
    .userapp_image2 : {
        . = . + 0xf000;          /* Größe des Stacks (z. B. 4 KB) */
    }
    _UserAppImEnd2 =. ;

    _end = .;

}
__bss_size = (__bss_end - __bss_start)>>3;

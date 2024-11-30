ENTRY(main2)
SECTIONS {
    . = 0x00;  

    .text : {
        *(.text)        /* Bootloader-Code */
    }

    .data : {
        *(.data)        /* Bootloader-Daten */
    }

    
    .bss : {
        *(.bss)
    }
}

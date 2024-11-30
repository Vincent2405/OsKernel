/*grösstenteils von https://github.com/bztsrc/raspi3-tutorial/blob/master/10_virtualmemory/mmu.c*/

#define UART0_BASE 0x09000000
// UART Registers
#define UARTDR     (*(volatile unsigned int *)(UART0_BASE + 0x00))
#define UARTFR     (*(volatile unsigned int *)(UART0_BASE + 0x18))
#define UARTIBRD   (*(volatile unsigned int *)(UART0_BASE + 0x24))
#define UARTFBRD   (*(volatile unsigned int *)(UART0_BASE + 0x28))
#define UARTLCR_H  (*(volatile unsigned int *)(UART0_BASE + 0x2C))
#define UARTCR     (*(volatile unsigned int *)(UART0_BASE + 0x30))
#define UART_BASE 0x09000000
#define UART_DR   (*(volatile unsigned int *)(UART_BASE + 0x00))  // Datenregister
#define VA_UART__BASE 0x09000000 //initial physisch
// Initialize UART
void BLuart_init2() {
    // Disable UART0.
    UARTCR = 0;

    // Set integer & fractional part of baud rate.
    UARTIBRD = 1;    // Integer part (for 115200 baud rate, assuming 24 MHz UART clock)
    UARTFBRD = 40;   // Fractional part

    // Set data format: 8 bits, no parity, 1 stop bit.
    UARTLCR_H = (1 << 5) | (1 << 6); // WLEN = 0b11 (8 bits)

    // Enable UART0, TX, and RX.
    UARTCR = (1 << 0) | (1 << 8) | (1 << 9); // UARTEN, TXE, RXE
}

void BLuart_send2(char c) {
    extern volatile int useVASpace;
    extern volatile unsigned int VAmmioBase;


    UART_DR = c;  // Schreiboperation in das UART-Datenregister
    
}


void BLuart_sendString(char* string,int size)
{
    for(int i = 0; i< size;i++)
    {
        BLuart_send2(*(string+i));
    }
}

void BLkprint(char* str,unsigned int size)
{
    BLuart_sendString(str,size);
}

void BLuart_print_hex(unsigned long value) {
    char hex_chars[] = "0123456789ABCDEF";
    BLuart_send2('0');
    BLuart_send2('x');

    for (int i = (sizeof(unsigned long) * 2) - 1; i >= 0; i--) {
        int nibble = (value >> (i * 4)) & 0xF;
        BLuart_send2(hex_chars[nibble]);
    }
    BLuart_send2('\n');
}

void BLuart_print_address(void* addr) {
    BLuart_print_hex((unsigned long)addr);  // Adresse als unsigned long übergeben
}

// UART Base Address for QEMU virt machine
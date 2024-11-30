/*grösstenteils von https://github.com/bztsrc/raspi3-tutorial/blob/master/10_virtualmemory/mmu.c*/


#include "sprintf.h"
#include "../memory.h"
#include "fd.h"
#include "../heap.h"
#define EOF (-1)
#define UART0_BASE 0xffffff8009000000
#define MMIO_BASE 0xffffff8009000000
#define MAXSTDOUPUFFER 100
// UART Registers
#define UARTDR     (*(volatile unsigned int *)(UART0_BASE + 0x00))
#define UARTFR     (*(volatile unsigned int *)(UART0_BASE + 0x18))
#define UARTIBRD   (*(volatile unsigned int *)(UART0_BASE + 0x24))
#define UARTFBRD   (*(volatile unsigned int *)(UART0_BASE + 0x28))
#define UARTLCR_H  (*(volatile unsigned int *)(UART0_BASE + 0x2C))
#define UARTCR     (*(volatile unsigned int *)(UART0_BASE + 0x30))
#define UART_DR   (*(volatile unsigned int *)(UART0_BASE + 0x00))  // Datenregister



// Initialize UART

extern volatile unsigned char _end;

void uart_init2() {
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

void uart_send2(char c) { 
    
    UART_DR = c;  // Schreiboperation in das UART-Datenregister
    
   
}

void stdoutToConsole()
{
    uart_sendString("[STDOUT]: ",10);

    //read from fd 0
    char* data=getDataPointer(1);
    for (int i = 0; i < MAXSTDOUPUFFER; i++)
    {
        if (data[i]==EOF)
        {
            break;
        }
        uart_send2(data[i]);
    }

}
void ConsoleToStdInd()
{

}


void uart_sendString(char* string,int size)
{
    int i=0;
    while (string[i]!='\0'&& i< size)
    {
        uart_send2(*(string+i));
        i++;
    }
}

void kprint(char* str,unsigned int size)
{
    uart_sendString(str,size);
    //memset(str,'\0',size);
}

void uart_print_hex(unsigned long value) {
    char hex_chars[] = "0123456789ABCDEF";
    uart_send2('0');
    uart_send2('x');

    // Schleife für 16 Hex-Stellen (64-Bit-Wert)
    for (int i = (sizeof(unsigned long) * 2) - 1; i >= 0; i--) {
        int nibble = (value >> (i * 4)) & 0xF;
        uart_send2(hex_chars[nibble]);
    }
    uart_send2('\n');
}

char uart_getchar() {
    char r;
    /* wait until something is in the buffer */
    do{asm volatile("nop");}while(UARTFR&0x10);
    /* read it and return */
    r=(char)(UART_DR);
    /* convert carrige return to newline */
    //uart_send2(r);
    return r=='\r'?'\n':r;
}



void uart_getstring(char *buffer, int max_len) {

    memset(buffer,'\0',max_len);
    int index = 0;
    char c;

    while (1) {
        c = uart_getchar(); 
        //echo current char
        uart_send2(c);
        if (c == '\b' || c == 0x7F) { 
            uart_send2('\b'); 
            uart_send2(' ');  
            uart_send2('\b'); 
            
            buffer[index] = ' ';
            index-=1;
        }
        else{
            if (c == '\n' || c == '\r') { 
                break;
            }

            if (index < max_len - 1) { 
                buffer[index++] = c;
            }
        }
    }

    buffer[index] = '\0'; // Null-Terminierung
}

// UART Base Address for QEMU virt machine

void strncpy(char *dest, const char *src, int strsize) {
    if (strsize == 0) return;  

    int i;
    for (i = 0; i < strsize - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }

    dest[i] = '\0';  // Nullterminierung hinzufügen
}
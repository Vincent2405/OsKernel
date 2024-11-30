#include "uart.h"
#include "cpuContext.h"
struct CPUContext CurrCpuContex;//statischer globaler kontext

CPUContext* getCurrentCpuContext()
{
    return &CurrCpuContex;
}

void setelr(unsigned long elr)
{
    CurrCpuContex.ELR=elr;
}
//nicht mehr anfassen sonst geht stack kaputt!!!!
//TODO das muss noch besser gemacht werden 
void saveCurrCpuStateToProcess(CPUContext* processCpuContext)
{
    
    processCpuContext->far = CurrCpuContex.far;
    processCpuContext->spsel = CurrCpuContex.spsel;
    processCpuContext->ELR = CurrCpuContex.ELR;
    processCpuContext->SP = CurrCpuContex.SP;
    processCpuContext->x0 = CurrCpuContex.x0;
    processCpuContext->x1 = CurrCpuContex.x1;
    processCpuContext->x2 = CurrCpuContex.x2;
    processCpuContext->x3 = CurrCpuContex.x3;
    processCpuContext->x4 = CurrCpuContex.x4;
    processCpuContext->x5 = CurrCpuContex.x5;
    processCpuContext->x6 = CurrCpuContex.x6;
    processCpuContext->x7 = CurrCpuContex.x7;
    processCpuContext->x8 = CurrCpuContex.x8;
    processCpuContext->x9 = CurrCpuContex.x9;
    processCpuContext->x10 = CurrCpuContex.x10;
    processCpuContext->x11 = CurrCpuContex.x11;
    processCpuContext->x12 = CurrCpuContex.x12;
    processCpuContext->x13 = CurrCpuContex.x13;
    processCpuContext->x14 = CurrCpuContex.x14;
    processCpuContext->x15 = CurrCpuContex.x15;
    processCpuContext->x16 = CurrCpuContex.x16;
    processCpuContext->x17 = CurrCpuContex.x17;
    processCpuContext->x18 = CurrCpuContex.x18;
    processCpuContext->x19 = CurrCpuContex.x19;
    processCpuContext->x20 = CurrCpuContex.x20;
    processCpuContext->x21 = CurrCpuContex.x21;
    processCpuContext->x22 = CurrCpuContex.x22;
    processCpuContext->x23 = CurrCpuContex.x23;
    processCpuContext->x24 = CurrCpuContex.x24;
    processCpuContext->x25 = CurrCpuContex.x25;
    processCpuContext->x26 = CurrCpuContex.x26;
    processCpuContext->x27 = CurrCpuContex.x27;
    processCpuContext->x28 = CurrCpuContex.x28;
    processCpuContext->x29 = CurrCpuContex.x29;  // Frame Pointer (FP)
    processCpuContext->x30 = CurrCpuContex.x30;  // Link Register (LR)
   
}

void loadCpuContextFromProcess(CPUContext* processCpuContext)
{
    CurrCpuContex.far = processCpuContext->far;
    CurrCpuContex.spsel = processCpuContext->spsel;
    CurrCpuContex.ELR = processCpuContext->ELR;
    CurrCpuContex.SP = processCpuContext->SP;
    CurrCpuContex.x0 = processCpuContext->x0;
    CurrCpuContex.x1 = processCpuContext->x1;
    CurrCpuContex.x2 = processCpuContext->x2;
    CurrCpuContex.x3 = processCpuContext->x3;
    CurrCpuContex.x4 = processCpuContext->x4;
    CurrCpuContex.x5 = processCpuContext->x5;
    CurrCpuContex.x6 = processCpuContext->x6;
    CurrCpuContex.x7 = processCpuContext->x7;
    CurrCpuContex.x8 = processCpuContext->x8;
    CurrCpuContex.x9 = processCpuContext->x9;
    CurrCpuContex.x10 = processCpuContext->x10;
    CurrCpuContex.x11 = processCpuContext->x11;
    CurrCpuContex.x12 = processCpuContext->x12;
    CurrCpuContex.x13 = processCpuContext->x13;
    CurrCpuContex.x14 = processCpuContext->x14;
    CurrCpuContex.x15 = processCpuContext->x15;
    CurrCpuContex.x16 = processCpuContext->x16;
    CurrCpuContex.x17 = processCpuContext->x17;
    CurrCpuContex.x18 = processCpuContext->x18;
    CurrCpuContex.x19 = processCpuContext->x19;
    CurrCpuContex.x20 = processCpuContext->x20;
    CurrCpuContex.x21 = processCpuContext->x21;
    CurrCpuContex.x22 = processCpuContext->x22;
    CurrCpuContex.x23 = processCpuContext->x23;
    CurrCpuContex.x24 = processCpuContext->x24;
    CurrCpuContex.x25 = processCpuContext->x25;
    CurrCpuContex.x26 = processCpuContext->x26;
    CurrCpuContex.x27 = processCpuContext->x27;
    CurrCpuContex.x28 = processCpuContext->x28;
    CurrCpuContex.x29 = processCpuContext->x29;  // Frame Pointer (FP)
    CurrCpuContex.x30 = processCpuContext->x30;  // Link Register (LR)
    
}



void printCpuContext(CPUContext* c)
{
    if (c == 0x0) {
        kprint("Error: CPUContext is NULL\n", 25);
        return;
    }

    kprint("X0: ", 4);
    uart_print_hex(c->x0);

    kprint("X1: ", 4);
    uart_print_hex(c->x1);

    kprint("X2: ", 4);
    uart_print_hex(c->x2);

    kprint("X3: ", 4);
    uart_print_hex(c->x3);

    kprint("X4: ", 4);
    uart_print_hex(c->x4);

    kprint("X5: ", 4);
    uart_print_hex(c->x5);

    kprint("X6: ", 4);
    uart_print_hex(c->x6);

    kprint("X7: ", 4);
    uart_print_hex(c->x7);

    kprint("X8: ", 4);
    uart_print_hex(c->x8);

    kprint("X9: ", 4);
    uart_print_hex(c->x9);

    kprint("X10: ", 5);
    uart_print_hex(c->x10);

    kprint("X11: ", 5);
    uart_print_hex(c->x11);

    kprint("X12: ", 5);
    uart_print_hex(c->x12);

    kprint("X13: ", 5);
    uart_print_hex(c->x13);

    kprint("X14: ", 5);
    uart_print_hex(c->x14);

    kprint("X15: ", 5);
    uart_print_hex(c->x15);

    kprint("X16: ", 5);
    uart_print_hex(c->x16);

    kprint("X17: ", 5);
    uart_print_hex(c->x17);

    kprint("X18: ", 5);
    uart_print_hex(c->x18);

    kprint("X19: ", 5);
    uart_print_hex(c->x19);

    kprint("X20: ", 5);
    uart_print_hex(c->x20);

    kprint("X21: ", 5);
    uart_print_hex(c->x21);

    kprint("X22: ", 5);
    uart_print_hex(c->x22);

    kprint("X23: ", 5);
    uart_print_hex(c->x23);

    kprint("X24: ", 5);
    uart_print_hex(c->x24);

    kprint("X25: ", 5);
    uart_print_hex(c->x25);

    kprint("X26: ", 5);
    uart_print_hex(c->x26);

    kprint("X27: ", 5);
    uart_print_hex(c->x27);

    kprint("X28: ", 5);
    uart_print_hex(c->x28);

    kprint("X29 (FP): ", 10);
    uart_print_hex(c->x29);

    kprint("X30 (LR): ", 10);
    uart_print_hex(c->x30);

    kprint("SP_EL0: ", 10);
    uart_print_hex(c->SP);

    kprint("ELR_EL1: ", 10);
    uart_print_hex(c->ELR);

    kprint("far: ", 10);
    uart_print_hex(c->far);
    kprint("spsel: ", 10);
    uart_print_hex(c->spsel);
    
}

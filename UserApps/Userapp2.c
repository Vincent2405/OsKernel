#include "stdLib/syscalls.h";

int main2()
{

    asm volatile("mov x0, #0");
    asm volatile("mov x1, #1");
    asm volatile("mov x2, #2");
    asm volatile("mov x3, #3");
    asm volatile("mov x4, #4");
    asm volatile("mov x5, #5");
    asm volatile("mov x6, #6");
    asm volatile("mov x7, #7");
    asm volatile("mov x8, #8");
    asm volatile("mov x9, #9");
    asm volatile("mov x10, #10");
    asm volatile("mov x11, #11");
    asm volatile("mov x12, #12");
    asm volatile("mov x13, #13");
    asm volatile("mov x14, #14");
    asm volatile("mov x15, #15");
    asm volatile("mov x16, #16");
    asm volatile("mov x17, #17");
    asm volatile("mov x18, #18");
    asm volatile("mov x19, #19");
    asm volatile("mov x20, #20");
    asm volatile("mov x21, #21");
    asm volatile("mov x22, #22");
    asm volatile("mov x23, #23");
    asm volatile("mov x24, #24");
    asm volatile("mov x25, #25");
    asm volatile("mov x26, #26");
    asm volatile("mov x27, #27");
    asm volatile("mov x28, 0xfa");
    asm volatile("mov x29, #29");
    asm volatile("mov x30, #30");

    while (1)
    {
        print("process2läuft\n",15);        
    }
    return 0;
}

#ifndef CPUCONTEXT_H
#define CPUCONTEXT_H

typedef struct CPUContext
{
    unsigned long far;
    unsigned long spsel;
    unsigned long ELR;
    unsigned long SP;
    unsigned long x0;
    unsigned long x1;
    unsigned long x2;
    unsigned long x3;
    unsigned long x4;
    unsigned long x5;
    unsigned long x6;
    unsigned long x7;
    unsigned long x8;
    unsigned long x9;
    unsigned long x10;
    unsigned long x11;
    unsigned long x12;
    unsigned long x13;
    unsigned long x14;
    unsigned long x15;
    unsigned long x16;
    unsigned long x17;
    unsigned long x18;
    unsigned long x19;
    unsigned long x20;
    unsigned long x21;
    unsigned long x22;
    unsigned long x23;
    unsigned long x24;
    unsigned long x25;
    unsigned long x26;
    unsigned long x27;
    unsigned long x28;
    unsigned long x29;
    unsigned long x30;
    

}CPUContext;
void safeCurrentConext();
CPUContext* getCurrentCpuContext();
void loadCpuContextFromStack();
void setelr(unsigned long elr);
void saveCurrCpuStateToProcess(CPUContext* processCpuContext);
void loadCpuContextFromProcess(CPUContext* processCpuContext);
int loadToCpu(CPUContext* context);
void printCpuContext(CPUContext* c);
#endif
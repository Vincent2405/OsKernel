#include "bootUart.h"
#include "mmu.h"
/*FLAGS UND DREFINES von https://github.com/bztsrc/raspi3-tutorial/blob/master/10_virtualmemory/mmu.c*/

unsigned long VAmmioBase=0x9000000;
// get addresses from linker
extern volatile unsigned char _data;
extern volatile unsigned char _kernelIm;
extern volatile unsigned char _end;//extern volatile unsigned char _end;
extern volatile unsigned char _kernelEnd;//extern volatile unsigned char _end;
//extern volatile unsigned char _end;

//unsigned long *pageTableStartAddr =(unsigned long *)0x40003000; //muss im ram liegen

void breakP(){}

void jump_to_address(unsigned long address);

unsigned long get_current_EL();
unsigned long read_pc();

int l1Count=0;
int l2Count=0;
int l3Count=0;

int useVASpace=0;

/**
 * Set up page translation tables and enable virtual memory
 */
unsigned long *KernelRamMapping;
unsigned long *UserRamMapping;

void *memcpy(void *dest, void *src, unsigned long n) {
    char *d = dest;
    char *s = src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}


void mmu_init()
{
    /*bootloader mmu init*/;


    //calculate the start of the pagetable:
    //unsigned long *test = (unsigned long*) 0x47002000;
    //test[0]=0xabcdeffff;

    unsigned long mmoBase=0x9000000;

    unsigned long ramStart=0x40000000;
    unsigned long ramEnd=0x47ffffff;
    unsigned long ramSize=ramEnd-ramStart;

    unsigned long KernelramSize=0x1000000; //16mb z.b.
    //unsigned long KernelramStart=ramEnd-0x1000000 +1; //+1 für alignment? //0x47000000
    //unsigned long KernelramEnd=ramEnd;
    unsigned long KernelramStart=(unsigned long )&_kernelIm; //+1 für alignment? //0x47000000
    unsigned long KernelramEnd=(unsigned long )&_kernelEnd; //+1 für alignment? //0x47000000

    unsigned long UserRamSize=ramSize-KernelramSize; //restlicher Ram z.b.also zb.b 128-16 mb 
    unsigned long UserRamStart=USERRAMSTART; 
    unsigned long UserRamEnd=UserRamStart+UserRamSize;
    
    int pagesKernelMMIO=1; // in mb

    int pagesNeeded = (UserRamSize +  +PAGESIZE-1) / PAGESIZE; // Anzahl der 4 KB Seiten
    

    int KernelpagesNeeded = (KernelramSize + (pagesKernelMMIO*PAGESIZE*PAGESIZE) +PAGESIZE-1) / PAGESIZE; // Anzahl der 4 KB Seiten

    int Kernell3Count = (KernelpagesNeeded); // Rundet auf, indem 511 hinzugefügt wird
    int Kernell2Count = (Kernell3Count + 511) / 512; // Rundet auf, indem 511 hinzugefügt wird
    int Kernell1Count = (Kernell2Count + 511) / 512;    // Rundet auf

    //irgend eine adresse im userspace wird danach eh
    KernelRamMapping=(unsigned long*)0x48000000; //möglicherweise alignment prüfen
    unsigned long UserTaleStart=(unsigned long)(KernelRamMapping ) | (unsigned long)0x200000 | 0x1000 ; // * 8 für bytes used //TODO berechne size korrekekt
    UserRamMapping=(unsigned long*)0x400f0000; 

    unsigned long r, b=(unsigned long)0x00;

    unsigned long kenelDest=0x47000000;
    //copy kernel an kernelStartAddress;

    //KernelMappingTable
    for (int i = 0; i < 512; i++) //64 bei 128mb
    {
        //2mbBlock
        UserRamMapping[Kernell1Count*512+i]=(unsigned long)(UserRamStart + (i<<21)) |    // physical address
        PT_BLOCK |     // it has the "Present" flag, which must be set, and we have area in it mapped by pages
        PT_AF |       // accessed flag. Without this we're going to have a Data Abort exception
        PT_KERNEL |     // non-privileged
        PT_ISH |      // inner shareable
        PT_MEM |       // normal memory
        PT_RW;
    }
    //kernel space
    for (int i = 0; i < 512; i++) //64 bei 128mb
    {
        //2mbBlock
        KernelRamMapping[512+i]=(unsigned long)(kenelDest + (i<<21)) |    // physical address
        PT_BLOCK |     // it has the "Present" flag, which must be set, and we have area in it mapped by pages
        PT_AF |       // accessed flag. Without this we're going to have a Data Abort exception
        PT_KERNEL |     // non-privileged
        PT_ISH |      // inner shareable
        PT_MEM |       // normal memory
        PT_RW;
    }
    
    KernelRamMapping[0]=(unsigned long)((unsigned char*)&KernelRamMapping[512]) |    // physical address
        PT_PAGE |     // it has the "Present" flag, which must be set, and we have area in it mapped by pages
        PT_AF |       // accessed flag. Without this we're going to have a Data Abort exception
        PT_KERNEL |     // non-privileged
        PT_ISH |      // inner shareable
        PT_MEM ;      // normal memory  
    KernelRamMapping[1]=(unsigned long)((unsigned char*)&KernelRamMapping[512+512]) |    // physical address
        PT_PAGE |     // it has the "Present" flag, which must be set, and we have area in it mapped by pages
        PT_AF |       // accessed flag. Without this we're going to have a Data Abort exception
        PT_KERNEL |     // non-privileged
        PT_ISH |      // inner shareable
        PT_MEM ;      // normal memory  



    //kernel mmio mappig
    UserRamMapping[0]=(unsigned long)((unsigned char*)&UserRamMapping[512]) |    // physical address
        PT_PAGE |     // it has the "Present" flag, which must be set, and we have area in it mapped by pages
        PT_AF |       // accessed flag. Without this we're going to have a Data Abort exception
        PT_KERNEL |     // non-privileged
        PT_ISH |      // inner shareable
        PT_MEM ;      // normal memory  
     


    //bootloader identity mapping wird danahc wieder gelöscht
    UserRamMapping[1]=(unsigned long)((unsigned char*)&UserRamMapping[512]) |    // physical address
        PT_PAGE |     // it has the "Present" flag, which must be set, and we have area in it mapped by pages
        PT_AF |       // accessed flag. Without this we're going to have a Data Abort exception
        PT_KERNEL |     // non-privileged
        PT_ISH |      // inner shareable
        PT_MEM ;      // normal memory  
    //identity mapp mmio bereich vom kernel aus
    UserRamMapping[512+72]=(unsigned long)(mmoBase)|    // physical address
        PT_BLOCK |     // it has the "Present" flag, which must be set, and we have area in it mapped by pages
        PT_AF |       // accessed flag. Without this we're going to have a Data Abort exception
        PT_KERNEL |     // non-privileged
        PT_ISH |      // inner shareable
        PT_MEM ;      // normal memory 


    KernelRamMapping[512+72]=(unsigned long)(mmoBase)|    // physical address
        PT_BLOCK |     // it has the "Present" flag, which must be set, and we have area in it mapped by pages
        PT_AF |       // accessed flag. Without this we're going to have a Data Abort exception
        PT_KERNEL |     // non-privileged
        PT_ISH |      // inner shareable
        PT_MEM ;      // normal memory


    //dma auf pageing seiten
    KernelRamMapping[512+512+48]=(unsigned long)(0x46000000)|    // physical address
        PT_BLOCK |     // it has the "Present" flag, which must be set, and we have area in it mapped by pages
        PT_AF |       // accessed flag. Without this we're going to have a Data Abort exception
        PT_KERNEL |     // non-privileged
        PT_ISH |      // inner shareable
        PT_MEM ;      // normal memory 
    
    // check for 4k granule and at least 36 bits physical address bus */
    asm volatile ("mrs %0, id_aa64mmfr0_el1" : "=r" (r));
    b=r&0xF;
    if(r&(0xF<<28)/*4k*/ || b<1/*36 bits*/) {
        return;
    }

    // first, set Memory Attributes array, indexed by PT_MEM, PT_DEV, PT_NC in our example
    r=  (0xFF << 0) |    // AttrIdx=0: normal, IWBWA, OWBWA, NTR
        (0x04 << 8) |    // AttrIdx=1: device, nGnRE (must be OSH too)
        (0x44 <<16);     // AttrIdx=2: non cacheable
    asm volatile ("msr mair_el1, %0" : : "r" (r));

    // next, specify mapping characteristics in translate control register
    r=  (0b00LL << 37) | // TBI=0, no tagging
        (b << 32) |      // IPS=autodetected
        (0b10LL << 30) | // TG1=4k
        (0b11LL << 28) | // SH1=3 inner
        (0b01LL << 26) | // ORGN1=1 write back
        (0b01LL << 24) | // IRGN1=1 write back
        (0b0LL  << 23) | // EPD1 enable higher half
        (25LL   << 16) | // T1SZ=25, 3 levels (512G)
        (0b00LL << 14) | // TG0=4k
        (0b11LL << 12) | // SH0=3 inner
        (0b01LL << 10) | // ORGN0=1 write back
        (0b01LL << 8) |  // IRGN0=1 write back
        (0b0LL  << 7) |  // EPD0 enable lower half
        (25LL   << 0);   // T0SZ=25, 3 levels (512G)
    asm volatile ("msr tcr_el1, %0; isb" : : "r" (r));

    // tell the MMU where our translation tables are. TTBR_CNP bit not documented, but required
    // lower half, user space
    asm volatile ("msr ttbr0_el1, %0" : : "r" ((unsigned long)&UserRamMapping[0] + TTBR_CNP));
    // upper half, kernel space
    asm volatile ("msr ttbr1_el1, %0" : : "r" ((unsigned long)&KernelRamMapping[0] + TTBR_CNP ));

    // finally, toggle some bits in system control register to enable page translation
    asm volatile ("dsb ish; isb; mrs %0, sctlr_el1" : "=r" (r));
    r|=0xC00800;     // set mandatory reserved bits
    r&=~((1<<25) |   // clear EE, little endian translation tables
         (1<<24) |   // clear E0E
         (1<<19) |   // clear WXN
         (1<<12) |   // clear I, no instruction cache
         (1<<4) |    // clear SA0
         (1<<3) |    // clear SA
         (1<<2) |    // clear C, no cache at all
         (1<<1));    // clear A, no aligment check
    r|=  (1<<0);     // set M, enable MMU


    asm volatile ("msr sctlr_el1, %0; isb" : : "r" (r));
    //set pc to relative adress to 0 also pc -= bootloaderStartAddr ??
    //jump to kernel adress 



    memcpy((void*)(0xffffff8000000000),(void*)&_kernelIm,KernelramSize);
    BLkprint("copied kernel to 0x47000000",30);
    
    

    //adjust_registers((unsigned long) 0xffffff8000000000 - KernelramStart);// setze register richtig
    
    jump_to_address((unsigned long)0xffffff8000000000);//TODO berechne den offset richtig anhand der übersetzungslevel und Pagesize


}


unsigned long get_current_EL() {
    unsigned long  el;
    __asm__ volatile (
        "mrs %0, CurrentEL\n"    // Lade das CurrentEL-Register in el
        "lsr %0, %0, #2\n"       // Schiebe den Wert um 2 Bits nach rechts, um nur den EL-Wert zu erhalten
        : "=r" (el)              // Ausgabe: el-Register
        :                        // Keine Eingabe
        :                        // Keine Clobber-Liste
    );
    return el;
}


void jump_to_address(unsigned long address) {
    asm volatile ("br %0" :: "r" (address) : "memory");
}

unsigned long read_pc() {
    unsigned long pc;
    asm volatile ("adr %0, ." : "=r" (pc));
    return pc;
}

unsigned long read_sp() {
    unsigned long pc;
    asm volatile ("adr %0, ." : "=r" (pc));
    return pc;
}
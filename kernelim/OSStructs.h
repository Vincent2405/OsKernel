#ifndef OSSTRUCTS_H
#define OSSTRUCTS_H
#define MAXFDS

typedef struct pageTable
{
    unsigned long pt[512+512]; //l1 + l2 2mb * 512 easily extendeble 
}pageTable;

typedef struct KernelPageTable
{
    unsigned long pt[512+512+512]; //kernel needs acces to memory mapped pageTableBreich   
}KernelPageTable;


#define MAXKERNELPAGES 80
struct KernelInfo
{
    int usedPages;
    struct KernelPageTable* kernelPT;
    char KernelBitmap[(MAXKERNELPAGES/8)];
    unsigned int KernelBitmapSize;
    unsigned int initialKernelPageCount;
    unsigned long kernelStartAdress;
};
struct KernelInfo* getKernelInfo();

#define RAMSTART 0x40000000
#define RAMEND 0x4fffffff

#define PAGESIZE    (4096*512) //2mb
#define MMIO_BASE 0x9000000//TODO
// granularity
#define PT_PAGE     0b11        // 4k granule
#define PT_BLOCK    0b01        // 2M granule
// accessibility
#define PT_KERNEL   (0<<6)      // privileged, supervisor EL1 access only
#define PT_USER     (1<<6)      // unprivileged, EL0 access allowed
#define PT_RW       (0<<7)      // read-write
#define PT_RO       (1<<7)      // read-only
#define PT_AF       (1<<10)     // accessed flag
#define PT_NX       (1UL<<54)   // no execute
// shareability
#define PT_OSH      (2<<8)      // outter shareable
#define PT_ISH      (3<<8)      // inner shareable
// defined in MAIR register
#define PT_MEM      (0<<2)      // normal memory
#define PT_DEV      (1<<2)      // device MMIO
#define PT_NC       (2<<2)      // non-cachable
#define USERRAMSTART 0x40000000
#define USERRAMEND 0x46000000
#define TTBR_CNP    1

#endif
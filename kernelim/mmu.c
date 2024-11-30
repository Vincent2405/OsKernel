#include "mmu.h"
#include "utils/uart.h"
#include "memory.h"
/*FLAGS und DEFINES von https://github.com/bztsrc/raspi3-tutorial/blob/master/10_virtualmemory/mmu.c*/
#define GICD_BASE 0x8000000

volatile extern char _kernelPageTabelStart;

void disable_mmu() {
    unsigned long sctlr;
    asm volatile("mrs %0, SCTLR_EL1" : "=r"(sctlr));
    sctlr &= ~((1UL << 0) | (1UL << 2) | (1UL << 12));
    asm volatile("msr SCTLR_EL1, %0" :: "r"(sctlr));
    asm volatile("isb");
}


//TODO alignment prüfen sollte eig aligned sein weil kernepSize genau 512+512+512*8 ist
struct pageTable *ProcessPtArr=(struct pageTable *)(0x046001000 + sizeof(struct KernelPageTable)); //ptArray in der physisch reservierten page table seite +2000 weil kernel bei 0xffffff8046000000


struct pageTable *getPageTable(int pid)//returns the pageTable of a Pid
{
    return (struct pageTable*)ProcessPtArr[pid].pt;
}


void mmu_init()
{
    
}

//initialisiert die initialen kenelPages mit der kernelStartgrösse und belegt in der Bitmap diese seiten
void mmu_init2(){
//kernel space
    struct KernelInfo* ki=getKernelInfo();
    int kernelPages=ki->initialKernelPageCount;//TODO berechne mit bootloader werten
    unsigned long kernelStart=ki->kernelStartAdress;
    unsigned long pageTablesStart=ki->kernelPT;
    unsigned long *KernelRamMapping=(unsigned long*)(pageTablesStart + (unsigned long)(0xffffff8000000000));
    //belege in golbal bitmap die gemappten seiten
    //idex für den kernel=kernelStart - Ramstart / pageSize
    for (int i = 0; i < kernelPages; i++)
    {
        bit_set(getBitmap(),((int)(kernelStart-RAMSTART)/PAGESIZE )+i,1); //set 1 for used memory
    }
    
    //belege in der kernelBitmap die gemappten seiten
    for (int i = 0; i < kernelPages; i++)
    {
        bit_set(ki->KernelBitmap,i,1); //set 1 for used memory
    }

    //bit_set(FreeSpaceBitmap,(int)(kernelStart-RAMSTART)/PAGESIZE,1); //set 1 for used memory
    //set map bitmap for pageTableSpace to 1
    bit_set(getBitmap(),(int)(pageTablesStart-RAMSTART)/PAGESIZE,1);
    if ((unsigned long)(MAXKERNELPAGES*PAGESIZE) >= pageTablesStart)
    {
        //belege auch noch in lokaler bitmap die page
        bit_set(ki->KernelBitmap,pageTablesStart/PAGESIZE,1);
    }
    

    KernelRamMapping[0]=(unsigned long)((unsigned char*)&KernelRamMapping[512] - 0xffffff8000000000) | PT_PAGE | PT_AF | PT_KERNEL | PT_ISH | PT_MEM ; 
    
    KernelRamMapping[1]=(unsigned long)((unsigned char*)&KernelRamMapping[512+512]-0xffffff8000000000) | PT_PAGE |PT_AF |PT_KERNEL | PT_ISH |  PT_MEM ;
    
    for (int i = 0; i < kernelPages; i++)
    {
        KernelRamMapping[512+i]=(unsigned long)kernelStart+(unsigned long)(i*PAGESIZE)|  PT_BLOCK | PT_AF | PT_KERNEL |  PT_ISH | PT_MEM ;
    }

    KernelRamMapping[512]=(unsigned long)kernelStart|   PT_BLOCK |     PT_AF |    PT_KERNEL |  PT_ISH |  PT_MEM ;

    KernelRamMapping[512+72]=(unsigned long)(MMIO_BASE)| PT_BLOCK | PT_AF | PT_KERNEL |  PT_ISH | PT_MEM ;    
    
    KernelRamMapping[512+64]=(unsigned long)(GICD_BASE)| PT_BLOCK | PT_AF | PT_KERNEL | PT_ISH | PT_MEM ;  
    //dma auf pageing seiten
    KernelRamMapping[512+512+48]=(unsigned long)pageTablesStart| PT_BLOCK | PT_AF | PT_KERNEL | PT_ISH | PT_MEM ;      
    
    asm volatile ("msr ttbr1_el1, %0" : : "r" ((unsigned long)&KernelRamMapping[0]  - 0xffffff8000000000+ TTBR_CNP ));
    
    return;


}


#define MAXCHILDREN 10
#define NULL 0x0
#define MAXPROCESSES 10

#include "processes.h"
#include "utils/uart.h"
#include "mmu.h"
#include "utils/armMakros.h"
#include "memory.h"
#include "heap.h"
int scheduleCount=0;
int porcessCount=0;
void eret()
{
    asm volatile("eret");
}
process processList[MAXPROCESSES];
struct process *currentProcess;

void initProcessList()
{   
    for (int i = 0; i < MAXPROCESSES; i++)
    {
        processList[i].ppid=-1;
        processList[i].state=off;
    }
}

//prints the process list
void printProcesses()
{
  for (int i = 0; i < porcessCount; i++)
  {
    kprint((&processList[i])->name,32);
    kprint("\n",1);
    kprint("pid: ",3);
    uart_print_hex(processList[i].pid);
    kprint("pageTableStartAddr: ",20);
    uart_print_hex((unsigned long)processList[i].page_table_base);
    kprint("state: ",20);
    uart_print_hex((unsigned long)processList[i].state);
    
  }
}

process* initProcess(char name[32],unsigned long segmentSize,unsigned long heapsize,unsigned long stackSize) //initial sizes
{  
    

    process* p=&(processList[porcessCount]);
    p->pid=porcessCount;
    p->page_table_base = getPageTable(porcessCount);
    beforSwitch();
    asm volatile ("msr ttbr0_el1, %0" : : "r" ((unsigned long)p->page_table_base + (unsigned long)TTBR_CNP));
    afterSwitch();
    currentProcess=&(processList[porcessCount]);
    p->processBitmapSize=(MAXPROZESSPAGES/8);
    for (int i = 0; i < p->processBitmapSize; i++)
    {
        p->processBitmap[i]=0x0;
    }
    //init fd Table
    for (int i = 0; i < MAXUSERFDS; i++)
    {
        p->fdTable[i]=-1;//unitialisied fds
    }
    p->fdTable[0]=0;//stdin
    p->fdTable[1]=1;//stdout
    p->fdTable[2]=2;//stderr

    
    //kprint("USERPTBASE: ",10);
    //uart_print_hex((unsigned long)p->page_table_base);
    //calculate needed size in bytes for initial size
    p->segmentSize=segmentSize;
    p->heap_size =heapsize;
    p->stackSize=stackSize;
    strncpy(p->name,name,32); // copy übergebenen name in p.name

    unsigned long initialProgrammSize=segmentSize+heapsize+stackSize;//setze stack ans ende vom programm

    int initialPageCount=(initialProgrammSize) / PAGESIZE +1; //aufegrundet 

    void* addr=Usermmap(0x0,initialProgrammSize,0,0,0,0);

    p->startArdess=addr;
    p->ProcessCpuContex.ELR=0x0;
    p->ProcessCpuContex.SP=segmentSize+heapsize+stackSize;

    //process* processZeroAddr=kmmap(NULL,initialProgrammSize, 0, 0,0,0,&processList[porcessCount]); //maps process to userspace memory
    porcessCount ++;
    //uart_print_hex(p->page_table_base);
    p->state=scheduled;
    //printProcesses(p);
    //currentProcess=NULL;
    return p;

}
/* fügt eine pageTable inzu und belegt den bipmap slot an der entsprechenden stelle*/
//alreadyUsedPages gibt im prinzip den offset an der

//adds new kernel Page in free Space und fügt besetzt in der bimap speicher



int addProcessPageTable(process* p,int count,unsigned char* bitmap,unsigned int bitmap_size_in_bytes)
{
    if(p->UsedPages+count > 512)
    {
        kprint("maxAmount",10);
        return -1;//task would have more than the max amount of ram //1GB

    }
    //berechne physische adresse des processes mit bitmap z.b.
    //suche nach freiem bitmap index 
    int pid=p->pid;
    
    struct pageTable* t = p->page_table_base;
    

    kprint("p.ptBase",10);
    uart_print_hex(t);

    int nextPage=p->UsedPages;

    for (int i = 0; i < count; i++)
    {
        addPagetoPageTable(t,nextPage+i,bitmap,bitmap_size_in_bytes,0,0);
    }
    //update tableCount
    p->UsedPages=p->UsedPages+count;

    return 0;
}


int freePageTable(struct pageTable* pagetable,int index,unsigned char* bitmap,unsigned int bitmap_size_in_bytes)
{
    int l1Index= (index) / 512 ; //abgerudent, da start bei 0
    int l2Index= (index);


    //entferne bitmap eintrag
    pagetable->pt[512+l2Index]=(unsigned long)(0x0);
    //TODO eigentlich schauen ob dies die letzte table im l1 index ist wenn ja auch l1 removen
}


//simple scheduler implementation


process *getProcessByPid(int pid)
{
    return &processList[pid];
}


void start_curr_process() {
    
    currentProcess->state=active;
    unsigned long spsr = 0; // EL0 mit aktiviertem User-Modus
    swapPageTable(currentProcess->page_table_base);
    asm volatile("msr elr_el1, %0" :: "r" (currentProcess->ProcessCpuContex.ELR));
    asm volatile("msr sp_el0, %0" :: "r" (currentProcess->ProcessCpuContex.SP));
    asm volatile("eret");
}

//just starts userprogramm1 TODO geschreites scheduling implementieren
int initScheduler()
{
    currentProcess=&processList[1];
}

process* getCurrentProcess()
{
    return currentProcess;
}

int scheduleNextTask()
{   
    kprint("scheduler aktiviert",20);
    scheduleCount++;
    saveCurrCpuStateToProcess(&currentProcess->ProcessCpuContex);
    
    //TODO besseres scheduling
    process* nextProcess=NULL;
    for (int i = 0; i < MAXPROCESSES; i++)
    {
        if(processList[i].state==scheduled)
        {
            nextProcess=&processList[i];
        }
    }
    if (nextProcess==NULL)
    {
        kprint("[kein neuer process gescheduled]\n" ,100);
        return -1;
    }
    kprint("schedulint PROCESS WITH PID: ",26);
    uart_print_hex(nextProcess->pid);
    
    currentProcess->state=scheduled; //stelle dan aktuelle process auf scheduled
    
    loadCpuContextFromProcess(&(nextProcess->ProcessCpuContex));

    kprint("scheduled new Process\n",25);
    nextProcess->state=active;
    //set page table to current
    
    swapPageTable(nextProcess->page_table_base);
    
    currentProcess=nextProcess;
    //currentProcess wird wieder in cpu geladen nach bl exec im irq handler
    return;
}

void swapPageTable(pageTable* pageTable)
{
    beforSwitch();
    asm volatile ("msr ttbr0_el1, %0" : : "r" ((unsigned long)pageTable + (unsigned long)TTBR_CNP));
    afterSwitch();
}

void beforSwitch()
{
    asm volatile("DSB SY \n");
}
void afterSwitch()
{
    asm volatile(
        "DSB SY \n"
        "ISB\n"
        "DSB SY \n"
        "ISB\n"
        "mov x0 , #0\n"
    );
    asm volatile ("tlbi vmalle1is");//invalidate PT
    asm volatile ("dsb ishst;tlbi vae1, x0; isb ;dsb ish; isb;");//invalidate PT IDK was das alles macht steht in docs
}

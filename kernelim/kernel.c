#include "utils/uart.h"
#include "utils/sprintf.h"
#include "processes.h"
#include "utils/armMakros.h"
#include "memory.h"
#include "heap.h"
#include "utils/sprintf.h"
#include "filesys.h"
#include "utils/fd.h"
#include "utils/armMakros.h"
#include "utils/cpuContext.h"

extern volatile unsigned char _kernelPageTabelStart;
extern volatile unsigned char _UserAppImStart;
extern volatile unsigned char _UserAppImEnd;
extern volatile unsigned char _UserAppImStart2;
extern volatile unsigned char _UserAppImEnd2;
extern volatile unsigned char _end;
void initGic(void);

struct KernelInfo kernel;

struct KernelInfo* getKernelInfo()
{
    return &kernel;
}

void initKernel()
{
    kernel.kernelPT=(struct pageTable*)0x46000000;
    kernel.kernelStartAdress=(unsigned long)0x47000000;

    kernel.usedPages=5;
    kernel.KernelBitmapSize=(MAXKERNELPAGES/8);
    for (int i = 0; i < kernel.KernelBitmapSize; i++)
    {
        kernel.KernelBitmap[i]=0x0;
    }
    kernel.initialKernelPageCount=10;
    initGlbalfdTabble();
    //kernel.initialKernelPageCount=(unsigned long)((((unsigned long)&_end+(unsigned long)PAGESIZE-1) / (unsigned long)PAGESIZE));//calczlates the size of the kernel initial in PAGES
}


int kernelMain()
{   
    
    //remove current user mapping
    
    asm volatile ("msr ttbr0_el1, %0" : : "r" (0xfffffffffff));
    kprint("addr of cpucontext:",20);
    uart_print_hex(getCurrentCpuContext());
    //init kernel strct
    initKernel();
    mmu_init2();
    initProcessList();

    initGic();
    
    iniHeap(getKernelHeap());
    
    //initFreeSpaceBitmap();
    Inode* root=initFileSystem();
    
    unsigned int stdinBuffSize=100;
    unsigned int stdoutBuffSize=100;    

    printf("printe über stdOut\n",0,0,0,0);
    printf("printe über stdOut2\n",0,0,0,0);

    //kleines filesystem
    Inode* Sub =AddSubDirectory(getFileRoot(),"test2Dir");
        Inode* testDir=AddSubDirectory(Sub,"testDir");
            addFile(testDir,"1",100);
            addFile(testDir,"2",100);
            addFile(testDir,"3",100);

        addFile(Sub,"file1",20);
        addFile(Sub,"file2",10);
        addFile(Sub,"file3",10);
    
    Inode* Sub2 =AddSubDirectory(getFileRoot(),"test3Dir");
    
    printFileSystem2(getFileRoot(),0);

    //copy testwise UserProgramm to process1
    char processName[32]="bash";
    process* process1=initProcess(processName,&_UserAppImEnd-&_UserAppImStart,0, 0xf000);
    //copys userApp1 to process1 location TODO bug fixen eigentlich sollte jeder process getrennt in sienm Userpsace auf 0x0 laufen
    swapPageTable(process1->page_table_base);
    memcpy(process1->startArdess,(void*)((unsigned long)0xffffff8000000000+&_UserAppImStart),(int)(&_UserAppImEnd-&_UserAppImStart)+0x10); 

    char processName2[32]="process2";
    process* process2=initProcess(processName2,&_UserAppImEnd2-&_UserAppImStart2,0, 0xf000);
    //copys userApp2 to process1 location
    swapPageTable(process2->page_table_base);
    memcpy(process1->startArdess,(void*)((unsigned long)0xffffff8000000000+&_UserAppImStart2),(int)(&_UserAppImEnd2-&_UserAppImStart2)+0x10);
    //uart_print_hex(&_UserAppImEnd-&_UserAppImStart);
    
    initScheduler();//initialises scheduler
    start_curr_process();//cold start current process
    
    //should not return
    while (1){}
    return 0;
    
}

//initialisert GIC mit timer 
void initGic(void) {
    asm volatile (
        // GIC Distributor Initialisierung
        "LDR X0, =0x8000000           \n" // GICDBASE
        "LDR X1, =0xffffff8000000000  \n" // Convert to kernel address
        "ADD X0, X1, X0               \n" // In X0: Kernel address of GICD_BASE
        
        "LDR X1, [X0, #0x000]         \n" // Lese GICD_CTLR
        "ORR X1, X1, #1               \n" // Setze Enable-Bit
        "STR X1, [X0, #0x000]         \n" // Schreibe zurück in GICD_CTLR
        
        "STR XZR, [X0, #0x80]        \n" // Setze GICD_IGROUPRn
        
        "LDR X1, =0xFFFFFFFF          \n" // Alle Interrupts freischalten
        "STR X1, [X0, #0x100]         \n" // GICD_ISENABLERn

        // CPU Interface Initialisierung
        "LDR X0, =0x8010000           \n" // GICCBASE
        "LDR X1, =0xffffff8000000000  \n" // Convert to kernel address
        "ADD X0, X1, X0               \n" // In X0: Kernel address of GICC_BASE
        
        "MOV X1, #1                   \n" // Aktivieren
        "STR X1, [X0, #0x000]         \n" // GICC_CTLR
        
        "MOV X1, #0xFF                \n" // Alle Prioritäten erlauben
        "STR X1, [X0, #0x004]         \n" // GICC_PMR

        // Timer Initialisierung
        "MRS X0, CNTFRQ_EL0           \n" // Lese Timer-Frequenz (optional)
        "MOV X0, #10000               \n" // 10 ms Intervall
        "MSR CNTV_TVAL_EL0, X0        \n" // Timer-Intervall setzen
        
        "MOV X0, #1                   \n" // Enable Timer
        "MSR CNTV_CTL_EL0, X0         \n"

        // Timer Interrupt aktivieren (IRQ 27)
        "LDR X0, =0x8000000           \n" // GICDBASE
        "LDR X1, =0xffffff8000000000  \n" // Convert to kernel address
        "ADD X0, X1, X0               \n" // In X0: Kernel address of GICD_BASE
        "MOV X1, #(1 << 27)           \n" // Bit für IRQ 27
        "STR X1, [X0, #0x100]         \n" // GICD_ISENABLERn

        // IRQs global freigeben
        "MSR DAIFClr, #0b0000         \n" // IRQ-Freigabe
        :
        : /* keine Eingaben */
        : "x0", "x1", "memory"        // Geänderte Register und Speicher kennzeichnen
    );
}


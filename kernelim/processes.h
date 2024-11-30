#ifndef PROCESSES_H
#define PROCESSES_H
#include "utils/cpuContext.h"
#include "OSStructs.h"

#define MAXCHILDREN 10
#define MAXPROZESSPAGES (10*8)//testweise 
#define MAXUSERFDS 5

#define scheduled 2
#define off 0
#define active 1

typedef struct process {
    int pid;
    int ppid; //parent Prcess ID
    char name[32];
    
    int state;
    int priority;
    int flags;

    struct pageTable* page_table_base;
    unsigned long* stack_pointer;
    unsigned long* heap_base;
    
    int fdTable[MAXUSERFDS];
    
    unsigned int segmentSize;
    unsigned int heap_size;
    unsigned int stackSize;
    

    int UsedPages;
    unsigned long* startArdess;
    unsigned long* text_segment;  // Zeiger auf das .text-Segment
    unsigned long* data_segment;  // Zeiger auf das .data-Segment
    unsigned long* bss_segment;   // Zeiger auf das .bss-Segments
    
    CPUContext ProcessCpuContex;
    unsigned long sp;
    unsigned long pc;
    unsigned long cpsr;

    unsigned long start_time;
    unsigned long cpu_time;
    unsigned long time_slice;
    char processBitmap[MAXPROZESSPAGES/8];
    unsigned int processBitmapSize;

    struct process* children[MAXCHILDREN];
    struct process* parent;

    int exit_code;
} process;

process* initProcess(char name[32],unsigned long segmentSize,unsigned long heapsize,unsigned long stackSize); //initial sizes
void printProcesses();
int initScheduler();
void eret();
process* getCurrentProcess();
int addProcessPageTable(process* p,int count,unsigned char* bitmap,unsigned int bitmap_size_in_bytes);
void initProcessList();
int scheduleNextTask();
void beforSwitch();
void afterSwitch();
void start_curr_process();
void swapPageTable(pageTable* pageTable);
#endif 
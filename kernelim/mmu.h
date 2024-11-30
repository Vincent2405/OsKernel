#include "OSStructs.h"

void mmu_init2();
struct pageTable *getPageTable(int pid);//returns the pageTable of a Pid
void disable_mmu();
#include "heap.h"
#include "syscalls.h"
#define PAGESIZE 1024*1024*2 //2mb

#define NULL 0x0

unsigned long getpagesize()
{
    return (unsigned long)PAGESIZE;
}


HeapInfo UserHeap;//der user heap

//initialisiert x pages vom betriebsystem
void* heapInitPage(int pages,HeapInfo* heap)
{
    unsigned long page_size = getpagesize(); 

    void* addr;
    mmap(&addr,0x0,page_size*pages,0,0,0,0);
    
    if (addr == (void*)-1) {
        //error in mmap
        return (void*)-1;
    }
    
    heap->size+=(pages*page_size);

    return addr;
}



void* memAlloc(unsigned long requestedsize,HeapInfo* heap)
{

    //gehe freie blöcke durch und schaue ob spiecher reicht (FRIST FIT)
    
    FreeNode *current = heap->Root;

    // Suche einen passenden freien Block
    while (current != NULL) {
        if (current->size >= requestedsize + sizeof(FreeNode)) {
            // Block ist groß genug -> Teilen
            unsigned long remaining_size = current->size - requestedsize - sizeof(FreeNode);


            FreeNode *allocated_block = (FreeNode *)((unsigned long)current + remaining_size + sizeof(FreeNode));
            allocated_block->size = (unsigned long)requestedsize; 

            current->size = remaining_size;
                        
            allocated_block->MemoryStartAdress=(void *)(allocated_block + 1);
            return allocated_block->MemoryStartAdress; 
        }

        current = current->nextFreeBlock;
    }
    
    //fordere neuen speicher vom betriebsystem an und füge ihn an freeListHinzu
    
    //aligne auf PAGESIZE
    unsigned long page_size = (unsigned long)getpagesize(); // Hole die Seitengröße
    unsigned long neededSpace=(unsigned long)sizeof(FreeNode)+requestedsize;
    unsigned long neededPages = (neededSpace+page_size-1)/page_size;//hocgerundet

    void* newAddr=heapInitPage(neededPages,heap);//hole freie speicherSeiten vom betriebsystem 
    if(newAddr==NULL)//falls speicheranfrage fehlschlägt frühzeitiges return
    {
        return NULL;
    }
    FreeNode *block = (FreeNode *)newAddr;

    block->size=(neededPages*page_size)-sizeof(FreeNode);
    block->MemoryStartAdress=(unsigned long)newAddr+sizeof(FreeNode);
    addToFreeList(block,heap);
    
    //rufe nochmal mmalloc auf
    return memAlloc(requestedsize,heap);
    
}


void memFree(void* addr,HeapInfo* heap)
{
    //setze den speicher wieder in freeList ein    
    //if (!addr) return;

    // Finde den Header des Blocks
    FreeNode *block = (FreeNode *)addr - 1;
    addToFreeList(block,heap);
    //TODO füge in freispeicherliste die aneinanderleigenden blöcke wieder zusammem
}

void addToFreeList(FreeNode* block,HeapInfo* heap) {
    FreeNode* current = heap->Root;

    if (current == NULL || block < current) {
        // Neuer Block wird zur neuen Wurzel
        block->nextFreeBlock = current;
        if (current != NULL) {
            current->prevFreeBlock = block;
        }
        block->prevFreeBlock = NULL;
        heap->Root = block;
        return;
    }

    // Suche die richtige Position in der sortierten Liste
    while (current->nextFreeBlock != NULL && current->nextFreeBlock < block) {
        current = current->nextFreeBlock;
    }

    // Füge den Block ein
    block->nextFreeBlock = current->nextFreeBlock;
    if (current->nextFreeBlock != NULL) {
        current->nextFreeBlock->prevFreeBlock = block;
    }
    current->nextFreeBlock = block;
    block->prevFreeBlock = current;
}

//TO0DO
/*
void concatFreeBlocks()
{

    FreeNode* curr=(*(HI.RootPointer))->nextFreeBlock;
    while (curr!=NULL)
    {
        //wenn die adresse des prev + seine size gliech dem current block ist dann concate sie
        if ((unsigned long)curr->prevFreeBlock->MemoryStartAdress + (unsigned long)curr->prevFreeBlock->size == (unsigned long)curr)
        {
            printf("prevBlckend=%p\n",((unsigned long)(curr->prevFreeBlock->MemoryStartAdress) + (unsigned long)(curr->prevFreeBlock->size))-Offset);
            printf("curBLock at %p\n",((unsigned long)curr-Offset));
            printf("benachbarte blöcke gefunden");
        }
        curr=curr->nextFreeBlock;
    }
    
}
*/
void iniHeap(HeapInfo* heap)
{
    heap->Root=NULL;
    heap->RootPointer=&heap->Root;
    heap->size=0;
    heap->kernel=1;
}

HeapInfo* getKernelHeap()
{
    return &UserHeap;    
}

void* malloc(unsigned long requestedsize)
{
    return memAlloc(requestedsize,getKernelHeap());
}

void free(void* addr)
{
    memFree(addr,getKernelHeap());
}
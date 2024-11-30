typedef struct FreeNode{
    struct FreeNode* nextFreeBlock;
    struct FreeNode* prevFreeBlock;
    unsigned long size;//size in bytes
    unsigned long MemoryStartAdress;
}FreeNode;

typedef struct heapInfo{
    struct FreeNode** RootPointer;
    FreeNode* Root;
    unsigned long size;
    int kernel; //if 1 heap belongs to kernel else heap belongs to User
}HeapInfo;

HeapInfo* getKernelHeap();

void* malloc(unsigned long requestedsize);
void free(void* addr);

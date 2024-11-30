#include "kernel.h"
#include "utils/uart.h"
#include "memory.h"
#include "processes.h"

#define NULL 0x0

unsigned char FreeSpaceBitmap[((RAMEND-RAMSTART) / PAGESIZE +1) /8];
unsigned int BitmapSize=((RAMEND-RAMSTART) / PAGESIZE +1) /8;

int initFreeSpaceBitmap()
{
    for (int i = 0; i < BitmapSize; i++)
    {
        FreeSpaceBitmap[i]=0x0;
    }
    FreeSpaceBitmap[0]=0x1;
}

unsigned char* getBitmap()
{
    return &FreeSpaceBitmap[0];
}
unsigned int getBitmapSize()
{
    return (((RAMEND-RAMSTART) / PAGESIZE )+1)/8;
}
void memset(void* addr, unsigned char value, unsigned int length) {
    unsigned char* ptr = (unsigned char*)addr; 
    for (unsigned long i = 0; i < length; i++) {
        ptr[i] = value; // Setze jeden Speicherbereich auf den angegebenen Wert
    }
}
int find_consecutive_Zero_bits(unsigned char* array, int array_size, int x) {
    int consecutive_zeros = 0;
    int start_index = -1;

    for (int bit_index = 0; bit_index < array_size * 8; bit_index++) {
        int byte_index = bit_index / 8;
        int bit_offset = bit_index % 8;

        if ((array[byte_index] & (1 << bit_offset)) == 0) {
            if (consecutive_zeros == 0) {
                start_index = bit_index;
            }
            consecutive_zeros++;

            if (consecutive_zeros >= x) {
                return start_index;
            }
        } else {
            consecutive_zeros = 0;
            start_index = -1;
        }
    }
    //not found
    return -1;
}


void* mapMameory(void* addr,unsigned int lenght,int prot,unsigned int flags,unsigned int fd, unsigned long offset,pageTable* pagetable,unsigned char* VABitmap,unsigned int VABitmapSize,unsigned char* golobalBitmap,unsigned int globalBitmalSize,int kernelUserbit)
{
    //calculate the pages needet um die länge zu repräsentieren
    int pagesNeeded=(lenght+(int)PAGESIZE-1)/(int)PAGESIZE; //annahme PAGESIZE und lenght in bytes //hochgerundet
    kprint("pagesNeedded for ",17);
    uart_print_hex((unsigned long)pagesNeeded);
    kprint("for lengt:",10);
    uart_print_hex((unsigned long) lenght);
    //finde free zusammengängenden virutellen adressbreich
    int VAindex=find_consecutive_Zero_bits(VABitmap,VABitmapSize,pagesNeeded);
    if(VAindex==-1)
    {
        kprint("keinen Freien SPeicher gefunden in der grösse",30);
        return (void*)-1;
    }
    void* VAaddr=(void*)((unsigned long)VAindex * (unsigned long)PAGESIZE);

    kprint("foud free space mit passender seitengrösse at index",55);
    uart_print_hex((unsigned long)VAindex);
    kprint("wird übersetzt zu VA",20);
    uart_print_hex(VAaddr);
    
    //TODO errorHandling
    //setze bitmap in der VAbitmap auch an entspreichenden stellen auf 1
    for (int i = 0; i < pagesNeeded; i++)
    {
        int ret=addPagetoPageTable(pagetable,VAindex+i,golobalBitmap,globalBitmalSize,flags,kernelUserbit);
        //TODO errorhandling
        bit_set(VABitmap,VAindex+i,1);
    }
    // returns the virtual adress => baiscly l2 index shiftet by 2mb 
    
    return VAaddr; 

}

//Maps User memory for Currentprocess
void* Usermmap(void* addr,unsigned int lenght,int prot,unsigned int flags,unsigned int fd, unsigned long offset)
{
    //current process
    process* cp=getCurrentProcess();
    pageTable* cpPt=cp->page_table_base;
    kprint("CurrentProcess ist" ,20);
    uart_print_hex(cpPt);
    
    unsigned char* VABitmap=cp->processBitmap;
    unsigned int VABitmapSize=cp->processBitmapSize;
    unsigned char* globalBItmap=getBitmap();
    unsigned int globalBitmalSize=getBitmapSize();
    //TODO flags implementen
   
    void* VAadress = mapMameory(addr,lenght,prot,flags,fd, offset,cpPt,VABitmap,VABitmapSize,globalBItmap,globalBitmalSize,0);
    if (VAadress==(void*)-1)
    {
        return VAadress;//gebe einfach weiter
    }
    //TODO wie KernelMMAP
    //memset(VAadress,0,((lenght+PAGESIZE-1)/PAGESIZE)*PAGESIZE);//aufgerundet auf seitengrösse
    
    return VAadress;
}
//Maps kernel memory for Kernel 
void* Kernelmmap(void* addr,unsigned int lenght,int prot,unsigned int flags,unsigned int fd, unsigned long offset)
{
    //TODO flags verarbeiten
    //current process
    struct KernelInfo* ki=getKernelInfo();
    pageTable* KernelPt=ki->kernelPT;
    unsigned char* VABitmap=ki->KernelBitmap;
    unsigned int VABitmapSize=ki->KernelBitmapSize;
    unsigned char* globalBItmap=getBitmap();
    unsigned int globalBitmalSize=getBitmapSize();
    //TODO flags implementen
    void* VAadress = mapMameory(addr,lenght,prot,flags,fd, offset,KernelPt,VABitmap,VABitmapSize,globalBItmap,globalBitmalSize,1);//1 for kernel Page
    if (VAadress==(void*)-1)
    {
        return VAadress;//gebe einfach weiter
    }
    VAadress=VAadress+(unsigned long)0xffffff8000000000;//kernel space page
    //TODO memset (muss pagetable gewechselt werden damit VA richtig angesprichen wird)
    //memset(VAadress,0,((lenght+PAGESIZE-1)/PAGESIZE)*PAGESIZE);//aufgerundet auf seitengrösse
    return VAadress;
}

void UserUnmap(void* addr,unsigned int size)
{
    struct process* cp=getCurrentProcess();
    struct pageTable* cpPt=cp->page_table_base;
    unsigned char* VABitmap=cp->processBitmap;
    unsigned int VABitmapSize=cp->processBitmapSize;
    unsigned char* golobalBitmap=getBitmap();
    unsigned int globalBitmalSize=getBitmapSize();

    unmapMemory(addr,size,cpPt,VABitmap,VABitmapSize,golobalBitmap,globalBitmalSize);
    

}
void KernelUnmap(void* addr,unsigned int size)
{
    struct KernelInfo* ki=getKernelInfo();
    struct pageTable* KernelPt=ki->kernelPT;
    unsigned char* VABitmap=ki->KernelBitmap;
    unsigned int VABitmapSize=ki->KernelBitmapSize;
    unsigned char* golobalBitmap=getBitmap();
    unsigned int globalBitmalSize=getBitmapSize();
    unmapMemory(addr,size,KernelPt,VABitmap,VABitmapSize,golobalBitmap,globalBitmalSize);
}

//unmaps speicher an einer besitmmten adresse
void unmapMemory(void* addr,unsigned int size,struct pageTable* pagetable,unsigned char* VABitmap,unsigned int VABitmapSize,unsigned char* golobalBitmap,unsigned int globalBitmalSize)
{
    int pagesNeeded=(size+PAGESIZE-1)/PAGESIZE; //annahme PAGESIZE in bytes //hochgerundet
    //calculate index form Virtual adress:
    int VAIndex=(int)((unsigned long)addr/ (unsigned long)PAGESIZE); 
    //remove from User bitmap
    for (int i = 0; i < pagesNeeded; i++)
    {
        bit_set(VABitmap,VAIndex+i,0);

        //remove from pageTable and GlobalPageTable
        RemovePageFromPageTable(pagetable,VAIndex+i,golobalBitmap,globalBitmalSize);
    }
}



int find_next_free_bit(unsigned char *bitmap, unsigned int size_in_bytes) {
    for (int byte_index = 0; byte_index < size_in_bytes; ++byte_index) {
        if (bitmap[byte_index] != 0xFF) {  // skip full bytes
            for (int bit_index = 0; bit_index < 8; ++bit_index) {
                if ((bitmap[byte_index] & (1 << bit_index)) == 0) {  // Prüfe ob das Bit 0 ist
                    return byte_index * 8 + bit_index;  // Berechne den Bit-Index
                }
            }
        }
    }
    return -1;  // Kein freies Bit gefunden
}

void bit_set(unsigned char *bitmap, int bitindex, int value) {
    int byte_index = bitindex / 8;            // Bestimme das Byte im Array
    int bit_offset =  (bitindex % 8);      // Bestimme das Bit im Byte (MSB-first)

    if (value == 1) {
        bitmap[byte_index] |= (1 << bit_offset);   // Setze das Bit auf 1
    } else {
        bitmap[byte_index] &= ~(1 << bit_offset);  // Setze das Bit auf 0
    }
}

//add singel pageTable at VAIndex(Virtual adress Index) and adds value to global Bitmap 
int addPagetoPageTable(pageTable* pagetable,int VAIndex,unsigned char* Golbalbitmap,unsigned int Golbalbitmap_size_in_bytes,int flags,int KernelUserBit)
{
    //searches GolbalBitmap for free space
    int index=find_next_free_bit(Golbalbitmap,Golbalbitmap_size_in_bytes);
    if(index==-1)
    {
        kprint("[error no free bit]",20);
        return -1;
    }

    bit_set(FreeSpaceBitmap,index,1);//set to 1 indiziert seite als belegt
    //calculate virual idecies for l2 and l1 index
    int l1Index= (VAIndex) / 512 ; //abgerudent, da start bei 0
    int l2Index= (VAIndex);

    //calculates acutal physical adress
    unsigned long actualAdress= (unsigned long)RAMSTART + (unsigned long)index*(unsigned long)4096*512;

    //rechne in viruelle PageTable adresse um, sodass kernel darauf zugreifen kann
    unsigned long VirtualPagetableAddr=((unsigned long)pagetable + (unsigned long)0xffffff8000000000);
    pageTable* VirtualPt=(pageTable*)VirtualPagetableAddr;
    
    if(KernelUserBit==0)
    {
        unsigned long VirtualPagetableAddr=((unsigned long)pagetable + (unsigned long)0xffffff8000000000);
        pageTable* VirtualPt=(pageTable*)VirtualPagetableAddr;

        //userPT
        VirtualPt->pt[512+l2Index]=(unsigned long)(actualAdress) |  PT_BLOCK |   PT_AF |   PT_USER | PT_ISH | PT_MEM ;        

        VirtualPt->pt[l1Index]=(unsigned long)((unsigned long)&(pagetable->pt[512]))| PT_PAGE | PT_AF | PT_USER |  PT_ISH | PT_MEM ;      
    }
    else if(KernelUserBit==1)
    {
        VirtualPt->pt[512+l2Index]=(unsigned long)(actualAdress) | PT_BLOCK | PT_AF | PT_KERNEL | PT_ISH | PT_MEM ; 
        
        VirtualPt->pt[l1Index]=(unsigned long)((unsigned long)&(pagetable->pt[512]))| PT_PAGE |PT_AF | PT_KERNEL | PT_ISH | PT_MEM; 
    }
    return 0;//erfolgreich
}
//removes Singe page at VAIndex( Virtual adress index) form pagetable and removes set one bit
int RemovePageFromPageTable(pageTable* pagetable,int VAIndex,unsigned char* Golbalbitmap,unsigned int Golbalbitmap_size_in_bytes)
{
    int l1Index= (VAIndex) / 512 ; //abgerudent, da start bei 0
    int l2Index= (VAIndex);

    //just removes l2 index => l1 index verweist ja vllt noch auf andere tables
    pagetable->pt[512+VAIndex]=(unsigned long)NULL;

    //set bit at VAIndex to 0
    bit_set(Golbalbitmap,VAIndex,0);
}

void printBitlist(char* bitmap,int lenght)
{
    kprint("\nBITLIST: \n",15);
    kprint("BITLISTLengt: \n",15);
    uart_print_hex((unsigned long)lenght);
    for (int i = 0; i < (lenght); i++)
    {
        kprint("BITLIST  ",10);
        uart_print_hex(bitmap[i]);
    }
}

void* memcpy(void* dest, void* src, int n) {
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;
    for (int i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}
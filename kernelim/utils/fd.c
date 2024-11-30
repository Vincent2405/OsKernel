#include "../filesys.h"
#include "sprintf.h"
#include "../processes.h"
#include "../memory.h"
#include "uart.h"
#include "fd.h"

#define NULL 0x0


KernelFD GolbalfiledescTable[MAXFDS];
int fdCount=0;

#define MAXUSERFDS 10

void initGlbalfdTabble()
{
    for (int i = 0; i < MAXFDS; i++)
    {
        GolbalfiledescTable[i].id=-1;
        GolbalfiledescTable[i].id=NULL;
        GolbalfiledescTable[i].used=0;
        GolbalfiledescTable[i].positionOffset=0x0;
    }
    GolbalfiledescTable[0].used=1;//stdin
    GolbalfiledescTable[0].id=0;
    GolbalfiledescTable[1].used=1;//stdin
    GolbalfiledescTable[1].id=1;
    GolbalfiledescTable[2].used=1;//stdin
    GolbalfiledescTable[2].id=2;
}

int getFreeFD()
{
    for (int i = 0; i < MAXFDS; i++)
    {
        if (GolbalfiledescTable[i].used==0)
        {
            GolbalfiledescTable->used=1;
            GolbalfiledescTable->id=i;
            return i;
        }
    }
    return -1;
}

int closeGlobalFD(int globalFD)
{
    GolbalfiledescTable[globalFD].indoe=NULL;
    GolbalfiledescTable[globalFD].positionOffset=0;
    GolbalfiledescTable[globalFD].id=-1;
    GolbalfiledescTable[globalFD].used=0;
    return 0;//success
}

int createGlobalFd(Inode* file,int flags)
{
    //find unused FD
    int fd=getFreeFD();
    GolbalfiledescTable[fd].indoe=file;
    //printf("im kernel wurde fd: %d mit zeiger auf inode ID: %d" , fd, file->id,0,0);
    fdCount++;
    return fd;
}

int getFreeUserFDIndex(process* p)
{
    for (int i = 0; i < MAXFDS; i++)
    {
        if (p->fdTable[i]==-1)
        {
            return i;
        }
    }
    return -1;
}

int globalkopen(char* path,int flags)//TODO if flags = 1 erstelle file
{
    //find inode from path
    Inode* file=getInodeByPath(path);
    if(file==NULL)
    {
        printf("no such file",0,0,0,0);
        return -1;
    }
    //creates fd im kernel um die datei zu indizieren
    int globalfd = createGlobalFd(file,flags);
    // addfd to UserFd table
    return globalfd;
}
void *filememcpy(void *dest, void *src, unsigned long n) {
    char *d = dest;
    char *s = src;
    while (n--&& *s!=EOF) {
        *d++ = *s++;
    }
    return dest;
}
int kread(int globalfd,char* data,int size)
{
    if (globalfd==0)
    {
        kprint("/: ",10);
        //lese aus dem uart data puffer
        uart_getstring(data, size);
        return 0;
    }
    //copy data from Inode to data*
    KernelFD* KFD=&GolbalfiledescTable[globalfd];

    if (KFD->positionOffset + size > KFD->indoe->size)
    {
        printf("error while Reading\n",0,0,0,0);
        data[0]= -1;
        return -1;
    }
    
    unsigned long offset= KFD->positionOffset;
    filememcpy(data,(void*)(KFD->indoe->data+offset),size);    

    KFD->positionOffset+=size;

    return 0;
}



int kwrite(int globalfd,char* data,int size)
{
    
    if (globalfd==0)
    {
        kprint("[STDIN]: ",10);
        kprint(data,size);
        return 0;
    }
    if (globalfd==1)
    {
        kprint(data,size);
        return 0;
    }
    if (globalfd==2)
    {
        kprint("[STDERR]: ",10);
        kprint(data,size);
        return 0;
    }
    
    //get the kernel Entry of User fd
    KernelFD* KFD=&GolbalfiledescTable[globalfd];
    
    //check if valid
    if (KFD->positionOffset + size > KFD->indoe->size)
    {
        printf("error while writing\n",0,0,0,0);
        return -1;
    }
    
    //get offset
    unsigned long offset= KFD->positionOffset;

    //copy data to file
    memcpy(KFD->indoe->data,data,size);

    return 0;    

}

char* getDataPointer(int globalfd)
{
    KernelFD* KFD=&GolbalfiledescTable[globalfd];
    return KFD->indoe->data;
}

//TODO 
void klseek(int globalfd,unsigned long offset,int whence)
{   
    //set the positionOffset + new offset
    KernelFD* KFD=&GolbalfiledescTable[globalfd];
    if(KFD->positionOffset+ offset < KFD->indoe->size && KFD->positionOffset+ offset >=0){
        KFD->positionOffset =KFD->positionOffset+ offset;//schaue ob der offset innderhalb der fil size liegt
    }

}

KernelFD* getGlobalFDTable()
{
    return &GolbalfiledescTable;
}

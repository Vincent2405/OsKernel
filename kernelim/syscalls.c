#include "processes.h"
#include "utils/cpuContext.h"
#include "utils/uart.h"
#include "memory.h"
#include "filesys.h"
#include "utils/fd.h"
#include "utils/sprintf.h"

#define NULL 0x0
void kernelPint(char* strAddr, int size)
{
    kprint(strAddr,size);
}

//kopiert den pid wert an den entsprechenden int pointer
void getPid(int* pid)
{
    pid[0]=getCurrentProcess()->pid; 
}

void mmap(void** ret ,void* addr,unsigned int lenght,int prot,unsigned int flags,unsigned int fd, unsigned long offset)
{
    
    ret[0]=Usermmap(addr,lenght,prot,flags,fd,offset); 
}

int getdents(int fd, DirectoryEntry** direntbufferAddr, int count) {
    // Zugriff auf Userspace-Puffer
    DirectoryEntry* direntbuffer = (DirectoryEntry*)direntbufferAddr[0];
    if (direntbuffer == NULL) {
        return -1; // Fehler: ungültiger Puffer
    }

    // Hole Kernel-Dateideskriptor
    int kernelfd = getCurrentProcess()->fdTable[fd];
    KernelFD* globalFDTable = getGlobalFDTable();
    KernelFD* fdEntry = &globalFDTable[kernelfd];

    // Prüfen, ob es sich um ein Verzeichnis handelt
    if (fdEntry->indoe->type != DIRECTORY) {
        return -1; // Kein Verzeichnis
    }

    DirectoryEntry* currDirent = fdEntry->indoe->headEntryPointer;
    int i = 0;

    while (i < count && currDirent != NULL) {
        // Kopiere den aktuellen Eintrag in den Userspace-Puffer
        memcpy(&direntbuffer[i], currDirent, sizeof(DirectoryEntry));
        // Setze den nächsten Zeiger (im Userspace-Puffer)
        if (currDirent->next != NULL) {
            direntbuffer[i].next = (DirectoryEntry*)((char*)direntbuffer + (i + 1) * sizeof(DirectoryEntry));
        } else {
            direntbuffer[i].next = NULL;
        }

        currDirent = currDirent->next;
        i++; // Eintrag erfolgreich kopiert
    }

    return i; // Anzahl der kopierten Einträge

}
int open(int* retFD,char* path,int flags)
{
    int kernelFD=globalkopen(path,flags);
    if (kernelFD == -1)
    {
        retFD[0] =-1;   
        return -1;
    }
    
    //find free user fd
    int* userFDTable=getCurrentProcess()->fdTable;
    //printf("added path to KERNELFD:%d",kernelFD,0,0,0);
    for (int i = 0; i < MAXUSERFDS; i++)
    {
        if(userFDTable[i]==-1)//unused fd found
        {
            userFDTable[i]=kernelFD;
            retFD[0] =i;
            return 0;//success
        }  
    }
    retFD[0] =-1;
    return -1;//no free fd
}

int close(int fd)
{
    //TODO validate fd
    //get kernelFD
    if(fd==-1)
    {
        return 0;
    }
    int kernelFD=getCurrentProcess()->fdTable[fd];
    int ret=closeGlobalFD(kernelFD);
    //TODO error handling
    
    //gebe userfd wieder frei
    getCurrentProcess()->fdTable[fd]=-1; //setze die fd wieder frei
    return 0;
}

int mkdir(char* path, char* dirname,int flags)
{
    //TODO flags
    Inode* currDir= getInodeByPath(path);
    Inode* newDir= AddSubDirectory(currDir,dirname);
    if (newDir==NULL)
    {
        printf("[failed To add subDir %s to %s ]\n",dirname,path,0,0);
        return -1;
    }
    printf("[added subDir %s to %s ]\n",dirname,path,0,0);
    return 0;
}
int mkfile(char* path, char* filename,int size,int flags)
{
    //TODO flags
    Inode* currDir= getInodeByPath(path);
    Inode* newFile= addFile(currDir,filename,size);
    if (newFile==NULL)
    {
        printf("[failed To add file %s to %s ]\n",filename,path,0,0);
        return -1;
    }
    printf("[added file %s to %s ]\n",filename,path,0,0);
    return 0;
}


int syscallEntryPoint(unsigned long type, unsigned long esr, unsigned long elr, unsigned long spsr, unsigned long far)
{   
    struct CPUContext *CurrCpuContext=getCurrentCpuContext();
    unsigned long arg0;
    unsigned long arg1;
    unsigned long arg2;
    unsigned long arg3;
    unsigned long arg4;
    unsigned long arg5;
    unsigned long arg6;
    unsigned long arg7;
    arg0=CurrCpuContext->x0;
    arg1=CurrCpuContext->x1;
    arg2=CurrCpuContext->x2;
    arg3=CurrCpuContext->x3;
    arg4=CurrCpuContext->x4;
    arg5=CurrCpuContext->x5;
    arg6=CurrCpuContext->x6;
    arg7=CurrCpuContext->x7;

    unsigned long callindex;
    asm volatile ( "mov %0, x8"      // liest den call index
        : "=r" (callindex));
    

    if(callindex==0x1)
    {
        //mmap
        mmap(arg0,arg1,arg2,arg3,arg4,arg5,arg5);
    }    
    if(callindex==0x2)
    {
        //fork 
        kprint("syscallForkErkannt\n",15);
        //TODO copy userprocess

    }
    if(callindex==0x3)
    {   
        //kprint
        kernelPint((char*)arg0,(int)arg1);
    }
    if(callindex==0x4)
    {
        //getPid         
        getPid((int*)arg0);
        //load from cpuContext
    }
    if(callindex==0x5)
    {
        //printhex 
        uart_print_hex(arg0);
        //load from cpuContext
    }
    if(callindex==0x6)
    {
        //getdents
        getdents(arg0,arg1,arg2);//fd buffer buffsize
        //printf("direntbuffer:%s\n",((DirectoryEntry*)arg1)->name,0,0,0);
        //load from cpuContext
    }
    if(callindex==0x7)
    {
        //open
        open(arg0,arg1,arg2);//ret fd ,char* path ,int flags
        //printFileSystem2(getFileRoot()->headEntryPointer,0);

        //load from cpuContext
    }
    if(callindex == 0x8)
    { 
        //wirte
        kwrite(arg0,arg1,arg2);
    }
    if(callindex == 0x9)
    { 
        //wirte
        kread(arg0,arg1,arg2);
    }
    if(callindex == 0xa)
    { 
        //mkdri
        mkdir(arg0,arg1,arg2);//char* path //int flag
        printFileSystem2(getFileRoot()->headEntryPointer,0);
    }
    
    //TODO unlink
    if(callindex == 0xb)
    { 
        //mkdri
        mkfile(arg0,arg1,arg2,arg3);//char* path //int flag
        printFileSystem2(getFileRoot()->headEntryPointer,0);
    }

    if(callindex == 0xc)
    { 
        //mkdri
        close(arg0);
    }

    return 0;//erfolgreicher syscall
}


#include "syscalls.h"

//copys count DIRECOTRYENTRYS to the buffer 
int getdents(int fd,DirectoryEntry** direntbuffer,int count)
{
    //svc
    asm volatile (
        "mov x8, #6\n"  
        "svc #0\n"      // svc exeption
    );
}

//creates fd and let it point to the Inode (can be file OR Dir)
int open(int* retFD,char* path,int flags)
{
    //svc
     asm volatile (
        "mov x8, #7\n"  
        "svc #0\n"      // svc exeption
    );
    
}

int read(int fd,char* data, int size)
{
    asm volatile (
        "mov x8, #9\n"  
        "svc #0\n"      // svc exeption
    );
}

int write(int fd, char* data,int size)
{
    asm volatile (
        "mov x8, #8\n"  
        "svc #0\n"      // svc exeption
    );
}

int mkdir(char* path,char* dirname,int flags)
{
    asm volatile (
        "mov x8, #10\n"  
        "svc #0\n"      // svc exeption
    );
}

int mkfile(char* path, char* filename,int size,int flags)
{
    asm volatile (
        "mov x8, #11\n"  
        "svc #0\n"      // svc exeption
    );
}

void mmap(void** ret ,void* addr,unsigned int lenght,int prot,unsigned int flags,unsigned int fd, unsigned long offset)
{
    asm volatile (
        "mov x8, #1\n"  
        "svc #0\n"      // svc exeption
    );
}
void printHex(unsigned long addr)
{
    asm volatile (
        "mov x8, #5\n"  
        "svc #0\n"      
    );
    
}
unsigned long getPid(int* pid)
{
    asm volatile (
        "mov x8, #4\n" 
        "svc #0\n"      
    );
};
void print(char* addr, int size)
{
    asm volatile (
        "mov x8, #3\n"  
        "svc #0\n"      
    );
    
}

void close(int fd)
{
    asm volatile (
        "mov x8, #12\n"  
        "svc #0\n"     
    );
}
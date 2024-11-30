#ifndef FD_H
#define FD_H
#include "../filesys.h"
#define MAXFDS 10
#define EOF 0xFF

typedef struct KernelFD
{
    int id;
    Inode* indoe;
    int used;
    unsigned long positionOffset;

}KernelFD;
int globalkopen(char* path,int flags);

int kwrite(int fd,char* data,int size);
void klseek(int fd,unsigned long offset,int whence);
int kread(int fd,char* data,int size);
int createGlobalFd(Inode* file,int flags);
char* getDataPointer(int globalfd);
int closeGlobalFD(int globalFD);
KernelFD* getGlobalFDTable();
void initGlbalfdTabble();
#endif
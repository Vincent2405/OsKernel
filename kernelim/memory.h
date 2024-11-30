#ifndef MEMORY_H
#define MEMORY_H

#include "OSStructs.h"
unsigned char* getBitmap();
unsigned int getBitmapSize();
int find_next_free_bit(unsigned char *bitmap, unsigned int size_in_bytes);
void bit_set(unsigned char *bitmap, int bitindex, int value);
int RemovePageFromPageTable(pageTable*,int VAIndex,unsigned char* Golbalbitmap,unsigned int Golbalbitmap_size_in_bytes);
void unmapMemory(void* addr,unsigned int size,pageTable*,unsigned char* VABitmap,unsigned int VABitmapSize,unsigned char* golobalBitmap,unsigned int globalBitmalSize);
void* Kernelmmap(void* addr,unsigned int lenght,int prot,unsigned int flags,unsigned int fd, unsigned long offset);
void* Usermmap(void* addr,unsigned int lenght,int prot,unsigned int flags,unsigned int fd, unsigned long offset);
void printBitlist(char* bitmap,int lenght);
int addPagetoPageTable(pageTable* pagetable,int VAIndex,unsigned char* Golbalbitmap,unsigned int Golbalbitmap_size_in_bytes,int flags,int KernelUserBit);
void* memcpy(void* dest, void* src, int n);
void memset(void* addr, unsigned char value, unsigned int length);
int initFreeSpaceBitmap();
#endif 
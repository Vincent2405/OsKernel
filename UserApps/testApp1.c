//#include "stdLib/syscalls.h"
//#include "stdLib/heap.h"
#include "bash.h"
#include "stdLib/syscalls.h";
#include "stdLib/heap.h";
#define NULL 0x0

int testFunc(int x, int y);

int main1()
{
    //move values in regs to test stack safe
    asm volatile("mov x0, #0");
    asm volatile("mov x1, #1");
    asm volatile("mov x2, #2");
    asm volatile("mov x3, #3");
    asm volatile("mov x4, #4");
    asm volatile("mov x5, #5");
    asm volatile("mov x6, #6");
    asm volatile("mov x7, #7");
    asm volatile("mov x8, #8");
    asm volatile("mov x9, #9");
    asm volatile("mov x10, #10");
    asm volatile("mov x11, #11");
    asm volatile("mov x12, #12");
    asm volatile("mov x13, #13");
    asm volatile("mov x14, #14");
    asm volatile("mov x15, #15");
    asm volatile("mov x16, #16");
    asm volatile("mov x17, #17");
    asm volatile("mov x18, #18");
    asm volatile("mov x19, #19");
    asm volatile("mov x20, #20");
    asm volatile("mov x21, #21");
    asm volatile("mov x22, #22");
    asm volatile("mov x23, #23");
    asm volatile("mov x24, #24");
    asm volatile("mov x25, #25");
    asm volatile("mov x26, #26");
    asm volatile("mov x27, #27");
    asm volatile("mov x28, 0xff");
    asm volatile("mov x29, #29");
    asm volatile("mov x30, #30");

        

    
    //print(&string,30);
    //int pid = getPidWrapper();
   
    /*
    DirectoryEntry* dirBuffer=malloc(1000);
    //mmap(&dirBuffer,0x0,1000,0,0,0,0);

    //testFunc(6, 7);
    int fd;
    open(&fd,"test2/testDir",0);
    getdents(fd,&dirBuffer,5);

    DirectoryEntry* curr=dirBuffer;
    while (curr!=NULL)
    {
        
        print(curr->name,32);
        curr=curr->next;
    }
    //printHex(fd);

    //read(0,dirBuffer,100);
    //write(1,dirBuffer,10);
    
    mkdir("test2/testDir","testSubDirFromUserCall",0);
    mkfile("test2/testDir","testfile",100,0);
    
    listDirect("test2/testDir");
    */
    startBash();
    
    while (1)
    {  
        print("process1läuft\n",15);    
    }
    
    return 100;
}
int testFunc(int x, int y)
{
    return x+y;
}









#define DIRECOTRY 1
#define FILE 0
typedef struct DirectoryEntry {
    char name[128];            // Name der Datei/des Verzeichnisses
    unsigned int inode_id;     // Verweis auf die Inode
    struct DirectoryEntry* next; // Nächster Eintrag in der Liste
    int type; //file or Direcotry
} DirectoryEntry;

int getdents(int fd,DirectoryEntry** direntbuffer,int count);
int open(int* retFD,char* path,int flags);
int read(int fd,char* data, int size);
int write(int fd, char* data,int size);
int mkdir(char* path,char* dirname,int flags);
int mkfile(char* path, char* filename,int size,int flags);
void mmap(void** ret ,void* addr,unsigned int lenght,int prot,unsigned int flags,unsigned int fd, unsigned long offset);
void printHex(unsigned long addr);
void print(char* addr, int size);
void close(int fd);
unsigned long getPid(int* pid);
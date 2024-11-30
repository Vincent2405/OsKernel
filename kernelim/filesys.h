#ifndef FILESYS_H
#define FILESYS_H
#define DIRECTORY 1
#define FILE 0
typedef struct DirectoryEntry {
    char name[128];            // Name
    unsigned int inode_id;     // Verweis auf Inode
    struct DirectoryEntry* next; // next
    int type; //file or Direcotry
} DirectoryEntry;

typedef struct Inode {
    unsigned int id;           
    unsigned int type;         
    unsigned int size;               
    DirectoryEntry* headEntryPointer; //points to dirhead in case of DIR
    int used;
    void* data; //points to data in case of File
} Inode;

// Globale Inode-Tabelle
#define MAXINODES 100

Inode* getInodeByPathAndDir(char* path,Inode* RootDir);
Inode* getInodeByPath(char* path);
Inode* initFileSystem();
void printFileSystem2(Inode* root,int space);
Inode* addFile(Inode* currDir,char* name,unsigned int size);
Inode* AddSubDirectory(Inode* currDir,char* name);
Inode* getFileRoot();
#endif
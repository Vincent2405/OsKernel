#include "utils/sprintf.h"
#include "filesys.h"
#include "heap.h"

#define NULL 0x0
//if type == DIRECTORY then the data pointer zeigt auf linked list of inodes for every directory
//if type == FILE then the data pointer zeigt auf linked list of inodes for every directory
//DirecotryEntry root;
Inode inode_table[MAXINODES]; // inodes die dann auf der disk liegen sollten


void initInodeTable()
{
    for (int i = 0; i < MAXINODES; i++)
    {
        Inode* t=&inode_table[i];
        t->data=NULL;
        t->headEntryPointer=NULL;
        t->id=0;
        t->size=0;
        t->type=0;
        t->used=0;
    }
    
}

unsigned int allocate_inode() {
    for (unsigned int i = 0; i < MAXINODES; i++) {
        if (!inode_table[i].used) { 
            inode_table[i].used = 1; //set used
            inode_table[i].id = i;  
            return i;               
        }
    }
    return -1; // Kein freier Platz
}

//frees the inode
unsigned int free_inode(int id) {
    
    Inode* inode=&inode_table[id];
    inode->headEntryPointer=NULL;
    inode->id=0;
    inode->size=0;
    inode->used=0;
    inode->type=0;
    return 0; // Kein freier Platz
}

Inode *rootDir;

Inode* AddSubDirectory(Inode* currDir,char* name)
{   
    //printf("adding new subDIr");
    //INDOE finden
    int freeInodeID= allocate_inode();
    if (freeInodeID==-1)
    {
        //printf("error bei addDir",0,0,0,0);
        return -1;
    }
    
    //printf("free inode at %d",freeInodeID);

    Inode* newDir=&inode_table[freeInodeID];
    newDir->size=0;
    newDir->type=DIRECTORY;
    newDir->id=freeInodeID;
    newDir->headEntryPointer=NULL;

    //printf("initialised new Dir");

    DirectoryEntry* dotdot= kmalloc(sizeof(DirectoryEntry));
    strncpy(dotdot->name,"..",128);
    dotdot->type=DIRECTORY;
    dotdot->next=NULL;
    dotdot->inode_id=(inode_table[currDir->id].id); //selbsteverweis

    //neuen . entry 
    DirectoryEntry* dot= kmalloc(sizeof(DirectoryEntry));
    strncpy(dot->name,".",128);
    dot->next=dotdot;
    dot->inode_id=newDir->id; //selbsteverweis
    dot->type=DIRECTORY;

    newDir->headEntryPointer=dot;
    newDir->size=2;//. und ..

    //füge neuen entry zu currEntry hinzu
    DirectoryEntry *newEntry=kmalloc(sizeof(DirectoryEntry));
    strncpy(newEntry->name, name, 128);
    newEntry->inode_id=freeInodeID;
    newEntry->type=DIRECTORY;

    newEntry->next = currDir->headEntryPointer; // Neuer Eintrag zeigt auf den aktuellen Head

    currDir->headEntryPointer = newEntry;       // Setze den neuen Eintrag als Head
    currDir->size++; 

    return newDir;
}

Inode* addFile(Inode* currDir,char* name,unsigned int size)
{
    int freeInodeID= allocate_inode();
    if (freeInodeID==-1)
    {
        //printf("error bei addfile",0,0,0,0);
        return NULL;
    }
    
    //printf("free inode at %d",freeInodeID);

    Inode* newFile=&inode_table[freeInodeID];
    newFile->size=size;
    newFile->type=FILE;
    newFile->id=freeInodeID;
    newFile->headEntryPointer=NULL;
    newFile->data=kmalloc(size);
    //printf("initialised new Dir");

    //füge neuen entry zu currEntry hinzu
    DirectoryEntry *newEntry=kmalloc(sizeof(DirectoryEntry));
    strncpy(newEntry->name, name, 128);
    newEntry->inode_id=freeInodeID;

    newEntry->next = currDir->headEntryPointer; // Neuer Eintrag zeigt auf den aktuellen Head

    currDir->headEntryPointer = newEntry;       // Setze den neuen Eintrag als Head
    currDir->size++;

    return newFile;
}



void delete_entry(DirectoryEntry** head, const char* name) {
    DirectoryEntry* current = *head;
    DirectoryEntry* prev = NULL;
    
    while (current != NULL) {
        if (strcmp2(current->name, name) == 0) {
            if (prev == NULL) {
                *head = current->next; // Entferne am Kopf der Liste
            } else {
                prev->next = current->next; // Überspringe das aktuelle Element
            }
            kfree(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}


void printDir(Inode* dir)
{
    DirectoryEntry* currEntry= dir->headEntryPointer;
    while (currEntry!= NULL)
    {
        if (inode_table[currEntry->inode_id].type==DIRECTORY)
        {
            //printf("[Entry %s ] mit INODE ID : %d und verweis auf headEntry INOde: %d \n",currEntry->name,currEntry->inode_id,inode_table[currEntry->inode_id].headEntryPointer->inode_id,0);
        }
        else{

            //printf("[Entry %s ] mit INODE ID : %d  \n",currEntry->name,currEntry->inode_id,0,0);
        }
        currEntry=currEntry->next;
    }
}
int strcmp2(const char *s1, const char *s2) {
    while (*s1 != '\0' && *s2 != '\0') {
        if (*s1 != *s2) {
            return (unsigned char)*s1 - (unsigned char)*s2;
        }
        s1++;
        s2++;
    }

    return (unsigned char)*s1 - (unsigned char)*s2;
}

int strncmp2(const char *s1, const char *s2, unsigned int n) {
    while (n > 0 && *s1 != '\0' && *s2 != '\0') {
        if (*s1 != *s2) {
            return (unsigned char)*s1 - (unsigned char)*s2;
        }
        s1++;
        s2++;
        n--;
    }

    if (n > 0) {
        return (unsigned char)*s1 - (unsigned char)*s2;
    }

    return 0; // Zeichen sind gleich bis zu `n` Zeichen
}
void printFileSystem2(Inode* root,int space) {
    DirectoryEntry* curr = root->headEntryPointer;

    while (curr != NULL) {
             
            for (int i = 0; i < space/2; i++)
            {
                printf(" ",0,0,0,0);
            
            }
            printf("└",0,0,0,0);
                
            for (int i = 0; i < space/2; i++)
            {
                printf("-",0,0,0,0);

            }
            printf("",0,0,0,0);
            printf("%s [INODE:%d]", curr->name,curr->inode_id,0,0);
            printf("\n",0,0,0,0);
            if (strncmp2(curr->name, ".", 128) != 0 && strncmp2(curr->name, "..", 128) != 0) {

                if (inode_table[curr->inode_id].type == DIRECTORY) {
                    
                    printFileSystem2(&inode_table[curr->inode_id],space+15); // gehe in Unterverzeichnis
                }
        }
        
        curr = curr->next;
    }
    
}



//gets the file from path
Inode* getInodeByPath(char* path)
{
    //arbeitet mit statischer root;
    return getInodeByPathAndDir(path,rootDir);
}

Inode* getInodeByPathAndDir(char* path, Inode* RootDir) {
    Inode* CurrDir = RootDir;

    int i = 0;

    while (path[i] != '\0') {
        int curri = 0;
        char currPathPart[128];

        while (path[i] == '/') {
            i++;
        }

        while (path[i] != '/' && path[i] != '\0') {
            if (curri < sizeof(currPathPart) - 1) {
                currPathPart[curri] = path[i];
                curri++;
            }
            i++;
        }
        currPathPart[curri] = '\0'; 

        if (currPathPart[0] == '\0') {
            continue;
        }

        DirectoryEntry* d = CurrDir->headEntryPointer;
        int found = 0;

        while (d != NULL) {
            if (strcmp2(d->name, currPathPart) == 0) {
                if (inode_table[d->inode_id].type == DIRECTORY) {
                    CurrDir = &inode_table[d->inode_id];
                    found = 1;
                    break;
                } else if (inode_table[d->inode_id].type == FILE) {
                    return &inode_table[d->inode_id];
                }
            }
            d = d->next; // Nächster Eintrag im Verzeichnis
        }

        if (!found) {
            return NULL;
        }
    }

    return CurrDir;
}

Inode* initFileSystem()
{
    initInodeTable();

    
    //grab free inode
    int IID=allocate_inode();
    rootDir=&inode_table[IID];
    rootDir->size=2;
    rootDir->type=DIRECTORY;
    rootDir->headEntryPointer=NULL;

  

    rootDir->headEntryPointer=NULL; // noch keine Entrys
    rootDir->type=DIRECTORY;
    inode_table[rootDir->id].type=DIRECTORY;
    
    return rootDir; 
    
}

Inode* getFileRoot()
{
    return rootDir;
}
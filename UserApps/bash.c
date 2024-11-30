#include "stdLib/syscalls.h"
#include "stdLib/heap.h"
#include "stdLib/string.h"

#define NULL 0x0
#define MAXARGS 20
#define MAX_ARGS 20         
#define MAX_ARG_LEN 100     
char* currPath = "";


void startBash()
{
    currPath="";
    //cmand line puffer 
    while (1)
    {
        
        
        print(currPath,100);
        int cmdLineBuffeSize=100;
        char* cmdLine=malloc(100);
        
       
        read(0,cmdLine,cmdLineBuffeSize);

        char args[MAX_ARGS][MAX_ARG_LEN];
        int argc = split_cmdline(cmdLine, args);


        char* cmd=args[0];
        if(strcmp(cmd,"ls")==0)
        {
            listDirect(currPath);
            printf("\n");
            //printf("\n");
        }
        if(strcmp(cmd,"cd")==0)
        {
            //take second argument
            changeDir(args[1]);
            //listDirect(currPath);
        }
        if(strcmp(cmd,"touch")==0)
        {
            //take second argument
            touch(currPath,args[1]);
            //listDirect(currPath);
        }
        if(strcmp(cmd,"mkdir")==0)
        {
            //take second argument
            mkDirCommand(currPath,args[1]);
            //listDirect(currPath);
        }
        if(strcmp(cmd,"cat")==0)
        {
            cat(currPath,args[1]);
        }
        //free(cmdLine);
        //verarbeite den command
    }
}

char* absPath(char* input)
{
   char *newPath;

    // Wenn der Input mit '/' beginnt, ist es ein absoluter Pfad
    if (input[0] == '/') {
        // Allokiere Speicher für den neuen absoluten Pfad
        newPath = malloc(strlen(input) + 1);
        if (!newPath) {
            //perror("malloc");
            return;
        }
        strcpy(newPath, input);
    } else {
        // Relativer Pfad: Berechne die Länge des neuen Pfads
        int currPathLength = strlen(currPath);
        int inputLength = strlen(input);

        // Speicher für neuen Pfad: Aktueller Pfad + "/" + Input + Null-Terminierung
        newPath = malloc(currPathLength + 1 + inputLength + 1);
        if (!newPath) {
            //perror("malloc");
            return;
        }

        // Kopiere aktuellen Pfad in den neuen Speicherbereich
        strcpy(newPath, currPath);

        // Füge "/" hinzu, falls nötig
        if (currPath[currPathLength - 1] != '/') {
            newPath[currPathLength] = '/';
            currPathLength++;
        }
        // Hänge den Input an
        strcpy(newPath + currPathLength, input);
    }
    return newPath;
}

int split_cmdline(const char *cmdline, char args[MAX_ARGS][MAX_ARG_LEN]) {
    int argc = 0;
    const char *ptr = cmdline;

    while (*ptr && argc < MAX_ARGS) {
        while (*ptr == ' ') {
            ptr++;
        }

        if (*ptr == '\0') break;

        int arg_len = 0;
      
        while (*ptr && *ptr != ' ' && arg_len < MAX_ARG_LEN - 1) {
            args[argc][arg_len++] = *ptr++;
        }
        
        args[argc][arg_len] = '\0';
        argc++;
    }

    return argc; // Anzahl der gefundenen args
}


int listDirect(char* path)
{
    int fd;
    open(&fd,path,0);
    int maxDir=100;
    DirectoryEntry* dirBuffer=malloc(sizeof(DirectoryEntry)*maxDir);
    getdents(fd,&dirBuffer,maxDir);
    if (dirBuffer==NULL)
    {
        printf("kaput\n");
        return -1;
    }
    
    //chek if invalid
    
    DirectoryEntry* currEntry= dirBuffer;
    while (currEntry!= NULL)
    {
        printf(currEntry->name);
        printf(" ");
        currEntry=currEntry->next;
    }
    close(fd);
    return 0;
}

//changes curentPath to valid newPath
void changeDir(const char *input) {
    char *newPath;

    newPath= absPath(input);

    int fd;
    open(&fd,newPath,0);
    if(fd==-1){
        close(fd);
        printf("no such dir or file\n");
        return -1;
    }
    int fd2;
    open(&fd2,currPath,0);
    if(fd==-1){
        close(fd);
        close(fd2);        
        printf("no such dir or file\n");
        return -1;
    }
    DirectoryEntry* dbuff = malloc(sizeof(DirectoryEntry)*10);//max 10 entrys
    //find entry 
    getdents(fd2,&dbuff,10);
    DirectoryEntry* curr=dbuff;
    while (curr!=NULL)
    {
        if(strcmp(input,curr->name)==0)
        {
            //found dir
            if(curr->type!=DIRECOTRY)
            {
                printf("no dir\n");
                close(fd);
                close(fd2);
                return -1;
            }
        }
        curr=curr->next;
    }
    close(fd);
    close(fd2);
    // Setze currPath auf den neuen Pfad
    
    currPath = newPath;


}

int cat(char* currPath,char* fileName)
{
    
    char* path =absPath(fileName);

    char* Buffer=malloc(20);
    int fd;
    open(&fd,path,0);//wenn -1 dann existiert file z.b. nicht
    if(fd==-1)
    {
        return -1;
    }
    read(fd,Buffer,20);
    printf(Buffer);
    close(fd);
}


int touch(char* currPath,char* fileName)
{
    mkfile(currPath,fileName,100,0);
}

int mkDirCommand(char* currPath,char* dirname)
{
    return mkdir(currPath,dirname,0);
}

int rmDirCommand(char* currPath)
{
    //TODO
    //return urmdir(currPath);
}

int rmFile(char* pathname)
{
    //TODO
    //return uunlink(pathname);
}

int tree()
{

}
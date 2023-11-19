#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#define MAX 1024 // max limit of Inodes

typedef struct
{
    uint32_t inode;
    uint32_t parentInode;
    char type;
    char name[32];
} Inode;

void listContents(uint32_t currentInode, Inode *iNodeList, int iNodeCount);
void changeDirectory(const char* name);
void createDirectory(const char *name);
void createFile(const char *name);







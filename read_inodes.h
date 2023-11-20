#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

typedef struct
{
    uint32_t inode;
    uint32_t parentInode;
    char type;
    char name[32];
} Inode;

void listContents(uint32_t currentInode, Inode *iNodeList, int iNodeCount);
void changeDirectory(const char* name, size_t iNodeCount, Inode *iNodeList, uint32_t *currentInode);
void createDirectory(const char* name, size_t *iNodeCount, Inode *iNodeList, uint32_t currentInode);
void createFile(const char *name, size_t *iNodeCount, Inode *iNodeList, uint32_t currentInode);






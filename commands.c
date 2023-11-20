#include "read_inodes.h"

void changeDirectory(const char* name, size_t iNodeCount, Inode *iNodeList, uint32_t *currentInode) {
    // for loop to loop thru each node
    for (size_t i = 0; i < iNodeCount; i++) {
        // check if current inode is directory type and name matches target name 
        if (iNodeList[i].type == 'd' && strcmp(iNodeList[i].name, name) == 0) {
            //if they match, update current inode to inode in directory
            *currentInode = iNodeList[i].inode;
            // print to confirm chagned dir
            printf("Changed dir to %s\n", name);
            // done since we're in the target dir
            return;
        }
    }

// if dir doesn't exits
printf("epic fail, dir not found\n"); 
    
}


// // can i make inodeNumList and inode count global variables?
void listContents(uint32_t currentInode, Inode *iNodeList, int iNodeCount) {
    // Print the current directory , one dot means itself. with inode num printed too.
    printf("%u .\n", currentInode);

    // find and print the parent directory, this is with 2 dots
    for (size_t i = 0; i < iNodeCount; i++) {
        // look for inode of current directory 
        if (iNodeList[i].inode == currentInode) {
            // print inode num of the current directory
            printf("%u ..\n", iNodeList[i].parentInode);
            // break from loop once found and printed dots and respected inode num
            break;
        }
    }

    // print all inodes in the current directory
    for (size_t i = 0; i < iNodeCount; i++) {
        // condition to check if inode is within current directory; if parent mathecs current then inode is a child of the current directory
        // also make sure that current directory doesn't print itself in its own contents
        if (iNodeList[i].parentInode == currentInode && iNodeList[i].inode != currentInode) {
            // print inode number and each file/subdir (should match canvas description exactly)
            printf("%u %s\n", iNodeList[i].inode, iNodeList[i].name);
        }
    }
}

void createDirectory(const char *name){


 // iterate thru inodeList to see if name of directory exiss, if yes, exit
 // check if inode count is 1024
 // create i node with new inodeCount and set parentInode, type, and also name
 // create file with its name as inode number
 // write into this file its . and .. inode values




}


void createFile(const char *name){


}
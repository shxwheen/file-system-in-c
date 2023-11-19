#include "read_inodes.h"

void changeDirectory(const char* name){


}
// // can i make inodeNumList and inode count global variables?
void listContents(uint32_t currentInode, Inode *iNodeList, int iNodeCount)
{
    // printf("Contents of inode %u:\n", currentInode);
    // int found = 0; // To check if any content is found

    // for (size_t i = 0; i < iNodeCount; i++) {
    //     if (iNodeList[i].parentInode == currentInode) {
    //         printf("%u %s\n", iNodeList[i].inode, iNodeList[i].name);
    //         found = 1;
    //     }
    // }

    // if (!found) {
    //     printf("No contents found for inode %u.\n", currentInode);
    // }
}




    // traverse thru inodeslsit array
    // identify which inode has teh current inode as its
    // parent and print the names of the inodes that have the current inode as its parent
    // print name and inode
    // for(int i = 0; i < sizeof(inode); i++){  
    //  // print parent nodes
    //  printf(" ", inodeNumList[i].parentINode);


    // }






void createDirectory(const char *name){


 // iterate thru inodeList to see if name of directory exiss, if yes, exit
 // check if inode count is 1024
 // create i node with new inodeCount and set parentInode, type, and also name
 // create file with its name as inode number
 // write into this file its . and .. inode values




}


void createFile(const char *name){


}
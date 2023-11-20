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
//return;
    
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


void createDirectory(const char* name, size_t *iNodeCount, Inode *iNodeList, uint32_t currentInode) {
    // error catching 

    // check if there is space for inode
    // check capacity which 1024
    if (*iNodeCount >= 1024) {
        printf("Inode limit reached. Cannot create new dir.\n");
        return;
    }

    // use for loop to check if directory name exists
    for (size_t i = 0; i < *iNodeCount; i++) {
        if (strcmp(iNodeList[i].name, name) == 0) {
            printf("dir %s already exists.\n", name);
            return;
        }
    }


    // create new inode for the directory
    // assume next inode number is the current count
    uint32_t newInode = *iNodeCount; 
     // access inode in list and set to dir type
    iNodeList[*iNodeCount].type = 'd';
    // access inode in list and set inode num
    iNodeList[*iNodeCount].inode = newInode;
    // assigns dir name to respected inode in inode array
    // subtract 1 to make space for null terminator in array
    strncpy(iNodeList[*iNodeCount].name, name, sizeof(iNodeList[*iNodeCount].name) - 1);
    // access inode in list and set parent
    iNodeList[*iNodeCount].parentInode = currentInode;
   
    // increment for next use
    (*iNodeCount)++;

    // storing name here
    char filename[32];
    // converts newInode to string and stores in filename
    sprintf(filename, "%u", newInode); 
    // open file
    FILE *file = fopen(filename, "w");
    // error catching
    if (file == NULL) {
        printf("Error creating dir file");
        return;
    }

    // write "." and ".." inode values into the file
    fprintf(file, ". %u\n.. %u\n", newInode, iNodeList[newInode].parentInode);
    // close file
    fclose(file);
    // GREAT SUCCESSSSS!!!!!!
    printf("Directory %s created.\n", name);
}


// literally same as create directory function except adjusted for file
void createFile(const char *name, size_t *iNodeCount, Inode *iNodeList, uint32_t currentInode){
    // error catching 

    // check if there is space for inode
    // check capacity which 1024
    if (*iNodeCount >= 1024) {
        printf("Inode limit reached. Cannot create new file.\n");
        return;
    }

    // use for loop to check if directory name exists
    for (size_t i = 0; i < *iNodeCount; i++) {
        if (strcmp(iNodeList[i].name, name) == 0) {
            printf("file %s already exists.\n", name);
            return;
        }
    }


    // create new inode for the file
    // assume next inode number is the current count
    uint32_t newInode = *iNodeCount; 
     // access inode in list and set to file type
    iNodeList[*iNodeCount].type = 'f';
    // access inode in list and set inode num
    iNodeList[*iNodeCount].inode = newInode;
    // assigns file name to respected inode in inode array
    // subtract 1 to make space for null terminator in array
    strncpy(iNodeList[*iNodeCount].name, name, sizeof(iNodeList[*iNodeCount].name) - 1);
    // access inode in list and set parent
    iNodeList[*iNodeCount].parentInode = currentInode;
   
    // increment for next use
    (*iNodeCount)++; 
    // storing name here
    char filename[32];
    // converts newInode # to string and stores in filename
    sprintf(filename, "%u", newInode); 
    // open file
    FILE *file = fopen(filename, "w");
    // error catching
    if (file == NULL) {
        printf("Error creating directory file");
        return;
    }

    // write "." and ".." inode values into the file
    fprintf(file, ". %u\n.. %u\n", newInode, iNodeList[newInode].parentInode);
    // close file
    fclose(file);
    // GREAT SUCCESSSSS!!!!!!
    printf("file %s created.\n", name);

}
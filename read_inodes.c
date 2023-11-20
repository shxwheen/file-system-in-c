#include "read_inodes.h"

void *checked_malloc(int len)
{
    void *p = malloc(len);
    if (p == NULL)
    {
        printf("Ran out of memory!\n");
        exit(1);
    }
    return p;
}


char *uint32_to_str(uint32_t i)
{
    // pretend to print to a string to get length
    int length = snprintf(NULL, 0, "%lu", (unsigned long)i);
    // allocate space for the actual string
    char* str = (char *)checked_malloc(length + 1);
    // print to string
    snprintf(str, length + 1, "%lu", (unsigned long)i);
    return str;
}


void loadInode(uint32_t inode, Inode *iNodeList, char type, int index) 

{
    char *filename = uint32_to_str(inode);
    FILE *file;
    char character; // Variable to store the read character


    // Open the binary file in read mode
    // "rb" stands for read binary
    file = fopen(filename, "rb"); 


    // Check if the file was opened successfully
    if (file == NULL)
    {
        perror("Error opening file");
        exit(1);
    }


    // Read characters from the file and print them as characters
    int char_count = 1;

    //uint32_t inode = 0;
    //char type = '\0';

    char prev = '\0';
    char curr = '\0';

    // flag for in/out of directory
    int inDir = 0;

    // count for "."
    //helps w parent tracking
    int dots = 0;
    //char nameBuffer[32];

    // index used for storing inodes to respected name
    int nameIndex = 0;
    // index for inode being iterated thru in the array of Inodes
    int nodeIdx = 0;
    // used for counting chars in the name, this helps with bypassing the dots that so that we can access directory
    int nameCount = 0;

    // read from file characher by character
    while (fread(&character, sizeof(char), 1, file) == 1)
    {
        // if "." then add to the dot counter
        // this counter will help keep track of Parent Inodes
        if(character == 46){
            // update "." count
            dots++;
        }
    
        if(char_count == 1)
        {

            prev = character;
            // check if directory type 
            if(type == 'd')
            {
                printf("This is a directory! %c\n", character);
                
                //flag for in directory
                inDir = 1;

            }
            else
            {
                printf("Add char: %c\n", character);
                // if dir flag is triggered, and count >= 3 (to access directories) , and valid iNode value from iNode list
                if(inDir == 1 && nodeIdx != 0 && nameCount >= 3){
                    //start appending to the name of the inode 
                    iNodeList[nodeIdx].name[nameIndex] = character;
                    nameIndex++;
                   // printf("nodeIdx: %d nameIndex: %d ",nodeIdx, nameIndex);
                }
                // increment count of name , helps determine validity of if statement 
                nameCount++;
            }
        }
        else
        {
            curr = character;
            if(prev == 0 && curr != 0)
            {
                // if printable char that is not "."
                if(!isprint(curr) && curr != 46)
                {
                    printf("INODE num: %d\n", character);
                    nameIndex = 0;
                    // if more than 2 dots
                    if(dots > 2){
                        // parent assigned value
                        (iNodeList)[character].parentInode = index;
                    }
                    // start processing inode index after 3 chars have been read (".")
                    if(nameCount >= 3){
                        // node index for accessing node array is assigned
                        // need to turn char into int
                        nodeIdx = (int)character;
                       // printf("nodeIdx 2: %d", nodeIdx);

                    }

                }

                else
                {
                    printf("Add char: %c\n", character);
                    //if in dir, node idx is valid, and >= to 3 chars (".")
                    if(inDir == 1 && nodeIdx != 0 && nameCount >= 3){
                        // add char to name of respected inode in respected name index
                        iNodeList[nodeIdx].name[nameIndex] = character;
                        // increment name idx
                        nameIndex++;
                        //printf("nodeIdx: %d nameIndex: %d ",nodeIdx, nameIndex);

                    }
                    // increment name count helps with if statements to determine validity
                    nameCount++;
                }
            }
            else if(prev != 0 && curr != 0)
            {
                if(isprint(curr))
                {
                    printf("Add char: %c\n", character);
                    //iNodeList[index].name[nameIndex++] = character;
                    // if in dir and valid inode accessed from array and chars are >= 3 (to bypass ".")
                    if(inDir == 1 && nodeIdx != 0 && nameCount >= 3){
                        // assign char to respected inode in array and respected idx in name
                        iNodeList[nodeIdx].name[nameIndex] = character;
                        // increment the name index 
                        nameIndex++;
                        //printf("nodeIdx: %d nameIndex: %d ",nodeIdx, nameIndex);
                    }
                    // increment count, helps with if statements to determine validity
                    nameCount++;

                }
            }
            else if(prev != 0 && curr == 0)
            {
                printf("Add null terminator. String is done!\n");
            }
        }
        char_count = char_count + 1;
        prev = character;
    }


    // Check for errors or end of file
    if (ferror(file))
    {
        perror("Error reading file");
        exit(1);
    }


    // Close the file
    fclose(file);
   
}


void loadInodesList(char *filename, Inode *iNodeList, size_t *iNodeCount)
{
    FILE *file;
    char character; // Variable to store the read character


    // Open the binary file in read mode
    file = fopen(filename, "rb"); // "rb" stands for read binary


    // Check if the file was opened successfully
    if (file == NULL)
    {
        perror("Error opening file");
        exit(1);
    }




    // Read characters from the file and print them as characters
    int char_count = 1;
    int index = 0;
    uint32_t inode = 0;
    char type = '\0';
    uint32_t inodeNumList[1024] = {0};
    char typeList[1024] = {'\0'};
    while (fread(&character, sizeof(char), 1, file) == 1)
    {
        if(char_count == 1 || char_count % 5 == 1)
        {
            //printf("INODE: %d\n", character);
            inode = character;
        }
        else if(character != 0)
        {




            //printf("TYPE: %c\n", character);
            type = character;
            printf("NEW FILE/DIR: %d, %c\n", inode, type);
            inodeNumList[index] = inode;
            typeList[index] = type;
            (*iNodeCount)++;
            index = index + 1;
        }
        char_count = char_count + 1;
    }


    // Check for errors or end of file
    if (ferror(file))
    {
        perror("Error reading file");
        exit(1);
    }


    // Close the file
    fclose(file);
    printf("%c", '\n');


    // requirement 3?????
    if(inodeNumList[0] != 0 || typeList[0] != 'd'){
        printf("Error, RUNNNNN");
        exit(1);
    }


    for(size_t i = 0; i < index; i++)
    {
        
        printf("NEW INODE: %d\n", inodeNumList[i]);
        loadInode(inodeNumList[i], iNodeList, typeList[i], i);
        printf("%c", '\n');
    }

    // for loop updates the types and inode #s from typelist and inodeNumList 
    // inode list that holds an array of inodes is being altered 
    for(size_t i = 0; i < index; i++){
        iNodeList[i].type = typeList[i];
        iNodeList[i].inode = inodeNumList[i];
    }
    //return 0;


}


void saveInodeList(const char *path, Inode *iNodeList, size_t iNodeCount){
    FILE *file = fopen(path,"w");
    // error catch
    if(file == NULL){
        printf("ERROR!!!!! inode list file is not valid");
        return;   
    }
    // update inode list with correct information with fprintf
    for(size_t i = 0; i<iNodeCount;i++){
        fprintf(file, "%u %u %c %s\n",iNodeList[i].inode,iNodeList[i].parentInode,iNodeList[i].type,iNodeList[i].name);
    }
    // close
    fclose(file);
}


int main(int argc, char* argv[]){
    // incorrect arg count (no dir given as input)
    if(argc != 2){
     printf("no directory given try again\n");
     return 1;
    }

    /// invalid dir name
    DIR *directory = opendir(argv[1]);
    if(directory == NULL){
        printf("invalid directory\n");
        exit(1);
    }

    //send in # of total inodes in use
    //create inode array of 1024
    //Inode dummy = {-1,0,'\0',"dummy"};
    Inode iNodeList[1024];
    size_t inodeCount = 0;
    uint32_t currentInode = 0;

    // change to specified dir
    chdir(argv[1]);
    closedir(directory);

    // load inodes into list
    loadInodesList("inodes_list", iNodeList, &inodeCount);
    // inode count
    printf("Inode Count: %zu\n", inodeCount);
    // for loop to print info
    for(int i =0; i<inodeCount;i++){
        printf("Inode: %d, Type: %c, Name: %s, Parent: %d \n",iNodeList[i].inode,iNodeList[i].type, iNodeList[i].name, iNodeList[i].parentInode);
    }

    // commands 
    // use strtok to tokenize arguments so i can access whatever comes after the commands
    char fname[50];
    while(1){
     printf("> ");
     fgets(fname,sizeof(fname), stdin);
     char* command = strtok(fname, " \n");
     if(command == NULL){
         continue;
     }
     if(strcmp(command, "ls") == 0){
         //printf("hello\n");
         listContents(currentInode,iNodeList,inodeCount);

     } else if(strcmp(command, "exit") == 0){
         break;
         exit(1);
     }
         else if(strcmp(command,"cd") == 0){
         char *dir = strtok(NULL, " \n");
         if(dir != NULL){
             // cd 
             changeDirectory(dir, inodeCount,iNodeList, &currentInode);
         }
     } else if(strcmp(command, "mkdir") == 0){
         char *dir = strtok(NULL," \n");
         if(dir != NULL){
             // mkdir 
             createDirectory(dir,&inodeCount,iNodeList,currentInode);
             saveInodeList("inodes_list",iNodeList,inodeCount);
         }
     } else if(strcmp(command, "touch") == 0){
         char *filename = strtok(NULL, " \n");
         if(filename != NULL){
            // create file (touch)
            createFile(filename,&inodeCount,iNodeList,currentInode);
            saveInodeList("inodes_list",iNodeList,inodeCount);



         }
     } else {
         printf("unknown commands\n");
     }


    }




    return 0;

}
















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
// send in pointer to first structure, go thru files that aren't directories, extract names, maybe open all dirs
// figure out how to keep track of name and parent
// for loop or if statement to update
{
    char *filename = uint32_to_str(inode);
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
    //uint32_t inode = 0;
    //char type = '\0';
    char prev = '\0';
    char curr = '\0';
    // flag for in/out of directory
    int inDir = 0;
    // count for "."
    //helps w parent tracking
    int dots = 0;

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
            if(type == 'd')
            {
                printf("This is a directory! %c\n", character);
                //flag for in directory
                inDir = 1;
            }
            else
            {
                printf("Add char: %c\n", character);
            }
        }
        else
        {
            curr = character;
            if(prev == 0 && curr != 0)
            {
                if(!isprint(curr) && curr != 46)
                {
                    printf("INODE num: %d\n", character);
                    if(dots > 2){
                        // parent assigned value
                        (iNodeList)[character].parentInode = index;
                    }

                }
                else
                {
                    printf("Add char: %c\n", character);
                }
            }
            else if(prev != 0 && curr != 0)
            {
                if(isprint(curr))
                {
                    printf("Add char: %c\n", character);
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


    for(size_t i = 0; i < index; i++){
        iNodeList[i].type = typeList[i];
        iNodeList[i].inode = inodeNumList[i];
    }
    //return 0;


}


// void saveInodeList(const char *path){
   
// }

int main(int argc, char* argv[]){
    // incorrect arg count
    // if(argc != 2){
    //  printf("no directory given");
    //  return 1;
    // }


    DIR *directory = opendir(argv[1]);
    if(directory == NULL){
        printf("invalid directory\n");
        exit(1);
    }
    Inode dummy = {-1,0,'\0',"dummy"};
    Inode iNodeList[1024] = {dummy}; // pointer to this in params
    size_t inodeCount = 0;
    uint32_t currentInode = 0;




    //send in # of total inodes in use
    //create inode array of 1024
   


    //traverse


    chdir(argv[1]);
    closedir(directory);
    //new array, initialize vals


    //DIR *directory;
    //directory = opendir(argv[1]);
    // make an if else for if it is null

    //pointers to vals
    // Inode *list = iNodeList;
    // int *icnt = inodeCount;


    // load inodes into list
    // maybe update load inode list
    loadInodesList("inodes_list", iNodeList, &inodeCount);
    printf("Inode Count: %zu\n", inodeCount);
    for(int i =0; i<inodeCount;i++){
        printf("Inode: %d, Type: %c, Parent: %d \n",iNodeList[i].inode,iNodeList[i].type, iNodeList[i].parentInode);
    }


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
             // cd logic
         }
     } else if(strcmp(command, "mkdir") == 0){
         char *dir = strtok(NULL," \n");
         if(dir != NULL){
             // mkdir logic
         }
     } else if(strcmp(command, "touch") == 0){
         char *filename = strtok(NULL, " \n");
         if(filename != NULL){


         }
     } else {
         printf("unknown commands\n");
     }


    }




    return 0;




 
    // check if directory invalid
    // struct stat dir_stat;
    // // if there is issue getting info from directory
    // // or
    // // if file path is not a directory
    // if(stat(argv[1], &dir_stat) == -1 || !S_ISDIR(dir_stat.st_mode)){
    //     printf("error, specified directory not valid\n");
    //     exit(1);
    // }
    // Read inodes_list file and load fs data


}
















#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512
#define NUM_BLOCK 5000
// struct to represent FreeBlocks
struct FreeBlock
{
    int index;
    struct FreeBlock *prevNode;
    struct FreeBlock *nextNode;
};

// struct to represent FileNode
struct FileNode
{
    char name[50];
    // for directory or file
    int isDirectory;
    // pointer to parent, siblings and children
    struct FileNode *parent;
    struct FileNode *next;
    struct FileNode *prev;
    struct FileNode *children;
    // for files only
    int size;
    int blockCount;
    int *blockPointers;
};

// global variale for freeNode
struct FreeBlock *freeBlockListHead = NULL;
// virtual disk representation
char virtualDisk[NUM_BLOCK][BLOCK_SIZE];
// root directory
struct FileNode *rootDirectory = NULL;

// initialize all the free blocks
void initializeFreeBlock()
{
    // loop through all the blocks
    for (int blockIndex = 0; blockIndex < NUM_BLOCK; blockIndex++)
    {
        // create a new free block
        struct FreeBlock *newFreeBlock = (struct FreeBlock *)malloc(sizeof(struct FreeBlock));
        // allocation failed
        if (newFreeBlock == NULL)
        {
            printf("Memory allocation failed for free block %d\n", blockIndex);
            exit(1);
        }
        newFreeBlock->index = blockIndex;
        newFreeBlock->prevNode = NULL;
        newFreeBlock->nextNode = NULL;

        // if this is the first block
        if (freeBlockListHead == NULL)
        {
            freeBlockListHead = newFreeBlock;
            newFreeBlock->prevNode = newFreeBlock;
            newFreeBlock->nextNode = newFreeBlock;
        }
        else
        {
            // insert the new block at the end of the list
            struct FreeBlock *tail = freeBlockListHead->prevNode;
            tail->nextNode = newFreeBlock;
            newFreeBlock->prevNode = tail;
            newFreeBlock->nextNode = freeBlockListHead;
            freeBlockListHead->prevNode = newFreeBlock;
        }
    }
}

// Allocate a free block (remove from head)
int allocateBlock()
{
    // if no free blocks available
    if (freeBlockListHead == NULL)
    {
        printf("Disk is full, no free blocks!\n");
        return -1;
    }

    int blockIndex = freeBlockListHead->index;

    // If only one block left
    if (freeBlockListHead->nextNode == freeBlockListHead)
    {
        free(freeBlockListHead);
        freeBlockListHead = NULL;
    }
    // More than one block
    else
    {
        struct FreeBlock *temporaryNode = freeBlockListHead;
        struct FreeBlock *nextNode = temporaryNode->nextNode;
        struct FreeBlock *prevNode = temporaryNode->prevNode;

        prevNode->nextNode = nextNode;
        nextNode->prevNode = prevNode;

        freeBlockListHead = nextNode;
        free(temporaryNode);
    }

    return blockIndex;
}

// Free a block (insert at tail)
void freeBlock(int blockIndex)
{
    struct FreeBlock *newBlock = (struct FreeBlock *)malloc(sizeof(struct FreeBlock));
    // allocation failed
    if (newBlock == NULL)
    {
        printf("Memory allocation failed while freeing block %d\n", blockIndex);
        exit(1);
    }
    newBlock->index = blockIndex;

    if (freeBlockListHead == NULL)
    {
        newBlock->nextNode = newBlock;
        newBlock->prevNode = newBlock;
        freeBlockListHead = newBlock;
        return;
    }

    struct FreeBlock *tailNode = freeBlockListHead->prevNode;
    tailNode->nextNode = newBlock;
    newBlock->prevNode = tailNode;
    newBlock->nextNode = freeBlockListHead;
    freeBlockListHead->prevNode = newBlock;
}

// Check if a file or directory with given name exists in the current directory
int nameExists(struct FileNode *currentWorkingDirectory, const char *name)
{
    if (currentWorkingDirectory->children == NULL)
        return 0;

    struct FileNode *temp = currentWorkingDirectory->children;

    // Traverse circular linked list
    do
    {
        if (strcmp(temp->name, name) == 0)
            return 1;
        temp = temp->next;
    } while (temp != currentWorkingDirectory->children);

    return 0;
}

// Function to create a directory node
struct FileNode *createDirectory(const char *DirectiryName, struct FileNode *parentNode)
{
    // allocate memory for new directory
    struct FileNode *newDir = (struct FileNode *)malloc(sizeof(struct FileNode));
    // allocation failed
    if (newDir == NULL)
    {
        printf("Memory allocation failed for directory %s\n", DirectiryName);
        exit(1);
    }
    strcpy(newDir->name, DirectiryName);
    newDir->isDirectory = 1;
    newDir->parent = parentNode;
    newDir->next = NULL;
    newDir->prev = NULL;
    newDir->children = NULL;
    newDir->size = 0;
    newDir->blockCount = 0;
    newDir->blockPointers = NULL;
    return newDir;
}

// Function to initialize the virtual file system
void initializeVFS()
{
    // initialize free blocks
    initializeFreeBlock();
    // create root directory
    rootDirectory = createDirectory("/", NULL);
}

// Insert child into parent's circular child list
void insertChild(struct FileNode **head, struct FileNode *newNode)
{
    if (*head == NULL)
    {
        *head = newNode;
        newNode->next = newNode;
        newNode->prev = newNode;
        return;
    }
    struct FileNode *tail = (*head)->prev;
    tail->next = newNode;
    newNode->prev = tail;
    newNode->next = *head;
    (*head)->prev = newNode;
}

// Create a file node
struct FileNode *createFile(const char *fileName, struct FileNode *parentNode)
{
    struct FileNode *newFile = (struct FileNode *)malloc(sizeof(struct FileNode));
    // allocation failed
    if (newFile == NULL)
    {
        printf("Memory allocation failed for file %s\n", fileName);
        exit(1);
    }
    // initialize file node
    strcpy(newFile->name, fileName);
    newFile->isDirectory = 0;
    newFile->parent = parentNode;
    newFile->next = NULL;
    newFile->prev = NULL;
    newFile->children = NULL;
    newFile->size = 0;
    newFile->blockCount = 0;
    newFile->blockPointers = NULL;
    return newFile;
}

// basic comands functions
// mkdir command
void createDirectoryCmd(struct FileNode *currentWorkingDirectory, const char *dirname)
{
    // Check for duplicate name
    if (nameExists(currentWorkingDirectory, dirname))
    {
        printf("Name already exists in current directory.\n");
        return;
    }

    struct FileNode *newDir = createDirectory(dirname, currentWorkingDirectory);
    insertChild(&currentWorkingDirectory->children, newDir);
    printf("Directory '%s' created successfully.\n", dirname);
}

// create file command
void createFileCmd(struct FileNode *currentWorkingDirectory, const char *filename)
{
    // Check for duplicate name
    if (nameExists(currentWorkingDirectory, filename))
    {
        printf("Name already exists in current directory.\n");
        return;
    }

    struct FileNode *newFile = createFile(filename, currentWorkingDirectory);
    insertChild(&currentWorkingDirectory->children, newFile);
    printf("File '%s' created successfully.\n", filename);
}

// write command
void writeFileCmd(struct FileNode *file, const char *data)
{
    // calculate number of blocks needed
    int length = strlen(data);
    int blocksNeeded = (length / BLOCK_SIZE) + 1;
    file->blockPointers = (int *)malloc(blocksNeeded * sizeof(int));
    // allocation failed
    if (file->blockPointers == NULL)
    {
        printf("Memory allocation failed for file block pointers\n");
        exit(1);
    }
    file->blockCount = blocksNeeded;
    file->size = length;

    int dataIndex = 0;
    for (int needIndex = 0; needIndex < blocksNeeded; needIndex++)
    {
        int blockIndex = allocateBlock();
        if (blockIndex == -1)
        {
            printf("Disk full while writing file.\n");
            return;
        }
        file->blockPointers[needIndex] = blockIndex;

        for (int byteIndex = 0; byteIndex < BLOCK_SIZE && data[dataIndex] != '\0'; byteIndex++)
        {
            virtualDisk[blockIndex][byteIndex] = data[dataIndex++];
        }
    }
}

// read command
void readFileCmd(struct FileNode *file)
{
    // combine all disk blocks into a single string buffer
    char *fileContent = (char *)malloc(file->size + 1);
    // allocation failed
    if (fileContent == NULL)
    {
        printf("Memory allocation failed while reading file\n");
        exit(1);
    }
    int contentIndex = 0;

    for (int blockIndex = 0; blockIndex < file->blockCount; blockIndex++)
    {
        int diskBlockIndex = file->blockPointers[blockIndex];
        for (int byteIndex = 0; byteIndex < BLOCK_SIZE && virtualDisk[diskBlockIndex][byteIndex] != '\0'; byteIndex++)
        {
            fileContent[contentIndex++] = virtualDisk[diskBlockIndex][byteIndex];
        }
    }
    fileContent[contentIndex] = '\0'; // Null-terminate
    int index = 0;
    // print content with escape sequence interpretation
    for (index = 0; fileContent[index] != '\0'; index++)
    {
        // Handle newline escape sequence
        if (fileContent[index] == '\\' && fileContent[index + 1] == 'n')
        {
            printf("\n");
            index++;
        }
        // Handle tab escape sequence
        else if (fileContent[index] == '\\' && fileContent[index + 1] == 't')
        {
            printf("\t");
            index++;
        }
        // Print normal character
        else
        {
            printf("%c", fileContent[index]);
        }
    }
    if (index == 0)
        printf("(empty)");
    printf("\n");
}

// delete command
void deleteCmd(struct FileNode *currentWorkingDirectory, struct FileNode *file)
{
    // free all allocated blocks
    for (int blockIndex = 0; blockIndex < file->blockCount; blockIndex++)
    {
        freeBlock(file->blockPointers[blockIndex]);
    }

    // remove node from circular linked list
    if (file->next == file && file->prev == file)
    {
        // only node in list
        currentWorkingDirectory->children = NULL;
    }
    else
    {
        // multiple nodes in list
        if (currentWorkingDirectory->children == file)
            currentWorkingDirectory->children = file->next; // move head if needed
        file->prev->next = file->next;
        file->next->prev = file->prev;
    }

    // free file’s allocated memory
    free(file->blockPointers);
    free(file);
}

// ls command
void listFileCmd(struct FileNode *currentWorkingDirectory)
{
    // if no children
    if (currentWorkingDirectory->children == NULL)
    {
        printf("(empty)\n");
        return;
    }

    struct FileNode *temp = currentWorkingDirectory->children;
    // traverse circular list
    do
    {
        if (temp->isDirectory)
            printf("%s/\n", temp->name);
        else
            printf("%s\n", temp->name);
        temp = temp->next;
    } while (temp != currentWorkingDirectory->children);
}

// cd command
void changeDirectoryCmd(struct FileNode **currentWorkingDirectory, const char *dirname)
{
    // handle special case for ".."
    if (strcmp(dirname, "..") == 0)
    {
        // move to parent directory
        if ((*currentWorkingDirectory)->parent != NULL)
        {
            *currentWorkingDirectory = (*currentWorkingDirectory)->parent;
        }
        return;
    }

    // search for directory name in currentWorkingDirectory->children
    struct FileNode *temp = (*currentWorkingDirectory)->children;
    if (temp == NULL)
    {
        printf("No such directory.\n");
        return;
    }
    // traverse circular list
    do
    {
        if (temp->isDirectory && strcmp(temp->name, dirname) == 0)
        {
            *currentWorkingDirectory = temp;
            printf("Moved to /%s\n", temp->name);
            return;
        }
        temp = temp->next;
    } while (temp != (*currentWorkingDirectory)->children);

    printf("Directory '%s' not found.\n", dirname);
}

// pwd command helper to print path recursively
void printPath(struct FileNode *directory)
{
    if (directory->parent == NULL)
        return;
    printPath(directory->parent);
    if (strcmp(directory->name, "/") != 0)
        printf("/%s", directory->name);
}

// pwd command
void printDirectoryCmd(struct FileNode *currentWorkingDirectory)
{
    if (currentWorkingDirectory == NULL)
        return;

    if (currentWorkingDirectory == rootDirectory)
        printf("/\n");
    else
    {
        printPath(currentWorkingDirectory);
        printf("\n");
    }
}

// df command
void displayInfoCmd()
{
    int totalBlocks = NUM_BLOCK;
    int freeBlocks = 0;
    // count free blocks
    if (freeBlockListHead != NULL)
    {
        struct FreeBlock *temp = freeBlockListHead;
        // traverse circular list
        do
        {
            freeBlocks++;
            temp = temp->nextNode;
        } while (temp != freeBlockListHead);
    }
    // calculate used blocks and usage percentage
    int usedBlocks = totalBlocks - freeBlocks;
    double usage = ((double)usedBlocks / totalBlocks) * 100.0;

    printf("Total Blocks: %d\n", totalBlocks);
    printf("Used Blocks:  %d\n", usedBlocks);
    printf("Free Blocks:  %d\n", freeBlocks);
    printf("Disk Usage:   %.2f%%\n", usage);
}

// remove directory command
void removeDirectoryCmd(struct FileNode *currentWorkingDirectory, struct FileNode *directory)
{
    // if it is actually a directory
    if (!directory->isDirectory)
    {
        printf("'%s' is not a directory.\n", directory->name);
        return;
    }

    // if directory is empty
    if (directory->children != NULL)
    {
        printf("Directory not empty. Remove files first.\n");
        return;
    }

    // remove from parent’s circular linked list
    if (directory->next == directory && directory->prev == directory)
    {
        // only node in list
        currentWorkingDirectory->children = NULL;
    }
    else
    {
        // multiple nodes
        if (currentWorkingDirectory->children == directory)
            currentWorkingDirectory->children = directory->next;
        directory->prev->next = directory->next;
        directory->next->prev = directory->prev;
    }

    // free memory
    free(directory);

    printf("Directory removed successfully.\n");
}

// helper method for shell interaction
void runShell()
{
    struct FileNode *currentDirectory = rootDirectory;
    char input[512];
    char arg1[100], arg2[512];

    printf("Virtual File System ready. Type 'exit' to quit.\n");

    while (1)
    {
        // Display prompt
        if (currentDirectory == rootDirectory)
            printf("/ > ");
        else
            printf("%s > ", currentDirectory->name);

        // Read user input
        if (fgets(input, sizeof(input), stdin) == NULL)
            continue;
        input[strcspn(input, "\n")] = '\0';

        // mkdir
        if (strncmp(input, "mkdir ", 6) == 0)
        {
            sscanf(input + 6, "%s", arg1);
            createDirectoryCmd(currentDirectory, arg1);
        }
        // create
        else if (strncmp(input, "create ", 7) == 0)
        {
            sscanf(input + 7, "%s", arg1);
            createFileCmd(currentDirectory, arg1);
        }

        // write
        else if (strncmp(input, "write ", 6) == 0)
        {
            if (sscanf(input + 6, "%s \"%[^\"]\"", arg1, arg2) == 2)
            {
                struct FileNode *temp = currentDirectory->children;
                int found = 0;
                if (temp != NULL)
                {
                    do
                    {
                        if (!temp->isDirectory && strcmp(temp->name, arg1) == 0)
                        {
                            writeFileCmd(temp, arg2);
                            printf("Data written successfully (size=%d bytes).\n", (int)strlen(arg2));
                            found = 1;
                            break;
                        }
                        temp = temp->next;
                    } while (temp != currentDirectory->children);
                }
                if (!found)
                    printf("File '%s' not found.\n", arg1);
            }
            else
            {
                printf("Usage: write <filename> \"data\"\n");
            }
        }
        // read
        else if (strncmp(input, "read ", 5) == 0)
        {
            sscanf(input + 5, "%s", arg1);
            struct FileNode *temp = currentDirectory->children;
            int found = 0;
            if (temp != NULL)
            {
                do
                {
                    if (!temp->isDirectory && strcmp(temp->name, arg1) == 0)
                    {
                        readFileCmd(temp);
                        found = 1;
                        break;
                    }
                    temp = temp->next;
                } while (temp != currentDirectory->children);
            }
            if (!found)
                printf("File '%s' not found.\n", arg1);
        }
        // delete
        else if (strncmp(input, "delete ", 7) == 0)
        {
            sscanf(input + 7, "%s", arg1);
            struct FileNode *temp = currentDirectory->children;
            int found = 0;
            if (temp != NULL)
            {
                do
                {
                    if (!temp->isDirectory && strcmp(temp->name, arg1) == 0)
                    {
                        deleteCmd(currentDirectory, temp);
                        printf("File deleted successfully.\n");
                        found = 1;
                        break;
                    }
                    temp = temp->next;
                } while (temp != currentDirectory->children);
            }
            if (!found)
                printf("File '%s' not found.\n", arg1);
        }
        // ls
        else if (strcmp(input, "ls") == 0)
        {
            listFileCmd(currentDirectory);
        }
        // cd dirname or cd ..
        else if (strncmp(input, "cd ", 3) == 0)
        {
            sscanf(input + 3, "%s", arg1);
            changeDirectoryCmd(&currentDirectory, arg1);
        }
        // pwd
        else if (strcmp(input, "pwd") == 0)
        {
            printDirectoryCmd(currentDirectory);
        }
        // df
        else if (strcmp(input, "df") == 0)
        {
            displayInfoCmd();
        }
        // rmdir
        else if (strncmp(input, "rmdir ", 6) == 0)
        {
            sscanf(input + 6, "%s", arg1);
            struct FileNode *temp = currentDirectory->children;
            while (temp != NULL && strcmp(temp->name, arg1) != 0)
            {
                temp = temp->next;
                if (temp == currentDirectory->children)
                    break;
            }
            if (temp == NULL)
                printf("Directory not found.\n");
            else
                removeDirectoryCmd(currentDirectory, temp);
        }

        // exit
        else if (strcmp(input, "exit") == 0)
        {
            printf("Memory released. Exiting program...\n");
            break;
        }
        // invalid command
        else if (strlen(input) > 0)
        {
            printf("Invalid command: %s\n", input);
        }
    }
}

int main()
{
    // initialize disk and root directory
    initializeVFS();
    // start shell interaction
    runShell();
    return 0;
}

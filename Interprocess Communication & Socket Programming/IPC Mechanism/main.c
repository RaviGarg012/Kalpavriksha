#include <stdio.h>
#include <stdlib.h>
#include "Structure.h"

void userIntraction()
{
    // display menu options to the user
    printf("Select IPC Mechanism to sort an array:\n");
    printf("1. File IPC\n");
    printf("2. Message Queue IPC\n");
    printf("3. Pipe IPC\n");
    printf("4. Shared Memory IPC\n");
    int choice;
    scanf("%d", &choice);
    // check choice
    if (choice < 1 || choice > 4)
    {
        printf("Invalid choice. Please select a valid option.\n");
        return;
    }

    // get the size of the array
    printf("Enter the size of the array: ");
    scanf("%d", &arraySize);
    // allocate memory for the array
    array = (int *)malloc(arraySize * sizeof(int));
    if (array == NULL)
    {
        printf("Memory allocation failed\n");
        return;
    }
    // get the elements of the array
    printf("Enter %d elements of the array:\n", arraySize);
    for (int index = 0; index < arraySize; index++)
    {
        scanf("%d", &array[index]);
    }

    // call the appropriate IPC mechanism based on user choice
    switch (choice)
    {
    case 1:
        fileIPC();
        break;
    case 2:
        messageQueueIPC();
        break;
    case 3:
        pipeIPC();
        break;
    case 4:
        sharedMemoryIPC();
        break;
    default:
        printf("Invalid choice\n");
        break;
    }

    // free the allocated memory
    free(array);
}

int main()
{
    // get the user input
    userIntraction();
    return 0;
}
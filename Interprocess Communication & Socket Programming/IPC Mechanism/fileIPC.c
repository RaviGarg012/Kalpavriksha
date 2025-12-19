#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "Structure.h"

void fileIPC()
{
    // display the original array by the parent process
    printf("Original Array: \n");
    displayArray();

    // create a child process
    pid_t pid = fork();
    // fork failed
    if (pid < 0)
    {
        printf("Fork failed\n");
        exit(1);
    }
    // child process
    else if (pid == 0)
    {
        // sort the array
        sortArray();
        // write the sorted array to a file
        FILE *file = fopen("sorted_array.txt", "w");
        if (file == NULL)
        {
            printf("File opening failed in child process\n");
            exit(1);
        }
        for (int index = 0; index < arraySize; index++)
        {
            fprintf(file, "%d\n", array[index]);
        }
        fclose(file);
        exit(0);
    }
    // parent process
    else
    {
        // wait for the child process to complete
        wait(NULL);
        // read the sorted array from the file
        FILE *file = fopen("sorted_array.txt", "r");
        if (file == NULL)
        {
            printf("File opening failed in parent process\n");
            return;
        }
        // read the sorted array
        for (int index = 0; index < arraySize; index++)
        {
            fscanf(file, "%d", &array[index]);
        }
        fclose(file);
        // display the sorted array
        printf("Sorted Array: \n");
        displayArray();
    }

    // remove the file after use
    remove("sorted_array.txt");
}
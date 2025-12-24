#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include "Structure.h"

// shared memory mechanism
void sharedMemoryIPC()
{
    // create shared memory
    int key = ftok("shmfile", 65);
    int shmid = shmget(key, arraySize * sizeof(int), 0666 | IPC_CREAT);

    // display original array
    printf("Original Array:\n");
    displayArray();

    pid_t pid = fork();
    if (pid < 0)
    {
        printf("Fork failed\n");
        exit(1);
    }
    // child process
    else if (pid == 0)
    {
        // attach to shared memory
        int *sharedArray = (int *)shmat(shmid, NULL, 0);

        // sort the array in shared memory
        sortArray();

        // copy data to shared memory
        for (int index = 0; index < arraySize; index++)
            sharedArray[index] = array[index];

        // detach from shared memory
        shmdt(sharedArray);
        exit(0);
    }

    // parent process
    else
    {
        // wait for child to complete
        wait(NULL);

        // attach to shared memory
        int *sharedArray = (int *)shmat(shmid, NULL, 0);

        // copy sorted data back to global array
        for (int index = 0; index < arraySize; index++)
            array[index] = sharedArray[index];

        // display sorted array
        printf("Sorted Array:\n");
        displayArray();

        // detach from shared memory
        shmdt(sharedArray);

        // destroy the shared memory
        shmctl(shmid, IPC_RMID, NULL);
    }
}
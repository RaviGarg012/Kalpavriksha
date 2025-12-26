#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "Structure.h"

// pipe mechanism
void pipeIPC()
{
    int pipe1[2], pipe2[2];

    // create pipes
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1)
    {
        printf("Pipe creation failed\n");
        exit(1);
    }

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
        // close unused ends
        close(pipe1[1]);
        close(pipe2[0]);

        // read array from parent
        read(pipe1[0], array, arraySize * sizeof(int));
        close(pipe1[0]);

        // sort array
        sortArray();

        // send sorted array back to parent
        write(pipe2[1], array, arraySize * sizeof(int));
        close(pipe2[1]);

        exit(0);
    }

    // parent process
    else
    {
        // close unused ends
        close(pipe1[0]);
        close(pipe2[1]);

        // send array to child
        write(pipe1[1], array, arraySize * sizeof(int));
        close(pipe1[1]);

        wait(NULL);

        // read sorted array from child
        read(pipe2[0], array, arraySize * sizeof(int));
        close(pipe2[0]);

        printf("Sorted Array:\n");
        displayArray();
    }
}
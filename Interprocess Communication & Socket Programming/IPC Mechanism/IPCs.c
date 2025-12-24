#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include "Structure.h"

// file mechanism
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

// message queue mechanism
void messageQueueIPC()
{
    // create message queue
    int key = ftok("msgqueue", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);
    struct messageBuffer message;

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
        // receive message from parent
        msgrcv(msgid, &message, sizeof(message.arrayData), 1, 0);

        // copy data to global array
        for (int index = 0; index < arraySize; index++)
            array[index] = message.arrayData[index];

        sortArray();
        // send sorted array back to parent
        for (int index = 0; index < arraySize; index++)
            message.arrayData[index] = array[index];

        message.messageType = 2;
        msgsnd(msgid, &message, sizeof(message.arrayData), 0);

        exit(0);
    }

    // parent process
    else
    {
        // copy data to message structure
        for (int index = 0; index < arraySize; index++)
            message.arrayData[index] = array[index];
        // send message to child
        message.messageType = 1;
        msgsnd(msgid, &message, sizeof(message.arrayData), 0);
        // wait for child to complete
        wait(NULL);
        // receive sorted array from child
        msgrcv(msgid, &message, sizeof(message.arrayData), 2, 0);
        // copy sorted data back to global array
        for (int index = 0; index < arraySize; index++)
            array[index] = message.arrayData[index];
        // display sorted array
        printf("Sorted Array:\n");
        displayArray();
        // destroy the message queue
        msgctl(msgid, IPC_RMID, NULL);
    }
}
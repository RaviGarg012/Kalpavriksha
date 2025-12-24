#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include "Structure.h"

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
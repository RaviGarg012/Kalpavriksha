#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "processStructure.h"

// user input
void userInput()
{
    // get the input details for the processes
    char command[20];
    int processID, burstTime, IOTime, IODuration, killProcessID, killTime;
    // take the input until the first command is EOF
    while (1)
    {
        // read the command
        scanf("%s", command);
        // if the command is exit, break the loop
        if (strcmp(command, "exit") == 0)
        {
            break;
        }
        // if the command is KILL, get the kill command details
        else if (strcmp(command, "KILL") == 0)
        {
            // get the kill command details
            scanf("%d %d", &killProcessID, &killTime);
            // add the kill event to the kill list
            addKillEvent(killProcessID, killTime);
        }
        else
        {
            scanf("%d %d %d %d", &processID, &burstTime, &IOTime, &IODuration);
            // create a new process control block and add it to the hash map and queue
            addProcess(command, processID, burstTime, IOTime, IODuration);
        }
    }
}

// increase the time unit by 1
void incrementTimeUnit()
{
    // sleep for 1 second
    Sleep(1000);
    // increment the time unit
    TimeUnit++;
}

// helper method to start the simulation
void simulateFCFS()
{
    // initiate running process
    ProcessControlBlock *CPUProcess = NULL;
    ProcessControlBlock *IOProcess = NULL;
    // loop until all processes are terminated
    while (TerminatedQueue->size != processMap->noOfElements)
    {
        // operation for cpu process
        // starting condition
        if (CPUProcess == NULL)
        {
            // dequeue next process
            CPUProcess = dequeue(readyQueue);
            if (CPUProcess != NULL && CPUProcess->IOTime > 0)
            {
                CPUProcess->state = 1;
                CPUProcess->IOTime += TimeUnit;
                CPUProcess->IOEndTime += CPUProcess->IOTime;
            }
        }
        // IO burst condition
        else if (CPUProcess->IOTime == TimeUnit)
        {
            CPUProcess->state = 2;
            enqueueProcess(IOWaitingQueue, CPUProcess->processID, -1);
            CPUProcess = dequeue(readyQueue);
            // dequeue next process
            if (CPUProcess != NULL && CPUProcess->IOTime > 0)
            {
                CPUProcess->state = 1;
                CPUProcess->IOTime += TimeUnit;
                CPUProcess->IOEndTime += CPUProcess->IOTime;
            }
        }
        // completion condition
        else if (CPUProcess->burstTime == 0)
        {
            CPUProcess->state = 3;
            // calculate turn around time and waiting time
            printf("Process %s completed at time %d\n", CPUProcess->processName, TimeUnit);
            CPUProcess->turnAroundTime = TimeUnit;
            CPUProcess->waitingTime += TimeUnit;
            // move to terminated queue

            enqueueProcess(TerminatedQueue, CPUProcess->processID, -1);
            // dequeue next process
            CPUProcess = dequeue(readyQueue);
            if (CPUProcess != NULL && CPUProcess->IOTime > 0)
            {
                CPUProcess->state = 1;
                CPUProcess->IOTime += TimeUnit;
                CPUProcess->IOEndTime += CPUProcess->IOTime;
            }
        }
        // decrement the burst time
        else
        {
            CPUProcess->burstTime--;
        }

        // operation for IO process
        // starting condition
        if (IOProcess == NULL)
        {
            IOProcess = dequeue(IOWaitingQueue);
            if (IOProcess != NULL)
            {
                IOProcess->state = 2;
            }
        }
        // completion condition
        else if (IOProcess->IOEndTime == TimeUnit)
        {
            IOProcess->state = 0;
            IOProcess->IOTime = -1; // IO completed
            // add back to ready queue
            enqueueProcess(readyQueue, IOProcess->processID, -1);
            // dequeue next IO process
            IOProcess = dequeue(IOWaitingQueue);
            if (IOProcess != NULL)
            {
                IOProcess->state = 2;
            }
        }

        // increment the time unit
        incrementTimeUnit();
    }
}

int main()
{
    // initialize the hash map and queue
    initializeStructures();
    // get the input from the user
    userInput();
    // simulate the FCFS scheduling
    simulateFCFS();
    // display the results
    displayResults();
    // free allocated memory
    freeMemory();
    return 0;
}
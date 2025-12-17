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
        // if the command is end, break the loop
        if (strcmp(command, "end") == 0)
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

// get the cpu prcess from the ready queue
ProcessControlBlock *getCPUProcess()
{
    ProcessControlBlock *cpu = dequeue(readyQueue);
    if (cpu != NULL && cpu->IOTime > 0)
    {
        cpu->state = 1; // Running
        if (cpu->IOTime > 0)
        {
            cpu->IOTime += TimeUnit;
            cpu->IOEndTime += cpu->IOTime;
        }
    }
    return cpu;
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
        // kill the process if any
        if (KillEventQueue->front != NULL && KillEventQueue->front->killTime == TimeUnit)
        {
            // kill the process
            killProcess(&CPUProcess, &IOProcess);
        }
        // operation for cpu process
        // starting condition
        if (CPUProcess == NULL)
        {
            // dequeue next process
            CPUProcess = getCPUProcess();
        }
        // IO burst condition
        else if (CPUProcess->IOTime == TimeUnit)
        {
            CPUProcess->state = 2;
            enqueueProcess(IOWaitingQueue, CPUProcess->processID, -1);
            CPUProcess = getCPUProcess();
        }
        // completion condition
        else if (CPUProcess->burstTime <= 0)
        {
            CPUProcess->state = 3;
            CPUProcess->turnAroundTime = TimeUnit;
            // move to terminated queue
            enqueueProcess(TerminatedQueue, CPUProcess->processID, -1);
            // dequeue next process
            CPUProcess = getCPUProcess();
        }
        // decrement the burst time
        if (CPUProcess != NULL && CPUProcess->burstTime > 0)
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
            if (CPUProcess == NULL)
            {
                CPUProcess = IOProcess;
                CPUProcess->state = 1;
                // decrement burst time for this time unit
                CPUProcess->burstTime--;
            }
            else
            {
                enqueueProcess(readyQueue, IOProcess->processID, -1);
            }
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
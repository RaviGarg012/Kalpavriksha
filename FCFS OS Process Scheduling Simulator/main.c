#include <stdio.h>
#include <string.h>
#include "processStructure.h"

// user input
void userInput()
{
    // get the input details for the processes
    char command[20];
    int processID, burstTime, IOTime, IODuration, killProcessID, killTime;
    // take the input until the first command is EOF
    while (scanf("%s", command) != EOF)
    {
        if (strcmp(command, "KILL") == 0)
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
        // read the next command
        scanf("%s", command);
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
#include <stdio.h>
#include <stdlib.h>
#include "processStructure.h"

// helper method to intialize a queue
Queue *getQueue()
{
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    if (queue == NULL)
    {
        printf("Memory allocation failed for queue.\n");
        exit(1);
    }
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
}
// function to initialize the hash map and queue
void initializeStructures()
{
    // allocate memory for hash map
    processMap = (HashMap *)malloc(sizeof(HashMap));
    // if memory allocation fails
    if (processMap == NULL)
    {
        printf("Memory allocation failed for process map.\n");
        exit(1);
    }
    processMap->noOfElements = 0;
    for (int bucketIndex = 0; bucketIndex < MAP_SIZE; bucketIndex++)
    {
        processMap->table[bucketIndex] = NULL;
    }

    // allocate memory for ready queue
    readyQueue = getQueue();

    // allocate memory for IO waiting queue
    IOWaitingQueue = getQueue();

    // allocate memory for terminated queue
    TerminatedQueue = getQueue();

    // allocate memory for kill event queue
    KillEventQueue = getQueue();
}

// hash function to get the bucket index
int getHash(int processID)
{
    return processID % MAP_SIZE;
}

// helper method to add process to hash map
void addProcessToHashMap(ProcessControlBlock *pcb)
{
    // get the bucket index
    int bucketIndex = getHash(pcb->processID);
    // insert pcb at the beginning
    pcb->next = processMap->table[bucketIndex];
    processMap->table[bucketIndex] = pcb;
    processMap->noOfElements++;
}

// helper method to add process in the queue
void enqueueProcess(Queue *queue, int processID, int killTime)
{
    // create a new queue node
    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
    if (newNode == NULL)
    {
        printf("Memory allocation failed for queue node.\n");
        exit(1);
    }
    newNode->processID = processID;
    newNode->killTime = killTime;
    newNode->next = NULL;

    // if the queue is empty
    if (queue->rear == NULL)
    {
        queue->front = queue->rear = newNode;
    }
    else
    {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
    queue->size++;
}

// helper method to add a process to the hash map and ready queue
void addProcess(char processName[], int processID, int burstTime, int IOTime, int IODuration)
{
    // create a new process control block
    ProcessControlBlock *newPCB = (ProcessControlBlock *)malloc(sizeof(ProcessControlBlock));
    if (newPCB == NULL)
    {
        printf("Memory allocation failed for process control block.\n");
        exit(1);
    }
    newPCB->processID = processID;
    strcpy(newPCB->processName, processName);
    newPCB->burstTime = burstTime;
    newPCB->IOTime = IOTime;
    newPCB->IODuration = IODuration;
    newPCB->state = 0; // Ready
    newPCB->turnAroundTime = 0;
    newPCB->next = NULL;

    // add the process to the hash map
    addProcessToHashMap(newPCB);

    // add the process to the ready queue
    enqueueProcess(readyQueue, processID, -1);
}

// helper method to add kill events
void addKillEvent(int processID, int killTime)
{
    // add process in the Kill queue
    enqueueProcess(KillEventQueue, processID, killTime);
}
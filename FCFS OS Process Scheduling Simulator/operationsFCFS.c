#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    // initialize time unit
    TimeUnit = 0;
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

// helper method to deque a node from a queue
ProcessControlBlock *dequeue(Queue *queue)
{
    if (queue->rear == NULL)
        return NULL;

    // get the front node
    QueueNode *temp = queue->front;
    ProcessControlBlock *pcb = NULL;
    // get the process control block from the hash map
    int bucketIndex = getHash(temp->processID);
    pcb = processMap->table[bucketIndex];
    while (pcb != NULL)
    {
        if (pcb->processID == temp->processID)
        {
            break;
        }
        pcb = pcb->next;
    }
    // move the front pointer to the next node
    queue->front = queue->front->next;
    // if the front becomes NULL, then change rear also as NULL
    if (queue->front == NULL)
    {
        queue->rear = NULL;
    }
    queue->size--;
    // free the memory of the dequeued node
    free(temp);
    return pcb;
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
    newPCB->waitingTime = -burstTime;
    newPCB->turnAroundTime = 0;
    newPCB->IOEndTime = IODuration;
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

// helper method to delete a prcess from a queue
int deleteProcess(Queue *queue, int processID)
{
    if (queue->front == NULL)
        return -1;

    QueueNode *curr = queue->front;
    QueueNode *prev = NULL;
    while (curr != NULL)
    {
        if (curr->processID == processID)
        {
            QueueNode *temp = curr;
            // if the first node is match
            if (prev == NULL)
            {
                queue->front = curr->next;
            }
            else
            {
                prev->next = curr->next;
            }
            if (curr == queue->rear)
            {
                queue->rear = prev;
            }
            free(temp);
            return 1;
        }
        prev = curr;
        curr = curr->next;
    }
    return 0;
}

// helper method to kill a process
void killProcess(ProcessControlBlock **CPUProcess, ProcessControlBlock **IOProcess)
{
    // dequeue process that should kill
    ProcessControlBlock *killPCB = dequeue(KillEventQueue);
    // add kill pcb pid to terminated queue
    if (killPCB->state == 3)
    {
        return;
    }
    killPCB->state = 4;
    enqueueProcess(TerminatedQueue, killPCB->processID, TimeUnit);
    // first check cpu process
    if (killPCB == (*CPUProcess))
    {
        (*CPUProcess) = NULL;
        return;
    }
    // then io process
    if (killPCB == (*IOProcess))
    {
        (*IOProcess) = NULL;
        return;
    }

    // check the ready queue for the process
    if (deleteProcess(readyQueue, killPCB->processID))
    {
        return;
    }
    // check IO queue for the process
    deleteProcess(IOWaitingQueue, killPCB->processID);
}
#ifndef PROCESS_STRUCTURE_C
#define PROCESS_STRUCTURE_C

#define MAP_SIZE 100
// structure of the process control block
typedef struct ProcessControlBlock
{
    int processID;
    char processName[20];
    int burstTime;
    int IOTime;
    int IODuration;
    // 0 - Ready, 1 - Running, 2 - Waiting, 3 - Terminated, 4 - Killed
    int state;
    int turnAroundTime;
    // next block
    struct ProcessControlBlock *next;
} ProcessControlBlock;

// structure for hash map
typedef struct HashMap
{
    ProcessControlBlock *table[MAP_SIZE];
    int noOfElements;
} HashMap;

// structure for queueNode and Queue
typedef struct QueueNode
{
    int processID;
    // kill time (only for kill queue)
    int killTime; // -1 for other than kill queue
    struct QueueNode *next;
} QueueNode;

typedef struct Queue
{
    QueueNode *front;
    QueueNode *rear;
    int size;
} Queue;

// global variables
HashMap *processMap;
Queue *readyQueue;
Queue *IOWaitingQueue;
Queue *TerminatedQueue;
Queue *KillEventQueue;

// function prototypes
void initializeStructures();
void addProcess(char processName[], int processID, int burstTime, int IOTime, int IODuration);
void addKillEvent(int processID, int killTime);
void simulateFCFS();
void displayResults();
void freeMemory();

#endif
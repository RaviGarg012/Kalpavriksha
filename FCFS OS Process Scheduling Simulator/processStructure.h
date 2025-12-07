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
    // 0 - New, 1 - Ready, 2 - Running, 3 - Waiting, 4 - Terminated, 5 - Killed
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
    struct QueueNode *next;
} QueueNode;

typedef struct Queue
{
    QueueNode *front;
    QueueNode *rear;
    int size;
} Queue;

// function prototypes
void initializeStructures();
void addProcess(char processName[], int processID, int burstTime, int IOTime, int IODuration);
void addKillEvent(int processID, int killTime);
void simulateFCFS();
void displayResults();
void freeMemory();

#endif
#ifndef STRUCTURE_H
#define STRUCTURE_H
#define MAX_SIZE 100
#include <stdlib.h>
// structure for message queue
struct messageBuffer
{
  long messageType;
  int arrayData[MAX_SIZE];
};

// global variable for array and size of the array
int *array;
int arraySize;

// function prototypes for different IPC mechanisms
void sortArray();
void displayArray();
void fileIPC();
void messageQueueIPC();
void pipeIPC();
void sharedMemoryIPC();
#endif
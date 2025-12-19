#ifndef STRUCTURE_H
#define STRUCTURE_H

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
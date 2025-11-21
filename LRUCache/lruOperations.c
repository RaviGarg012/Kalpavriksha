#include <stdio.h>
#include <stdlib.h>
// import lru structure
#include "LRUStructure.h"

Queue *queueHead, *queueTail;
HashMap *hashMap;
// intializing hashmap and queue
void createLRUCache(int capacity)
{
    // declare the queue head and tail to NULL
    queueHead = NULL;
    queueTail = NULL;
    hashMap = (HashMap *)malloc(sizeof(HashMap));
    // set capacity
    hashMap->capacity = capacity;
    // set no of elements to zero
    hashMap->noOfElements = 0;
    // set all the node in the table to NULL
    hashMap->table = (HashNode **)malloc(MAX_SIZE * sizeof(HashNode *));
    for (int tableIndex = 0; tableIndex < MAX_SIZE; tableIndex++)
    {
        hashMap->table[tableIndex] = NULL;
    }
}
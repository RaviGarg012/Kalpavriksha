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

// helper method for getting hashcode index
int getIndex(int key)
{
    return key % MAX_SIZE;
}

// update the queue for most recently used node
void updateQueue(Queue *node)
{
    // check if it at head
    if (node == queueHead)
        return;
    // check for tail
    if (node == queueTail)
    {
        // set tail to its prev
        queueTail = queueTail->prevNode;
        queueTail->nextNode = NULL;
        // set node prev to NULL
        node->prevNode = NULL;
        // add node to head
        node->nextNode = queueHead;
        queueHead = node;
        return;
    }
    /// if its in the middle set its neighbors first
    // for left neighbor
    node->prevNode->nextNode = node->nextNode;
    // for right neighbor
    node->nextNode->prevNode = node->prevNode;
    // set its prev to NULL
    node->prevNode = NULL;
    node->nextNode = queueHead;
    // update queueHead
    queueHead = node;
}

// getting queue node from hashmap
Queue *get(int key)
{
    // get index
    int index = getIndex(key);
    // start from first node and check all the node for key
    HashNode *current = hashMap->table[index];
    while (current != NULL)
    {
        // check for key
        if (current->key == key)
        {
            // update the queue for this node
            updateQueue(current->queueNode);
            return current->queueNode;
        }
        current = current->next;
    }
    return NULL;
}
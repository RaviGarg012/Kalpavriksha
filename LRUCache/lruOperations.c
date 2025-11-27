#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    // allcation failed
    if (hashMap == NULL)
    {
        printf("Memory Allocation Failed!");
        exit(1);
    }
    // set capacity
    hashMap->capacity = capacity;
    // set no of elements to zero
    hashMap->noOfElements = 0;
    // set all the node in the table to NULL
    hashMap->table = (HashNode **)malloc(MAX_SIZE * sizeof(HashNode *));
    if (hashMap->table == NULL)
    {
        printf("Memory Allocation Failed!");
        exit(1);
    }
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
        if (queueTail)
            queueTail->nextNode = NULL;
        // set node prev to NULL
        node->prevNode = NULL;
        // add node to head
        node->nextNode = queueHead;
        queueHead->prevNode = node;
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
    queueHead->prevNode = node;
    // update queueHead
    queueHead = node;
}

// helper method creating a new queueNode
Queue *createQNode(int key, char *value)
{
    Queue *newNode = (Queue *)malloc(sizeof(Queue));
    if (newNode == NULL)
    {
        printf("Memory Allocation Failed!");
        exit(1);
    }
    newNode->key = key;
    newNode->value = (char *)malloc(MAX_TOKEN_VALUE);
    if (newNode->value == NULL)
    {
        printf("Memory Allocation Failed!");
        exit(1);
    }
    strcpy(newNode->value, value);
    newNode->prevNode = NULL;
    newNode->nextNode = NULL;
    return newNode;
}

// helper method to create a new HashNode
HashNode *createHNode(int key, Queue *qNode)
{
    HashNode *hNode = (HashNode *)malloc(sizeof(HashNode));
    if (hNode == NULL)
    {
        printf("Memory Allocation Failed!");
        exit(1);
    }
    hNode->key = key;
    hNode->queueNode = qNode;
    hNode->next = NULL;
    return hNode;
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

// helper method to add new node in queue and hashMap
void addNewNode(int key, char *value)
{
    // first get the index for that key
    int index = getIndex(key);
    // create new queue node
    Queue *qNode = createQNode(key, value);
    // create new hashnode
    HashNode *hNode = createHNode(key, qNode);

    // add qNode at head of queue
    if (queueHead == NULL)
    {
        queueHead = queueTail = qNode;
    }
    else
    {
        qNode->nextNode = queueHead;
        queueHead->prevNode = qNode;
        // update head
        queueHead = qNode;
    }

    /// add that hNode to head of the index in table of hashmap
    hNode->next = hashMap->table[index];
    hashMap->table[index] = hNode;
    // update no of element in the hashmap
    hashMap->noOfElements++;
}

// helper method to delete the hashnode in hashmap
void deleteHashNode(int key)
{
    // get the index
    int index = getIndex(key);
    // get the head of theat idnex
    HashNode *current = hashMap->table[index];
    HashNode *prev = NULL;
    while (current != NULL)
    {
        if (current->key == key)
        {
            if (prev == NULL)
            {
                // for first node
                hashMap->table[index] = current->next;
            }
            else
            {
                // set prev next to curr next
                prev->next = current->next;
            }
            free(current);
            // update no of elements
            hashMap->noOfElements--;
            return;
        }
        // update prev and curr
        prev = current;
        current = current->next;
    }
}

// helper method to delete the least used node from the queue and hashMap
void deleteLeastUsedNode()
{
    if (queueTail == NULL)
        return;
    // get the node at the tail of queue
    Queue *leastNode = queueTail;
    // update queueTail
    queueTail = queueTail->prevNode;
    if (queueTail)
        queueTail->nextNode = NULL;

    // set prev of least node to null
    leastNode->prevNode = NULL;
    // get the key for least node
    int key = leastNode->key;
    // delete hashNode at that key
    deleteHashNode(key);

    // free the memory for least Node
    free(leastNode->value);
    free(leastNode);
}

// put method
void put(int key, char *value)
{
    // get the queue node from hashmap if key is already present
    Queue *newNode = get(key);
    // if node is not null then update value on that key
    if (newNode)
    {
        strcpy(newNode->value, value);
        return;
    }

    // node is NULL then its add new node to queue and hashmap
    addNewNode(key, value);

    // check the capacity condition
    if (hashMap->noOfElements > hashMap->capacity)
    {
        deleteLeastUsedNode();
    }
}

// free memory for queue
void freeQueue()
{
    // node at head
    Queue *current = queueHead, *next = NULL;
    while (current != NULL)
    {
        next = current->nextNode;
        // free current node
        free(current->value);
        free(current);
        current = next;
    }
    /// queue head and tail to null
    queueHead = queueTail = NULL;
}

// free hashmap
void freeHashMap()
{
    // traverse through all the index and node at the hashmap and free the memory
    HashNode *current = NULL, *next = NULL;
    for (int tableIndex = 0; tableIndex < MAX_SIZE; tableIndex++)
    {
        current = hashMap->table[tableIndex];
        // traverse the list
        while (current)
        {
            // set next value
            next = current->next;
            // free current
            free(current);
            current = next;
        }
    }

    // free hashmap
    free(hashMap->table);
    free(hashMap);
    hashMap = NULL;
}

// method for deallocating the memory after all the operations
void freeMemory()
{
    // first free memory for queue
    freeQueue();
    // then free memory for hashmap
    freeHashMap();
}
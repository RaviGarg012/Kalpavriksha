#ifndef LRUStructure_H
#define LRUStructure_H
#define MAX_SIZE 100
#define MAX_TOKEN_VALUE 20
// queue node
typedef struct Queue
{
    int key;
    char *value;
    // double link
    struct Queue *prevNode;
    struct Queue *nextNode;
} Queue;

// hash node
typedef struct HashNode
{
    int key;
    Queue *queueNode;
    struct HashNode *next;
} HashNode;

// hashmap structure
typedef struct HashMap
{
    struct HashNode **table;
    int noOfElements;
    int capacity;
} HashMap;

// function declaration
void createLRUCache(int capacity);
Queue *get(int key);
void put(int key, char *value);
void freeMemory();
#endif
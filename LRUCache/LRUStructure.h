#ifndef LRUStructure_H
#define LRUStructure_H
#define MAX_SIZE 100
// queue node
typedef struct Queue
{
    int key;
    char *value;
    // double link
    Queue *prevNode;
    Queue *nextNode;
} Queue;

// hash node
typedef struct HashNode
{
    int key;
    Queue *queueNode;
    HashNode *next;
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
#endif
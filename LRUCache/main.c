#include <stdio.h>
#include <stdlib.h>
// import lru structure
#include "LRUStructure.h"
#define MAX_LINE 100
// user input
void userInteraction()
{
    // for whole line
    char line[MAX_LINE];
    // for the command like put get and createCache
    char command[20];
    int capacity;
    int key;
    char value[50];

    // get the first input from user for capacity
    fgets(line, sizeof(line), stdin);
    // for create cache
    if (sscanf(line, "createCache %d", &capacity) == 1)
    {
        // create the cache
        createLRUCache(capacity);
    }
    else
    {
        // error, because first line must be creating cache
        printf("Error: First command must be createCache\n");
        return;
    }

    // process other 2 commands
    while (fgets(line, sizeof(line), stdin))
    {
        // for put
        if (sscanf(line, "put %d %s", &key, value) == 2)
        {
            // put the value
            put(key, value);
        }
        // for get
        else if (sscanf(line, "get %d", &key) == 1)
        {
            // get the key
            Queue *node = get(key);
            // print node value
            if (node)
            {
                printf("%s\n", node->value);
            }
            // for null
            else
            {
                printf("NULL\n");
            }
        }
        // invalid command
        else
        {
            printf("Invalid command!\n");
        }
    }
}
int main()
{
    // user interaction
    userInteraction();
    return 0;
}
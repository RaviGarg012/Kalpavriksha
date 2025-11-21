#include <stdio.h>
#include <stdlib.h>

void printArray(int *array, int sizeOfArray)
{
    for (int arrayIndex = 0; arrayIndex < sizeOfArray; arrayIndex++)
    {
        printf("%d ", *(array + arrayIndex));
    }
    printf("\n");
}

int main()
{
    // take the input
    int sizeOfArray;
    scanf("%d", &sizeOfArray);
    // create array
    int *originalArray = (int *)malloc(sizeOfArray * sizeof(int));
    // take the array input
    for (int arrayIndex = 0; arrayIndex < sizeOfArray; arrayIndex++)
    {
        scanf("%d", &originalArray[arrayIndex]);
    }

    // create array to copy
    int *copyArray = (int *)malloc(sizeOfArray * sizeof(int));

    // copy the array
    for (int arrayIndex = 0; arrayIndex < sizeOfArray; arrayIndex++)
    {
        *(copyArray + arrayIndex) = *(originalArray + arrayIndex);
    }

    printf("\noriginal Array:\n");
    printArray(originalArray, sizeOfArray);
    printf("\ncopied Array:\n");
    printArray(copyArray, sizeOfArray);
    return 0;
}
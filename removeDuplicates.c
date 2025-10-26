#include <stdio.h>
#include <stdlib.h>

// helper method to swap elements in an array
void swapElements(int *searchingArray, int firstIndex, int secondIndex)
{
    int temporary = *(searchingArray + firstIndex);
    *(searchingArray + firstIndex) = *(searchingArray + secondIndex);
    *(searchingArray + secondIndex) = temporary;
}

int main()
{
    // take the input
    int sizeOfArray;
    scanf("%d", &sizeOfArray);
    // create array
    int *searchingArray = (int *)malloc(sizeOfArray * sizeof(int));
    // take the array input
    for (int arrayIndex = 0; arrayIndex < sizeOfArray; arrayIndex++)
    {
        scanf("%d", &searchingArray[arrayIndex]);
    }

    // remove the duplicates
    int lowerIndex = 0;
    int higherIndex = sizeOfArray - 1;
    while (higherIndex >= lowerIndex)
    {
        // iterate through lower index to higher index and shift the element to higher index
        for (int arrayIndex = lowerIndex + 1; arrayIndex <= higherIndex; arrayIndex++)
        {
            if (*(searchingArray + lowerIndex) == *(searchingArray + arrayIndex))
            {
                swapElements(searchingArray, arrayIndex, higherIndex);
                higherIndex--;
                arrayIndex--;
            }
        }
        lowerIndex++;
    }

    // size of array
    printf("\nsize of array after removing duplicates: %d\n", lowerIndex);
    // print the array
    for (int arrayIndex = 0; arrayIndex < lowerIndex; arrayIndex++)
    {
        printf("%d ", *(searchingArray + arrayIndex));
    }
    return 0;
}
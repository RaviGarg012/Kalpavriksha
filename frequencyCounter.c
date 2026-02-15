#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MINVALUE INT_MIN

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

    // calculate the frequecy of each element
    for (int arrayFirstIndex = 0; arrayFirstIndex < sizeOfArray; arrayFirstIndex++)
    {
        // if element already considered then skip it this time
        if (searchingArray[arrayFirstIndex] == MINVALUE)
            continue;

        int frequencyOfElement = 1;
        for (int arrayCurrentIndex = arrayFirstIndex + 1; arrayCurrentIndex < sizeOfArray; arrayCurrentIndex++)
        {
            // increasing the frequency if current element is equals to searching element
            if (searchingArray[arrayCurrentIndex] == searchingArray[arrayFirstIndex])
            {
                frequencyOfElement++;
                searchingArray[arrayCurrentIndex] = MINVALUE;
            }
        }
        printf("Element %d appears %d times\n", searchingArray[arrayFirstIndex], frequencyOfElement);
    }
    return 0;
}
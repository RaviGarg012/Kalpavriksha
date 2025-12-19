#include <stdio.h>
#include "Structure.h"

// to swap two elements in the array
void swap(int firstIndex, int secondIndex)
{
    int temp = array[firstIndex];
    array[firstIndex] = array[secondIndex];
    array[secondIndex] = temp;
}

// function to perform quicksort on the array
void quickSort(int start, int end)
{
    // base case
    if (start >= end)
    {
        return;
    }

    int pivot = array[end];
    int partitionIndex = start;
    // partitioning the array
    for (int index = start; index < end; index++)
    {
        if (array[index] <= pivot)
        {
            // swap array[index] and array[partitionIndex]
            swap(index, partitionIndex);
            partitionIndex++;
        }
    }

    // swap array[partitionIndex] and array[end] (or pivot)
    swap(partitionIndex, end);

    // recursively sort the left and right subarrays
    quickSort(start, partitionIndex - 1);
    quickSort(partitionIndex + 1, end);
}

void sortArray()
{
    quickSort(0, arraySize - 1);
}

// helper method to display the array
void displayArray()
{
    for (int index = 0; index < 0; index++)
    {
        printf("%d ", array[index]);
    }
    printf("\n");
}
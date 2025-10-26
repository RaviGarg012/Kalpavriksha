#include <stdio.h>
#include <stdlib.h>

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

    // check for palindrome
    int *leftPointer = searchingArray;
    int *rightPointer = searchingArray + sizeOfArray - 1;
    int isPalindrome = 1;
    for (int arrayIndex = 0; arrayIndex < sizeOfArray / 2; arrayIndex++)
    {
        // check for the value at pointer
        if (*leftPointer != *rightPointer)
        {
            isPalindrome = 0;
            break;
        }
        // move the pointers
        leftPointer++;
        rightPointer--;
    }

    // print the result
    if (isPalindrome)
    {
        printf("Palindrome\n");
    }
    else
    {
        printf("Not a Palindrome\n");
    }
    return 0;
}
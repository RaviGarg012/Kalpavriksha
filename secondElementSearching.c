#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAXVALUE INT_MAX
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

  // searching second largest and second smallests element in the array
  int largestElement = MINVALUE, secondLargestElement = MINVALUE;
  int smallestElement = MAXVALUE, secondSmallestElement = MAXVALUE;
  for (int arrayIndex = 0; arrayIndex < sizeOfArray; arrayIndex++)
  {
    // search for second largest element
    if (searchingArray[arrayIndex] > largestElement)
    {
      secondLargestElement = largestElement;
      largestElement = searchingArray[arrayIndex];
    }
    else if (searchingArray[arrayIndex] < largestElement && searchingArray[arrayIndex] > secondLargestElement)
    {
      secondLargestElement = searchingArray[arrayIndex];
    }

    // search for second smallest element
    if (searchingArray[arrayIndex] < smallestElement)
    {
      secondSmallestElement = smallestElement;
      smallestElement = searchingArray[arrayIndex];
    }
    else if (searchingArray[arrayIndex] > smallestElement && searchingArray[arrayIndex] < secondSmallestElement)
    {
      secondSmallestElement = searchingArray[arrayIndex];
    }
  }

  // if second element has no changes then it is not possible to find second smallest element
  if (secondLargestElement == MINVALUE)
  {
    printf("Array neither have second smallest Element nor second largest element");
  }
  else
  {
    printf("Second Smallest Element: %d\n", secondSmallestElement);
    printf("Second Largest Element: %d\n", secondLargestElement);
  }
  return 0;
}
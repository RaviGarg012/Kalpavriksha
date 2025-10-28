/***
 ** doubts / questions
 1. In the generateRandomMatrix method, is it acceptable to use rand() function for generating random numbers, or should I implement a custom random number generator?
 2. In the rotateMatrixClockwise method, is it ok to use four temporary variables to hold the values of the four sides of the matrix during rotation, or should I try to minimize the use of extra variables?
    (I used this for better readability.)
***/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// helper method to generate a random matrix of N by N
void generateRandomMatrix(int **imageMatrix, int sizeOfMatrix)
{
    // set seed to generate random number
    srand(time(0));
    for (int imageRow = 0; imageRow < sizeOfMatrix; imageRow++)
    {
        for (int imageColumn = 0; imageColumn < sizeOfMatrix; imageColumn++)
        {
            // generating random number from 0 to 255
            *(*(imageMatrix + imageRow) + imageColumn) = rand() % 256;
        }
    }
}

// helper method for rotating the image matrix 90 degree clockwise
void rotateMatrixClockwise(int **imageMatrix, int sizeOfMatrix)
{
    int halfSizeOfMatrix = sizeOfMatrix / 2;
    int maxCellValue = sizeOfMatrix - 1;
    for (int matrixBoundaryIndex = 0; matrixBoundaryIndex < halfSizeOfMatrix; matrixBoundaryIndex++)
    {
        for (int matrixCellIndex = matrixBoundaryIndex; matrixCellIndex < maxCellValue - matrixBoundaryIndex; matrixCellIndex++)
        {
            // take a element from all four side of boundary
            int topRowElement = *(*(imageMatrix + matrixBoundaryIndex) + matrixCellIndex);
            int RightColumnElement = *(*(imageMatrix + matrixCellIndex) + maxCellValue - matrixBoundaryIndex);
            int bottomRowElement = *(*(imageMatrix + maxCellValue - matrixBoundaryIndex) + maxCellValue - matrixCellIndex);
            int LeftColumnElement = *(*(imageMatrix + maxCellValue - matrixCellIndex) + matrixBoundaryIndex);

            // rotate/swap these element 90 degree clockwise
            *(*(imageMatrix + matrixBoundaryIndex) + matrixCellIndex) = LeftColumnElement;
            *(*(imageMatrix + matrixCellIndex) + maxCellValue - matrixBoundaryIndex) = topRowElement;
            *(*(imageMatrix + maxCellValue - matrixBoundaryIndex) + maxCellValue - matrixCellIndex) = RightColumnElement;
            *(*(imageMatrix + maxCellValue - matrixCellIndex) + matrixBoundaryIndex) = bottomRowElement;
        }
    }
}

// helper method to apply smoothning filter on image matrix
void smoothImageMatrix(int **imageMatrix, int sizeOfMatrix)
{
    // array to hold the prev row element in the image matrix
    int *previousRowElements = (int *)malloc(sizeOfMatrix * sizeof(int));

    for (int imageRow = 0; imageRow < sizeOfMatrix; imageRow++)
    {
        // array to hold the current row element in the image matrix
        int *currentRowElements = (int *)malloc(sizeOfMatrix * sizeof(int));
        // copy elements of current row to currentRowElements array
        for (int imageColumn = 0; imageColumn < sizeOfMatrix; imageColumn++)
        {
            *(currentRowElements + imageColumn) = *(*(imageMatrix + imageRow) + imageColumn);
        }

        // iterate through each column of current row
        for (int imageColumn = 0; imageColumn < sizeOfMatrix; imageColumn++)
        {
            int sumOfNeighbors = 0;
            int countOfNeighbors = 0;

            // iterate through the 3x3 grid
            for (int neighborRow = -1; neighborRow <= 1; neighborRow++)
            {
                for (int neighborColumn = -1; neighborColumn <= 1; neighborColumn++)
                {
                    int neighborImageRow = imageRow + neighborRow;
                    int neighborImageColumn = imageColumn + neighborColumn;

                    // check if neighbor indices are within bounds
                    if (neighborImageRow < 0 || neighborImageColumn < 0 || neighborImageRow >= sizeOfMatrix || neighborImageColumn >= sizeOfMatrix)
                    {
                        // skip out of bound indices
                        continue;
                    }

                    // get the neighbor element value
                    if (neighborRow == -1)
                    {
                        sumOfNeighbors += *(previousRowElements + neighborImageColumn);
                    }
                    else if (neighborRow == 0)
                    {
                        sumOfNeighbors += *(currentRowElements + neighborImageColumn);
                    }
                    else
                    {
                        sumOfNeighbors += *(*(imageMatrix + neighborImageRow) + neighborImageColumn);
                    }

                    // increase count of neighbors
                    countOfNeighbors++;
                }
            }

            // update the image matrix cell with average of neighbors
            *(*(imageMatrix + imageRow) + imageColumn) = sumOfNeighbors / countOfNeighbors;
        }
        // free the previous row elements array
        free(previousRowElements);
        // copy current row elemnts to previous row
        previousRowElements = currentRowElements;
    }
    // free the last PreviousRowElements allocatted memory
    free(previousRowElements);
    previousRowElements = NULL;
}

// helper method for printing the image matrix
void printImageMatrix(int **imageMatrix, int sizeOfMatrix)
{
    for (int imageRow = 0; imageRow < sizeOfMatrix; imageRow++)
    {
        for (int imageColumn = 0; imageColumn < sizeOfMatrix; imageColumn++)
        {
            // print the cell of matrix
            printf("%3d ", *(*(imageMatrix + imageRow) + imageColumn));
        }
        // next line for next row
        printf("\n");
    }
}

int main()
{
    // get the input for size of image matrix
    int sizeOfMatrix;
    printf("Enter matrix size (2-10): ");
    scanf("%d", &sizeOfMatrix);
    // initializing image matrix array
    int **imageMatrix = (int **)malloc(sizeOfMatrix * sizeof(int *));
    // check for memory allocation failure
    if (imageMatrix == NULL)
    {
        printf("Memory allocation failed!\n");
        return 0;
    }

    int sizeOfInteger = sizeof(int);
    for (int matrixRow = 0; matrixRow < sizeOfMatrix; matrixRow++)
    {
        // initializing each row of matrix
        *(imageMatrix + matrixRow) = (int *)malloc(sizeOfMatrix * sizeOfInteger);
        // check for memory allocation failure
        if (*(imageMatrix + matrixRow) == NULL)
        {
            printf("Memory allocation failed!\n");
            return 0;
        }
    }

    // generate matrix
    generateRandomMatrix(imageMatrix, sizeOfMatrix);

    // print original matrix
    printf("\n");
    printf("Original Randomly Generated Matrix:\n");
    printImageMatrix(imageMatrix, sizeOfMatrix);
    printf("\n");

    // rotate the matrix in 90 degree clockwise
    rotateMatrixClockwise(imageMatrix, sizeOfMatrix);

    // print rotated matrix
    printf("Matrix after 90 degree Clockwise Rotation:\n");
    printImageMatrix(imageMatrix, sizeOfMatrix);

    // smooth the image matrix
    smoothImageMatrix(imageMatrix, sizeOfMatrix);

    // print smoothed matrix
    printf("\n");
    printf("Matrix after Applying 3x3 Smoothing Filter:\n");
    printImageMatrix(imageMatrix, sizeOfMatrix);

    // free allocated memory for image matrix
    for (int matrixRow = 0; matrixRow < sizeOfMatrix; matrixRow++)
    {
        // free each row of matrix
        free(*(imageMatrix + matrixRow));
    }
    free(imageMatrix);
    return 0;
}
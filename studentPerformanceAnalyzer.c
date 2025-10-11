#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_VALUE INT_MAX;

// structure of student
struct Student
{
    int rollNo;
    char name[50];
    int marksSubject1;
    int marksSubject2;
    int marksSubject3;
};

// helper method to print the student information
void printStudentInfo(struct Student student)
{
    // get the total marks of student
    int totalMarks = student.marksSubject1 + student.marksSubject2 + student.marksSubject3;
    // calcualte average of marks
    float averageMarks = (float)totalMarks / 3;

    // print the student info
    printf("Roll: %d\n", student.rollNo);
    printf("Name: %s\n", student.name);
    printf("Total: %d\n", totalMarks);
    printf("Average: %.2f\n", averageMarks);

    // print the grade of student and performance
    if (averageMarks >= 85)
    {
        printf("Grade: A\n");
        printf("Performance: *****\n");
    }
    else if (averageMarks >= 70)
    {
        printf("Grade: B\n");
        printf("Performance: ****\n");
    }
    else if (averageMarks >= 50)
    {
        printf("Grade: C\n");
        printf("Performance: ***\n");
    }
    else if (averageMarks >= 35)
    {
        printf("Grade: D\n");
        printf("Performance: **\n");
    }
    else
    {
        printf("Grade: F\n");
    }
}

// helper method to print the list of roll numbers of students using recursion
void printListOfRollNumbers(int *studentsRollNumbers, int maxRollNumberIndex)
{
    // base case
    if (maxRollNumberIndex < 0)
    {
        return;
    }

    int minRollNumber = MAX_VALUE;
    int minRollNumberIndex = -1;
    // find the minimum roll number and its index
    for (int rollNumberIndex = 0; rollNumberIndex <= maxRollNumberIndex; rollNumberIndex++)
    {
        if (studentsRollNumbers[rollNumberIndex] < minRollNumber)
        {
            minRollNumber = studentsRollNumbers[rollNumberIndex];
            minRollNumberIndex = rollNumberIndex;
        }
    }
    // print the minimum roll number
    printf("%d ", minRollNumber);
    // replace the max roll number with last index of array
    studentsRollNumbers[minRollNumberIndex] = studentsRollNumbers[maxRollNumberIndex];
    // recursive call
    printListOfRollNumbers(studentsRollNumbers, maxRollNumberIndex - 1);
}

int main()
{
    struct Student students[101];
    int totalStudents;
    int studentsRollNumbers[101] = {0};

    // get the number of total students
    scanf("%d", &totalStudents);
    // get the data of each student
    for (int studentIndex = 0; studentIndex < totalStudents; studentIndex++)
    {
        // get roll no of student
        scanf("%d", &students[studentIndex].rollNo);
        // get name of student
        scanf("%s", students[studentIndex].name);
        // get the marks of all three subjects
        scanf("%d %d %d", &students[studentIndex].marksSubject1, &students[studentIndex].marksSubject2, &students[studentIndex].marksSubject3);
        // mark the roll number as present
        studentsRollNumbers[studentIndex] = students[studentIndex].rollNo;
    }

    printf("\n\n");

    // printing the information of each student
    for (int i = 0; i < totalStudents; i++)
    {
        printStudentInfo(students[i]);
        printf("\n");
    }

    // printing the list of roll numbers of students using recursion
    printf("List of Roll Numbers: ");
    printListOfRollNumbers(studentsRollNumbers, totalStudents - 1);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>

// Structure for each segment of the snake
struct Node
{
    int row, col;
    struct Node *next;
};

// Function to create a new node
struct Node *createNode(int r, int c)
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->row = r;
    newNode->col = c;
    newNode->next = NULL;
    return newNode;
}

// Function to add a new head segment (for movement or growth)
void addHead(struct Node **head, int r, int c)
{
    struct Node *newHead = createNode(r, c);
    newHead->next = *head;
    *head = newHead;
}

// Function to remove the tail segment (if no food eaten)
void removeTail(struct Node **head)
{
    if (*head == NULL || (*head)->next == NULL)
        return;

    struct Node *temp = *head;
    while (temp->next->next != NULL)
        temp = temp->next;

    free(temp->next);
    temp->next = NULL;
}

// Function to check if the snake collides with itself
int checkSelfCollision(struct Node *head, int row, int column)
{
    struct Node *temp = head;
    while (temp != NULL)
    {
        if (temp->row == row && temp->col == column)
            return 1;
        temp = temp->next;
    }
    return 0;
}

// Function to check if a position contains food
int isFood(int r, int c, int **food, int F)
{
    for (int i = 0; i < F; i++)
    {
        if (food[i][0] == r && food[i][1] == c)
        {
            food[i][0] = -1; // mark as eaten
            return 1;
        }
    }
    return 0;
}

int main()
{
    int R, C;
    scanf("%d %d", &R, &C);

    int F;
    scanf("%d", &F);
    int **food = (int **)malloc(F * sizeof(int *));
    for (int index = 0; index < F; index++)
        food[index] = (int *)malloc(2 * sizeof(int));
    for (int index = 0; index < F; index++)
        scanf("%d %d", &food[index][0], &food[index][1]);

    int M;
    scanf("%d", &M);
    char *moves = (char *)malloc(M * sizeof(char));
    for (int index = 0; index < M; index++)
        scanf(" %c", &moves[index]);

    // Initialize snake
    struct Node *snake = createNode(1, 1);
    int length = 1;

    // Movement  U, D, L, R
    int dr, dc;

    for (int move = 0; move < M; move++)
    {
        int head_r = snake->row;
        int head_c = snake->col;

        // new direction
        switch (moves[move])
        {
        case 'U':
            dr = -1;
            dc = 0;
            break;
        case 'D':
            dr = 1;
            dc = 0;
            break;
        case 'L':
            dr = 0;
            dc = -1;
            break;
        case 'R':
            dr = 0;
            dc = 1;
            break;
        default:
            dr = dc = 0;
        }

        int new_r = head_r + dr;
        int new_c = head_c + dc;

        // Wall collision check
        if (new_r < 1 || new_r > R || new_c < 1 || new_c > C)
        {
            printf("GAME OVER %d\n", move + 1);
            return 0;
        }

        // Self-collision check
        if (checkSelfCollision(snake, new_r, new_c))
        {
            printf("GAME OVER %d\n", move + 1);
            return 0;
        }

        // Add new head
        addHead(&snake, new_r, new_c);

        // Check food
        if (isFood(new_r, new_c, food, F))
        {
            length++; // grow — do not remove tail
        }
        else
        {
            removeTail(&snake); // normal move
        }
    }
    // If survived all moves
    printf("ALIVE %d\n", length);

    // free allocated memory
    for (int i = 0; i < F; i++)
        free(food[i]);
    free(food);
    free(moves);
    while (snake != NULL)
    {
        struct Node *temp = snake;
        snake = snake->next;
        free(temp);
    }
    return 0;
}

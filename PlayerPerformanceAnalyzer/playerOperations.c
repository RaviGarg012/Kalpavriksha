#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Players_data.h"
#include "playerStructure.h"

// get the teamId from the teams array
int getTeamId(const char *teamName)
{
    for (int teamId = 0; teamId < teamCount; teamId++)
    {
        if (strcmp(teams[teamId], teamName) == 0)
        {
            // team id from 1 to teamCount
            return teamId + 1;
        }
    }
    // team not found
    return -1;
}

// calculate performance index for a player
void calculatePerformanceIndex(PlayerDetail *player)
{
    // base case
    if (player == NULL)
        return;

    // formula for performance index
    if (strcmp(player->role, "Batsman") == 0)
    {
        player->performanceIndex = (player->battingAverage * player->strikeRate) / 100.0;
    }
    else if (strcmp(player->role, "All-rounder") == 0)
    {
        player->performanceIndex = ((player->battingAverage * player->strikeRate) / 100.0) + (player->wickets * 2);
    }
    else
    {
        player->performanceIndex = (player->wickets * 2) + (100.0 - player->economyRate);
    }
}
// helper method to add the player in playerlist
void addPlayerToPlayerList(PlayerDetail **head, PlayerDetail *newPlayer)
{
    if (newPlayer == NULL)
        return;

    newPlayer->nextPlayer = NULL;

    // new player has highest performance
    if (*head == NULL || newPlayer->performanceIndex > (*head)->performanceIndex)
    {
        newPlayer->nextPlayer = *head;
        *head = newPlayer;
        return;
    }

    PlayerDetail *current = *head;

    // Traverse until you find the insertion point
    while (current->nextPlayer != NULL &&
           current->nextPlayer->performanceIndex >= newPlayer->performanceIndex)
    {
        current = current->nextPlayer;
    }

    // Insert node
    newPlayer->nextPlayer = current->nextPlayer;
    current->nextPlayer = newPlayer;
}

// add player to the respective team list
void addPlayerToTeam(Team *team, PlayerDetail *player)
{
    if (team == NULL || player == NULL)
        return;

    // increment total players in the team
    team->totalPlayers += 1;
    team->teamId = getTeamId(player->teamName);
    strcpy(team->teamName, player->teamName);
    // add player to the respective list based on role
    if (strcmp(player->role, "Batsman") == 0)
    {
        team->averageBattingStrikerate = ((team->averageBattingStrikerate * (team->totalPlayers - 1)) + player->strikeRate) / team->totalPlayers;
        addPlayerToPlayerList(&team->batsmenList, player);
    }
    else if (strcmp(player->role, "All-rounder") == 0)
    {
        team->averageBattingStrikerate = ((team->averageBattingStrikerate * (team->totalPlayers - 1)) + player->strikeRate) / team->totalPlayers;
        addPlayerToPlayerList(&team->allroundersList, player);
    }
    else if (strcmp(player->role, "Bowler") == 0)
    {
        addPlayerToPlayerList(&team->bowlersList, player);
    }
}

// function to initialize teams
void initializeTeams(Team allTeams[])
{

    // populate allPlayers from players data
    for (int playerIndex = 0; playerIndex < playerCount; playerIndex++)
    {
        PlayerDetail *newPlayer = (PlayerDetail *)malloc(sizeof(PlayerDetail));

        if (newPlayer == NULL)
        {
            printf("Memory allocation failed!\n");
            exit(1);
        }
        // copy the player data to new custom structure
        *newPlayer = (PlayerDetail){players[playerIndex].id, "", "", "",
                                    players[playerIndex].totalRuns,
                                    players[playerIndex].battingAverage,
                                    players[playerIndex].strikeRate,
                                    players[playerIndex].wickets,
                                    players[playerIndex].economyRate,
                                    0.0,
                                    NULL};

        strcpy(newPlayer->playerName, players[playerIndex].name);
        strcpy(newPlayer->teamName, players[playerIndex].team);
        strcpy(newPlayer->role, players[playerIndex].role);
        // calculate performance index for the player
        calculatePerformanceIndex(newPlayer);
        // get the team id
        int teamId = getTeamId(newPlayer->teamName);
        // add player to the respective team
        addPlayerToTeam(&allTeams[teamId - 1], newPlayer);
    }
}

// helper method to display player details
void displayPlayerDetails(PlayerDetail *player, int includeTeam)
{
    if (player == NULL)
        return;
    printf("%-6d %-24s ", player->playerId, player->playerName);
    if (includeTeam)
    {
        printf("%-15s ", player->teamName);
    }
    printf("%-12s %-8d %-8.1f %-8.1f %-8d %-8.1f %-10.2f\n",
           player->role,
           player->totalRuns,
           player->battingAverage,
           player->strikeRate,
           player->wickets,
           player->economyRate,
           player->performanceIndex);
}

// quicksort the teams according to batting average
void quickSortTeamsByStrikeRate(Team teams[], int teamIds[], int low, int high)
{
    // base case
    if (low >= high)
        return;
    // take the pivot element
    int pivot = high;
    int left = low - 1, right = high;
    while (1)
    {
        // find element greater than pivot from left
        do
        {
            left++;
        } while (teams[teamIds[left]].averageBattingStrikerate > teams[teamIds[pivot]].averageBattingStrikerate);
        // find element smaller than pivot from right
        do
        {
            right--;
        } while (right >= low && teams[teamIds[right]].averageBattingStrikerate < teams[teamIds[pivot]].averageBattingStrikerate);
        // if pointers cross, break
        if (left >= right)
            break;
        // swap the elements
        int temp = teamIds[left];
        teamIds[left] = teamIds[right];
        teamIds[right] = temp;
    }
    // swap pivot to its correct position
    int temp = teamIds[left];
    teamIds[left] = teamIds[pivot];
    teamIds[pivot] = temp;
    // recursive calls
    quickSortTeamsByStrikeRate(teams, teamIds, low, left - 1);
    quickSortTeamsByStrikeRate(teams, teamIds, left + 1, high);
}

// helper method to heapify the max heap
void heapify(HeapNode **heap, int heapSize, int index)
{
    int largest = index;
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;
    // check if left child is larger than root
    if (leftChild < heapSize && (*heap)[leftChild].player->performanceIndex > (*heap)[largest].player->performanceIndex)
    {
        largest = leftChild;
    }
    // check if right child is larger than largest so far
    if (rightChild < heapSize && (*heap)[rightChild].player->performanceIndex > (*heap)[largest].player->performanceIndex)
    {
        largest = rightChild;
    }

    // if largest is not root
    if (largest != index)
    {
        // swap
        HeapNode temp = (*heap)[index];
        (*heap)[index] = (*heap)[largest];
        (*heap)[largest] = temp;

        // recursively heapify the affected sub-tree
        heapify(heap, heapSize, largest);
    }
}

// helper method to build max heap
void buildMaxHeap(HeapNode **heap, int heapSize)
{
    for (int index = (heapSize / 2) - 1; index >= 0; index--)
    {
        // heapify each node
        heapify(heap, heapSize, index);
    }
}

// helper to display sorted players by role using max heap
void displaySortPlayerByRole(Team allTeams[], int roleChoice)
{
    HeapNode *heap = (HeapNode *)malloc(teamCount * sizeof(HeapNode));
    int heapSize = 0;
    // create array to store players of specific roles
    for (int index = 0; index < teamCount; index++)
    {
        PlayerDetail *playerHead = NULL;
        // add player to heap based on role
        if (roleChoice == 1)
            playerHead = allTeams[index].batsmenList;
        else if (roleChoice == 2)
            playerHead = allTeams[index].bowlersList;
        else
            playerHead = allTeams[index].allroundersList;

        // initialize heap node
        if (playerHead != NULL)
        {
            heap[index].player = playerHead;
            heap[index].teamIndex = index;
            heapSize++;
        }
        else
        {
            heap[index].player = NULL;
            heap[index].teamIndex = -1;
        }
    }

    buildMaxHeap(&heap, heapSize);
    // get the root of heap and print the data
    while (heapSize > 0)
    {
        // get the player with highest performance index
        HeapNode topNode = heap[0];
        displayPlayerDetails(topNode.player, 1);
        // move to the next player
        if (topNode.player->nextPlayer != NULL)
        {
            heap[0].player = topNode.player->nextPlayer;
        }
        else
        {
            // replace root with last element
            heap[0] = heap[heapSize - 1];
            heapSize--;
        }
        // heapify the root
        heapify(&heap, heapSize, 0);
    }

    // free allocated memory for heap
    free(heap);
}
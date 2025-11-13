#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// importing structure of players
#include "PlayerStructure.h"
#include "Players_data.h"
#define MAX_TEAM 10

Team allTeams[MAX_TEAM] = {0};

// add player to a team
void addPlayer()
{
    int teamId;
    printf("Enter Team ID to add player (1-10): ");
    scanf("%d", &teamId);
    if (teamId < 1 || teamId > MAX_TEAM)
    {
        printf("Invalid Team ID! Please try again.\n");
        return;
    }
    printf("Enter Player Details:\n");
    PlayerDetail *newPlayer = (PlayerDetail *)malloc(sizeof(PlayerDetail));
    // set team name based on team id
    strcpy(newPlayer->teamName, teams[teamId - 1]);
    if (newPlayer == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    // get player details from user
    printf("Player ID: ");
    scanf("%d", &newPlayer->playerId);
    printf("Name: ");
    scanf(" %[^\n]", newPlayer->playerName);
    int roleChoice;
    printf("Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &roleChoice);
    if (roleChoice < 1 || roleChoice > 3)
    {
        printf("Invalid role choice! Please try again.\n");
        free(newPlayer);
        return;
    }
    switch (roleChoice)
    {
    case 1:
        strcpy(newPlayer->role, "Batsman");
        break;
    case 2:
        strcpy(newPlayer->role, "Bowler");
        break;
    case 3:
        strcpy(newPlayer->role, "All-rounder");
        break;
    default:
        break;
    }
    printf("Total Runs: ");
    scanf("%d", &newPlayer->totalRuns);
    printf("Batting Average: ");
    scanf("%f", &newPlayer->battingAverage);
    printf("Strike Rate: ");
    scanf("%f", &newPlayer->strikeRate);
    printf("Wickets: ");
    scanf("%d", &newPlayer->wickets);
    printf("Economy Rate: ");
    scanf("%f", &newPlayer->economyRate);
    // calculate performance index for the player
    calculatePerformanceIndex(newPlayer);
    // add player to the respective team
    addPlayerToTeam(&allTeams[teamId - 1], newPlayer);
    // success message
    printf("Player added successfully to Team %s!\n", teams[teamId - 1]);
}

// display players of a specific team
void displayPlayers()
{
    int teamId;
    printf("Enter Team ID: ");
    scanf("%d", &teamId);
    if (teamId < 1 || teamId > MAX_TEAM)
    {
        printf("Invalid Team ID! Please try again.\n");
        return;
    }
    // display players of the team
    Team selectedTeam = allTeams[teamId - 1];
    printf("\nPlayers of Team %s:\n", selectedTeam.teamName);
    printf("==================================================================================================\n");
    printf("%-6s %-20s %-12s %-8s %-8s %-8s %-8s %-8s %-10s\n",
           "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf.Index");
    printf("==================================================================================================\n");
    PlayerDetail *currentPlayer;
    // display batsmen
    currentPlayer = selectedTeam.batsmenList;
    while (currentPlayer != NULL)
    {
        displayPlayerDetails(currentPlayer, 0);
        currentPlayer = currentPlayer->nextPlayer;
    }
    // display allrounders
    currentPlayer = selectedTeam.allroundersList;
    while (currentPlayer != NULL)
    {
        displayPlayerDetails(currentPlayer, 0);
        currentPlayer = currentPlayer->nextPlayer;
    }
    // display bowlers
    currentPlayer = selectedTeam.bowlersList;
    while (currentPlayer != NULL)
    {
        displayPlayerDetails(currentPlayer, 0);
        currentPlayer = currentPlayer->nextPlayer;
    }
    printf("==================================================================================================\n");

    printf("Total Players: %d\n", selectedTeam.totalPlayers);
    printf("Average Batting Strike Rate: %.2f\n", selectedTeam.averageBattingStrikerate);
}

// helper method to display teams by average batting strike rate
void displayTeamsByStrikeRate()
{
    // create an array of teams id and sort that
    int teamIds[MAX_TEAM];
    for (int teamIndex = 0; teamIndex < MAX_TEAM; teamIndex++)
    {
        teamIds[teamIndex] = teamIndex;
    }
    // sort team ids based on average batting strike rate using quick sort
    quickSortTeamsByStrikeRate(allTeams, teamIds, 0, MAX_TEAM - 1);
    printf("\nTeams Sorted by Average Batting Strike Rate:\n");
    printf("==============================================================================\n");
    printf("%-4s %-20s %-20s %-15s\n", "ID", "Team Name", "Avg Bat SR", "Total Players");
    printf("==============================================================================\n");
    for (int teamIndex = 0; teamIndex < MAX_TEAM; teamIndex++)
    {
        printf("%-4d %-20s %-20.2f %-15d\n",
               teamIds[teamIndex] + 1,
               allTeams[teamIds[teamIndex]].teamName,
               allTeams[teamIds[teamIndex]].averageBattingStrikerate,
               allTeams[teamIds[teamIndex]].totalPlayers);
    }
    printf("==============================================================================\n");
}

void userInteraction()
{
    // user input
    while (1)
    {
        printf("\n");
        printf("==============================================================================\n");
        printf("   ICC ODI Player Performance Analyzer\n");
        printf("==============================================================================\n");
        printf("1. Add Player to Team\n");
        printf("2. Display Players of a Specific Team\n");
        printf("3. Display Teams by Average Batting Strike Rate\n");
        printf("4. Display Top K Players of a Specific Team by Role\n");
        printf("5. Display all Players of specific role Across All Teams by performance index\n");
        printf("6. Exit\n");
        printf("==============================================================================\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            // Add Player to Team
            addPlayer();
            break;
        case 2:
            // Display Players of a Specific Team
            displayPlayers();
            break;
        case 3:
            // Display Teams by Average Batting Strike Rate
            displayTeamsByStrikeRate();
            break;
        case 4:
            // Display Top K Players of a Specific Team by Role
            break;
        case 5:
            // Display all Players of specific role Across All Teams by performance index
            break;
        case 6:
            // Exit
            printf("Exiting the program.....\n");
            return;
        default:
            printf("Invalid choice! Please try again.\n");
        }
    }
}

// free all allocated memory for teams
void freeAllTeams(Team allTeams[])
{
    for (int teamIndex = 0; teamIndex < teamCount; teamIndex++)
    {
        PlayerDetail *currentPlayer;
        PlayerDetail *nextPlayer;

        // Free batsmen list
        currentPlayer = allTeams[teamIndex].batsmenList;
        while (currentPlayer != NULL)
        {
            nextPlayer = currentPlayer->nextPlayer;
            free(currentPlayer);
            currentPlayer = nextPlayer;
        }

        // Free allrounders list
        currentPlayer = allTeams[teamIndex].allroundersList;
        while (currentPlayer != NULL)
        {
            nextPlayer = currentPlayer->nextPlayer;
            free(currentPlayer);
            currentPlayer = nextPlayer;
        }

        // Free bowlers list
        currentPlayer = allTeams[teamIndex].bowlersList;
        while (currentPlayer != NULL)
        {
            nextPlayer = currentPlayer->nextPlayer;
            free(currentPlayer);
            currentPlayer = nextPlayer;
        }
    }
}

int main()
{
    // initialize teams from the provided data
    initializeTeams(allTeams);
    // user interaction
    userInteraction();

    // free allocated memory for players in each team
    freeAllTeams(allTeams);
    return 0;
}
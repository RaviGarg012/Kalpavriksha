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
    printf("%-6d %-20s ", player->playerId, player->playerName);
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
#ifndef PLAYERS_STRUCTURE_H
#define PLAYERS_STRUCTURE_H

// structure for player data
typedef struct PlayerDetail
{
  int playerId;
  char playerName[50];
  char teamName[50];
  char role[50];
  int totalRuns;
  float battingAverage;
  float strikeRate;
  int wickets;
  float economyRate;
  float performanceIndex;
  struct PlayerDetail *nextPlayer;
} PlayerDetail;

// structure for team
typedef struct
{
  int teamId;
  char teamName[50];
  int totalPlayers;
  float averageBattingStrikerate;
  // three pointer for the list of batsmen, allrounders and bowlers
  PlayerDetail *batsmenList;
  PlayerDetail *allroundersList;
  PlayerDetail *bowlersList;
} Team;

// function declarations
void initializeTeams(Team teams[]);
void calculatePerformanceIndex(PlayerDetail *player);
void addPlayerToTeam(Team *team, PlayerDetail *player);
void displayPlayerDetails(PlayerDetail *player, int includeTeam);
#endif
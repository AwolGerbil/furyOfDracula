#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "game.h"
#include "HunterView.h"
#include "hunter.h"
#include "Queue.h"

#define unknownMove(X) (X == UNKNOWN_LOCATION)

static int startLoc (int player);
static int bFS (HunterView gameState, int dest, int curr, int player);
static int randomLoc (HunterView gameState, int playerLoc, int player);
static char* locCode (int locNum);

void decideMove (HunterView gameState) {
    srand(time(NULL));

    int player = getCurrentPlayer(gameState);
    int playerLoc = getLocation(gameState, player);
    int bestMove = UNKNOWN_LOCATION;

    // Starting round
    if (!getRound(gameState)) {
        bestMove = startLoc(player);
    }

    // Rest if low
    if (unknownMove(bestMove)
        && getHealth(gameState, player) < 5) {
        bestMove = playerLoc;
    }

    // Kill dracula
    int draculaLoc = getLocation(gameState, PLAYER_DRACULA);
    if (unknownMove(bestMove)
        && draculaLoc >= ALICANTE && draculaLoc <= ZURICH) {
        bestMove = bFS(gameState, draculaLoc, playerLoc, player);
    }

    // Cannot research yet
    if (unknownMove(bestMove)
        && getRound(gameState) < 6) {
        bestMove = randomLoc(gameState, playerLoc, player);
    }

    // Research
    if (unknownMove(bestMove)) {
        bestMove = playerLoc;
    }

    registerBestPlay(locCode(bestMove), "YOLO\n\n\n\n");
}

static int startLoc (int player) {
    int startLocs[] = {MUNICH, STRASBOURG, SARAGOSSA, KLAUSENBURG};
    return startLocs[player];
}

static int bFS (HunterView gameState, int dest, int curr, int player) {

    Queue Q = newQueue();

    int visited[NUM_MAP_LOCATIONS];
    int pre[NUM_MAP_LOCATIONS];
    int i;
    for(i=0;i<NUM_MAP_LOCATIONS;i++){
        visited[i] = 0;
        pre[i] = 0;
    }

    int numAdj;

    int v;
    while(!QueueIsEmpty(Q)){
        v = QueueLeave(Q);
        if(v == dest){
            while(1){
                v = pre[v];
                if (pre[v] == curr){
                    return v;
                }
            }
        }
        int *adj = connectedLocations(gameState, &numAdj, v, player, getRound(gameState), 1, 1, 1);
        for(i=0;i<numAdj;i++){
            if(!visited[adj[i]]){
                pre[adj[i]] = v;
                visited[adj[i]] = 1;
                QueueJoin(Q, adj[i]);
            }
        }

    }

    //We should never get here...
    return UNKNOWN_LOCATION;

}
static int randomLoc (HunterView gameState, int playerLoc, int player) {
    int numLocations;
    int* adjLocs = connectedLocations(gameState, &numLocations,
                                      playerLoc, player,
                                      getRound(gameState),
                                      1, 1, 0);
    return adjLocs[rand() % numLocations];
}

static char* locCode (int locNum) {
    char* locCodes[] = {"AL", "AM", "AT", "BA", "BI", "BE", "BR", "BO",
                        "BU", "BC", "BD", "CA", "CG", "CD", "CF", "CO",
                        "CN", "DU", "ED", "FL", "FR", "GA", "GW", "GE",
                        "GO", "GR", "HA", "JM", "KL", "LE", "LI", "LS",
                        "LV", "LO", "MA", "MN", "MR", "MI", "MU", "NA",
                        "NP", "NU", "PA", "PL", "PR", "RO", "SA", "SN",
                        "SR", "SJ", "SO", "ST", "SW", "SZ", "TO", "VA",
                        "VR", "VE", "VI", "ZA", "ZU", "NS", "EC", "IS",
                        "AO", "BB", "MS", "TS", "IO", "AS", "BS", "C?",
                        "S?", "HI", "D1", "D2", "D3", "D4", "D5", "TP"};
    return locCodes[locNum];
}

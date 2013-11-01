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
static void findPorts (int possLocs[]);
static char* locCode (int locNum);

void decideMove (HunterView gameState) {
    struct timeval time;
    gettimeofday(&time, NULL);
    srand((time.tv_sec * 1000) + (time.tv_usec / 1000));

    int i;
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

    // Determine possible locations of Dracula
    int* possLocs = possibleLocations(gameState);
    int possCount;
    int probSum = 0;
    int randNum;
    if (unknownMove(bestMove)) {
        if (draculaLoc >= ALICANTE && draculaLoc <= ZURICH) {
            bestMove = bFS(gameState, draculaLoc, playerLoc, player);
        } else {
            if (draculaLoc = SEA_UNKNOWN) {
                findPorts(possLocs);
            }

            for (i = 0; i < NUM_MAP_LOCATIONS; i++) {
                possCount++;
                probSum += possLocs[i];
                possLocs[i] = probSum;
            }

            randNum = rand() % probSum;
            for (i = 0; i < NUM_MAP_LOCATIONS && possLocs[i] <= randNum; i++);
            bestMove = bFS(gameState, i, playerLoc, player);
        }
    }

    free(possLocs);

    // Cannot research yet
    if (unknownMove(bestMove)
        && getRound(gameState) < 6) {
        bestMove = randomLoc(gameState, playerLoc, player);
    }

    // Research
    if (unknownMove(bestMove) || possCount > 30) {
        bestMove = playerLoc;
    }

    registerBestPlay(locCode(bestMove), "YOLO\n\n\n\n");
}

static int startLoc (int player) {
    int startLocs[] = {MUNICH, STRASBOURG, SARAGOSSA, KLAUSENBURG};
    return startLocs[player];
}

static int bFS (HunterView gameState, int dest, int curr, int player) {

    if(curr == dest){
        return dest;
    }

    Queue Q = newQueue();

    int visited[NUM_MAP_LOCATIONS];
    int pre[NUM_MAP_LOCATIONS];
    int i;
    for (i = 0; i < NUM_MAP_LOCATIONS; i++) {
        visited[i] = 0;
        pre[i] = 0;
    }

    int numAdj;

    QueueJoin(Q, curr);

    int v;
    while (!QueueIsEmpty(Q)) {
        v = QueueLeave(Q);

        if (v == dest) {
            while (1) {
                if (pre[v] == curr){
                    return v;
                }
                v = pre[v];
            }

        }


        int *adj = connectedLocations(gameState, &numAdj, v, player, getRound(gameState), 1, 1, 1);
        for (i = 0; i < numAdj; i++) {
            if (!visited[adj[i]]) {
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

static void findPorts (int possLocs[]) {
    if (possLocs[NORTH_SEA]) {
        possLocs[AMSTERDAM] = possLocs[NORTH_SEA];
        possLocs[EDINBURGH] = possLocs[NORTH_SEA];
        possLocs[HAMBURG] = possLocs[NORTH_SEA];
    }
    if (possLocs[ENGLISH_CHANNEL]) {
        possLocs[LE_HAVRE] = possLocs[ENGLISH_CHANNEL];
        possLocs[LONDON] = possLocs[ENGLISH_CHANNEL];
        possLocs[PLYMOUTH] = possLocs[ENGLISH_CHANNEL];
    }
    if (possLocs[IRISH_SEA]) {
        possLocs[DUBLIN] = possLocs[IRISH_SEA];
        possLocs[LIVERPOOL] = possLocs[IRISH_SEA];
        possLocs[SWANSEA] = possLocs[IRISH_SEA];
    }
    if (possLocs[ATLANTIC_OCEAN]) {
        possLocs[CADIZ] = possLocs[ATLANTIC_OCEAN];
        possLocs[GALWAY] = possLocs[ATLANTIC_OCEAN];
        possLocs[LISBON] = possLocs[ATLANTIC_OCEAN];
    }
    if (possLocs[BAY_OF_BISCAY]) {
        possLocs[BORDEAUX] = possLocs[BAY_OF_BISCAY];
        possLocs[NANTES] = possLocs[BAY_OF_BISCAY];
        possLocs[SANTANDER] = possLocs[BAY_OF_BISCAY];
    }
    if (possLocs[MEDITERRANEAN_SEA]) {
        possLocs[ALICANTE] = possLocs[MEDITERRANEAN_SEA];
        possLocs[BARCELONA] = possLocs[MEDITERRANEAN_SEA];
        possLocs[CAGLIARI] = possLocs[MEDITERRANEAN_SEA];
        possLocs[MARSEILLES] = possLocs[MEDITERRANEAN_SEA];
    }
    if (possLocs[TYRRHENIAN_SEA]) {
        possLocs[CAGLIARI] = possLocs[TYRRHENIAN_SEA];
        possLocs[GENOA] = possLocs[TYRRHENIAN_SEA];
        possLocs[NAPLES] = possLocs[TYRRHENIAN_SEA];
        possLocs[ROME] = possLocs[TYRRHENIAN_SEA];
    }
    if (possLocs[IONIAN_SEA]) {
        possLocs[ATHENS] = possLocs[IONIAN_SEA];
        possLocs[SALONICA] = possLocs[IONIAN_SEA];
        possLocs[VALONA] = possLocs[IONIAN_SEA];
    }
    if (possLocs[ADRIATIC_SEA]) {
        possLocs[BARI] = possLocs[ADRIATIC_SEA];
        possLocs[VENICE] = possLocs[ADRIATIC_SEA];
    }
    if (possLocs[BLACK_SEA]) {
        possLocs[CONSTANTA] = possLocs[BLACK_SEA];
        possLocs[VARNA] = possLocs[BLACK_SEA];
    }

    int i;
    for (i = NORTH_SEA; i <= BLACK_SEA; i++) possLocs[i] = 0;
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

#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "game.h"
#include "HunterView.h"
#include "hunter.h"

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
        printf("STARTING ROUND!!!\n");
        bestMove = startLoc(player);
    }

    // Rest if low
    if (unknownMove(bestMove)
        && getHealth(gameState, player) < 5) {
        printf("TOO LOW!!!\n");
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
        printf("WHERE ARE YA DRACULA :P\n");
        if (draculaLoc >= ALICANTE && draculaLoc <= ZURICH) {
            printf("I'M COMING TO GETCHA!!!\n");
            bestMove = bFS(gameState, draculaLoc, playerLoc, player);
        } else {
            if (draculaLoc == SEA_UNKNOWN) {
                printf("WHY ARE YOU AT SEA???\n");
                findPorts(possLocs);
            }

            for (i = 0; i < NUM_MAP_LOCATIONS; i++) {
                possCount++;
                probSum += possLocs[i];
                possLocs[i] = probSum;
            }

            randNum = rand() % probSum;
            for (i = 0; i < NUM_MAP_LOCATIONS && possLocs[i] <= randNum; i++);
            printf("WHEE, WERE GOING TO %s\n", locCode(i));
            printf("HOPE THIS BFS WORKS!!!\n");
            bestMove = bFS(gameState, i, playerLoc, player);
        }
    }

    free(possLocs);

    // Last resort
    if (unknownMove(bestMove)) {
        // Cannot research yet
        if (getRound(gameState) < 6) {
            printf("WOO CAN'T RESEARCH YET\n");
            bestMove = randomLoc(gameState, playerLoc, player);
        } else {
            printf("LETS SIT HERE\n");
            bestMove = playerLoc;
        }
    }

    int trail[MEGA_TRAIL_SIZE];
    getMegaHistory (gameState, player, trail);

    for (i = 0; i < MEGA_TRAIL_SIZE && trail[i] > 70; i++);

    if (i >= 10) {
        printf("LETS RESEARCH");
        bestMove = playerLoc;
    }

    printf("HERES MY TURN!!!\n");
    registerBestPlay(locCode(bestMove), "hmmmm");
    printf("%s, hmmm\n", locCode(bestMove));
}

static int startLoc (int player) {
    int startLocs[] = {MUNICH, STRASBOURG, SARAGOSSA, KLAUSENBURG};
    return startLocs[player];
}

static int bFS (HunterView gameState, int dest, int curr, int player) {
    if (curr == dest) {
        return dest;
    }

    // Random Counter
    int i;

    // Set pre array
    int pre[NUM_MAP_LOCATIONS];
    for (i = 0; i < NUM_MAP_LOCATIONS; i++) pre[i] = UNKNOWN_LOCATION;

    int queue[NUM_MAP_LOCATIONS];
    int front = 0;
    int size = 0;

    pre[curr] = curr;
    queue[front + size] = curr;
    size++;

    int* adj;
    int numAdj;

    int location;
    while (size) {
        location = queue[front];
        front++;
        size--;

        // If destination found, backtrace to next ideal location
        if (location == dest) {
            for (; pre[location] != curr; location = pre[location]);
            return location;
        }

        adj = connectedLocations(gameState, &numAdj, location, player, getRound(gameState), 1, 1, 1);
        for (i = 0; i < numAdj; i++) {
            if (pre[adj[i]] == UNKNOWN_LOCATION) {
                pre[adj[i]] = location;
                queue[front + size] = adj[i];
                size++;
            }
        }

        free(adj);
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
    int ret = adjLocs[rand() % numLocations];
    free(adjLocs);
    return ret;
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

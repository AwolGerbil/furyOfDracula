#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "HunterView.h"

#define MAX_ROUNDS 366
#define NO_EDGE -1
#define LAND 0
#define RAIL 1
#define SEA 2
#define LAND_RAIL 3

// #define's for previously defined numbers (but too long)
// DB = Double Back
#define DB1 74
#define DB2 75
#define DB3 76
#define DB4 77
#define DB5 78

// GS = Game Start, HP = Life Points
#define GS_HUNTER_HP 9
#define GS_DRACULA_HP 40

// Rest isn't defined as a move
#define REST -2

static void draculaMove (HunterView hv, char* play, int round);
static void hunterMove (HunterView hv, char* play, int round);
static int locNum (char* locCode);
static int playerTurns (HunterView hv, int player);

//Graph-related functions
static void addLink (HunterView hv, int start, int finish, int type);
static void makeGraph (HunterView hv);

static void canReachInN (HunterView currentView, LocationID start, int locs[], int n, int type);

typedef struct hunterView {
    int turns; // Number of Turns
    int score; // Current Score

    // Move Tracker
    char move[NUM_PLAYERS][MAX_ROUNDS];

    // Location Tracker
    char location[NUM_PLAYERS][MAX_ROUNDS];

    // Dracula Tracker
    char possLoc[2][NUM_MAP_LOCATIONS];

    // Hospital Tracker
    char isInHospital[NUM_PLAYERS-1][MAX_ROUNDS];

    // Health Tracker
    char health[NUM_PLAYERS][MAX_ROUNDS];

    // Adjacency Matrix
    char adjMatrix[NUM_MAP_LOCATIONS][NUM_MAP_LOCATIONS];
} hView;

HunterView newHunterView (char* pastPlays, playerMessage messages[]) {
    // Random Counters
    int i, j;

    // Allocate Memory
    HunterView hv = malloc(sizeof(hView));

    // Calculate Number of Turns played
    hv->turns = (strlen(pastPlays) + 1) / 8;

    // Simulate pastPlays
    // 1. Populate Location and Health Trackers
    // 2. Calculate Score

    //Initialise location, health and score:
    hv->score = GAME_START_SCORE;

    for (i = 0; i < NUM_PLAYERS; i++) {
        if (i == PLAYER_DRACULA) {
            hv->health[i][0] = GAME_START_BLOOD_POINTS;
        } else {
            hv->health[i][0] = GAME_START_HUNTER_LIFE_POINTS;
        }
        hv->location[i][0] = UNKNOWN_LOCATION;
    }

    for (i = 0; i < NUM_MAP_LOCATIONS; i++) {
        hv->possLoc[0][i] = 1;
    }

    //Initialise adjacent matrix
    for (i = 0; i < NUM_MAP_LOCATIONS; i++) {
        for (j = 0; j < NUM_MAP_LOCATIONS; j++) {
            hv->adjMatrix[i][j] = NO_EDGE;
        }
    }

    // Populate Adjacency Matrix
    makeGraph(hv);

    // Iterate through Player Turns
    int turn, round;

    for (turn = 0; turn < hv->turns; turn++) {
        round = ((turn) / 5);

        if (turn % NUM_PLAYERS == PLAYER_DRACULA) { // Dracula's Move
            draculaMove(hv, &pastPlays[turn * 8], round);
        } else { // Hunter's Move
            hunterMove(hv, &pastPlays[turn * 8], round);
        }
    }

    return hv;
}

// Interprets Dracula's Move

/* Dracula's Turn Format */

// 0: D (Dracula's turn)
// 1-2: Location/Move
    // Location         -> Location code
    // Unknown City     -> C?
    // Unknown Sea      -> S?
    // Hide             -> HI
    // Double Back 1-5  -> D{1-5}
    // Teleport         -> TP
// 3: Trap Placed
    // Trap             -> T
    // No Trap          -> .
// 4: Immature Vampire Placed
    // Immature Vampire -> V
    // No Vampire       -> .
// 5: Action Phase
    // Trap Malfunction -> M
    // Vampire Matured  -> V
    // Nothing          -> .
// 6: .
static void draculaMove (HunterView hv, char* play, int round) {
    // Check whether its actually Dracula's Play
    assert(play[0] == 'D');

    // Random Counters
    int i, j;

    // Update health for the round
    if (round >= 1) {
        hv->health[PLAYER_DRACULA][round] = hv->health[PLAYER_DRACULA][round-1];
    } else {
        hv->health[PLAYER_DRACULA][round] = GAME_START_BLOOD_POINTS;
    }

    // Update Score for Dracula's Turn
    hv -> score -= SCORE_LOSS_DRACULA_TURN;

    // Update Move for Dracula's Turn
    char buffer[3];
    buffer[0] = play[1];
    buffer[1] = play[2];
    buffer[2] = 0;
    int currMove = locNum(buffer); // Could be Hide/Double Back/TP
    hv->move[PLAYER_DRACULA][round] = currMove;

    // Update Location for Dracula's Turn
    int currLocation = currMove;
    int offset = 0;
    if (currMove == HIDE || currMove == DOUBLE_BACK_1) {
        offset = 1;
        currLocation = hv->location[PLAYER_DRACULA][round - offset];
    } else if (currMove >= DB2 && currMove <= DB5) {
        offset = currMove - DB2 + 2;
        currLocation = hv->location[PLAYER_DRACULA][round - offset];
    }
    if (currMove == TELEPORT) {
        currLocation = CASTLE_DRACULA;
    }
    hv->location[PLAYER_DRACULA][round] = currLocation;

    // Copy Dracula Tracker
    for (i = 0; i < NUM_MAP_LOCATIONS; i++) {
        hv->possLoc[1][i] = hv->possLoc[0][i];
    }

    int* connLoc;
    int numLoc;

    // Update Dracula Tracker
    if (currLocation >= ALICANTE && currLocation <= BLACK_SEA) {
        for (i = 0; i < NUM_MAP_LOCATIONS; i++) {
            hv->possLoc[0][i] = 0;
        }

        hv->possLoc[0][currLocation] = 1;
    } else {
        // Expand out
        for (i = 0; i < NUM_MAP_LOCATIONS; i++) {
            if (hv->possLoc[1][i]) {
                hv->possLoc[0][i] = hv->possLoc[1][i] + 1;

                connLoc = connectedLocations(hv, &numLoc, hv->possLoc[1][i], PLAYER_DRACULA, 0, 1, 0, 1);
                for (j = 0; j < numLoc; j++) {
                    if (!hv->possLoc[1][connLoc[j]]) {
                        hv->possLoc[0][connLoc[j]] = 1;
                    }
                }
                free(connLoc);
            }
        }

        // Trim to Land or Sea
        if (currLocation == CITY_UNKNOWN) {
            for (i = NORTH_SEA; i <= BLACK_SEA; i++) {
                hv->possLoc[0][i] = 0;
            }
        } else if (currLocation == SEA_UNKNOWN) {
            for (i = ALICANTE; i <= ZURICH; i++) {
                hv->possLoc[0][i] = 0;
            }
        }

        // Remove Trail
        int history[TRAIL_SIZE];
        getHistory(hv, PLAYER_DRACULA, history);
        for (i = 0; i < TRAIL_SIZE; i++) {
            if (history[i] >= ALICANTE && history[i] <= BLACK_SEA) {
                hv->possLoc[0][history[i]] = 0;
            }
        }
    }

    // Update Dracula's Health if at Sea
    if ((currLocation >= NORTH_SEA && currLocation <= BLACK_SEA)
        || currLocation == SEA_UNKNOWN) {
        hv->health[PLAYER_DRACULA][round] -= LIFE_LOSS_SEA;
    }

    // Update Dracula's Health if at Castle Dracula
    if (currLocation == CASTLE_DRACULA) {
        hv->health[PLAYER_DRACULA][round] += LIFE_GAIN_CASTLE_DRACULA;
    }

    // Update Score if Vampire Matures
    if (play[5] == 'V') {
        hv->score -= SCORE_LOSS_VAMPIRE_MATURES;
    }
}

// Interprets Hunter's Move

/* Hunter's Turn Format */

// 0: x={G, S, H, M} (x's turn)
// 1-2: Location
    // Location         -> Location code
// 3-6: Encounters
    // Trap             -> T
    // Immature Vampire -> V
    // Dracula          -> D
    // Buffer           -> .
static void hunterMove (HunterView hv, char* play, int round) {
    // Random Counter
    int i;

    // Check whether it is actually a Hunter's Play
    assert(play[0] == 'G' || play[0] == 'S'
           || play[0] == 'H' || play[0] == 'M');

    // Determine which Hunter
    int hunter;
    if (play[0] == 'G') {
        hunter = PLAYER_LORD_GODALMING;
    } else if (play[0] == 'S') {
        hunter = PLAYER_DR_SEWARD;
    } else if (play[0] == 'H') {
        hunter = PLAYER_VAN_HELSING;
    } else {
        hunter = PLAYER_MINA_HARKER;
    }

    //Initialise health to previous turn's health
    if (round >= 1) {
        hv->health[hunter][round] = hv->health[hunter][round-1];
    } else {
        hv->health[hunter][round] = GAME_START_HUNTER_LIFE_POINTS;
    }

    // Update Move and Location for Hunter's Turn
    char buffer[3];
    buffer[0] = play[1];
    buffer[1] = play[2];
    buffer[2] = 0;
    char currLocation = locNum(buffer);

    char currMove = currLocation;
    if (round >= 1 && currLocation == hv->location[hunter][round - 1]) {
        currMove = REST;
    }

    hv->move[hunter][round] = currMove;
    hv->location[hunter][round] = currLocation;

    // Update Health if Resting
    if (currMove == REST) {
        hv->health[hunter][round] += LIFE_GAIN_REST;
        if (hv->health[hunter][round] > 9) {
            hv->health[hunter][round] = 9;
        }
    }

    // Simulate Encounters
    for (i = 3; i <= 6; i++) {
        if (play[i] == 'T') {
            hv->health[hunter][round] -= LIFE_LOSS_TRAP_ENCOUNTER;
        } else if (play[i] == 'D') {
            hv->health[hunter][round] -= LIFE_LOSS_DRACULA_ENCOUNTER;
            hv->health[PLAYER_DRACULA][round - 1] -= LIFE_LOSS_HUNTER_ENCOUNTER;
        } else if (play[i] == '.') {
            break;
        }
    }

    if (hv->health[hunter][round] <= 0) {
        //Hunter has died! Oh no!
        //Reset health and teleport to hospital!
        hv->health[hunter][round] = GS_HUNTER_HP;
        hv->isInHospital[hunter][round] = 1;
    } else {
        hv->isInHospital[hunter][round] = 0;
    }

}

// Converts Location Code to Location Number
static int locNum (char* locCode) {
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
    int retVal;
    for (retVal = 0;
         strcmp(locCodes[(int)retVal], locCode) && retVal < NUM_LOCATIONS;
         retVal++);

    if (retVal == NUM_LOCATIONS) {
        retVal = -1;
    }

    return retVal;
}

// Adds Edges
static void addLink (HunterView hv, int start, int finish, int type) {
    assert(start < NUM_MAP_LOCATIONS && finish < NUM_MAP_LOCATIONS);
    assert(type >= LAND && type <= SEA);

    int currentType = hv->adjMatrix[start][finish];

    if ((type == LAND && currentType == RAIL)
        || (type == RAIL && currentType == LAND)) {
        //Special case to signify land and rail connection
        hv->adjMatrix[start][finish] = LAND_RAIL;
    } else {
        hv->adjMatrix[start][finish] = type;
    }
}

static int playerTurns (HunterView hv, int player) {
    if (player < getCurrentPlayer(hv)) {
        // Player has played in this round:
        return getRound(hv) + 1;
    } else {
        // If the player has not played in this round:
        return getRound(hv);
    }
}

// Frees all memory previously allocated for the HunterView
// toBeDeleted. toBeDeleted should not be accessed after the call.
void disposeHunterView (HunterView toBeDeleted) {
    free(toBeDeleted);
}

Round getRound (HunterView currentView) {
    return (currentView->turns/5);
}

PlayerID getCurrentPlayer (HunterView currentView) {
    return ((currentView->turns) % 5);
}

int getScore (HunterView currentView) {
    return currentView->score;
}

int getHealth (HunterView currentView, PlayerID player) {
    if (playerTurns(currentView, player) == 0) {
        if (player == PLAYER_DRACULA) {
            return GAME_START_BLOOD_POINTS;
        } else {
            return GAME_START_HUNTER_LIFE_POINTS;
        }
    } else {
        return currentView->health[player][playerTurns(currentView, player) - 1];
    }
}

LocationID getLocation(HunterView currentView, PlayerID player) {
    if (playerTurns(currentView, player) == 0) {
        return UNKNOWN_LOCATION;
    }

    if (player == PLAYER_DRACULA) {
        return currentView->move[player][playerTurns(currentView, player) - 1];
    } else {
        if (currentView->isInHospital[player][playerTurns(currentView, player) - 1]) {
            return ST_JOSEPH_AND_ST_MARYS;
        } else {
            return currentView->location[player][playerTurns(currentView, player) - 1];
        }
    }
}

void getHistory (HunterView currentView, PlayerID player,
                 LocationID trail[TRAIL_SIZE]) {
    // Random Counter
    int i;

    // Turns Player has had
    int pTurns = playerTurns(currentView, player);

    // Backtrace into stored locations
    for (i = 0; i < TRAIL_SIZE; i++) {
        if (i >= pTurns) { // Not enough Turns played
            trail[i] = UNKNOWN_LOCATION;
        } else if (player == PLAYER_DRACULA) { // Dracula
            trail[i] = currentView->move[player][pTurns - i - 1];
        } else { // Hunter
            trail[i] = currentView->location[player][pTurns - i -1 ];
        }
    }
}

void getMegaHistory (HunterView currentView, PlayerID player,
                     LocationID trail[MEGA_TRAIL_SIZE]) {
    // Random Counter
    int i;

    // Turns Player has had
    int pTurns = playerTurns(currentView, player);

    // Backtrace into stored locations
    for (i = 0; i < MEGA_TRAIL_SIZE; i++) {
        if (i >= pTurns) { // Not enough Turns played
            trail[i] = UNKNOWN_LOCATION;
        } else { // Anyone
            trail[i] = currentView->location[player][pTurns - i -1 ];
        }
    }
}

LocationID* connectedLocations (HunterView currentView,
                               int* numLocations, LocationID from,
                               PlayerID player, Round round,
                               int road, int rail, int sea) {
    int locs[NUM_MAP_LOCATIONS];
    int i;
    for (i = 0; i < NUM_MAP_LOCATIONS; i++) {
        locs[i] = -1;
    }

    if (road) {
        canReachInN(currentView, from, locs, 1, LAND);
    }

    if (sea) {
        canReachInN(currentView, from, locs, 1, SEA);
    }

    if (rail && (player != PLAYER_DRACULA)) {
        int railMoves = (player + round) % 4;
        canReachInN(currentView, from, locs, railMoves, RAIL);
    }

    *numLocations = 0;
    for (i = 0; i < NUM_MAP_LOCATIONS; i++) {
        if (locs[i] != -1) {
            (*numLocations)++;
        }
    }

    LocationID *retVal = malloc(sizeof(LocationID) * (*numLocations));

    int j = 0;
    for (i = 0; i < NUM_MAP_LOCATIONS; i++) {
        if (locs[i] != -1) {
            retVal[j] = i;
            j++;
        }
    }

    return retVal;
}

// Locs stores the min. number of moves to get to a location
// -1 if unreachable, 0 if reachable in n, ...
static void canReachInN (HunterView currentView, LocationID start, int locs[], int n, int type) {
    locs[start] = n;

    if (n == 0) {
        return;
    }

    int i;
    int edge;
    for (i = 0; i < NUM_MAP_LOCATIONS; i++) {
        edge = currentView->adjMatrix[start][i];
        if (n - 1 > locs[i]) {
            if (type == edge
                || (edge == LAND_RAIL
                    && (type == RAIL || type == LAND))) {
                canReachInN(currentView, i, locs, n-1, type);
            }
        }
    }
}

LocationID* possibleLocations (HunterView currentView) {
    int* retVal = malloc(NUM_MAP_LOCATIONS * sizeof(int));
    int i;
    for (i = 0; i < NUM_MAP_LOCATIONS; i++) {
        retVal[i] = currentView->possLoc[0][i];
    }

    return retVal;
}

static void makeGraph (HunterView hv){
    /* ROAD Connections*/

    // ALICANTE
    addLink(hv, ALICANTE, SARAGOSSA, LAND);
    addLink(hv, ALICANTE, GRANADA, LAND);
    addLink(hv, ALICANTE, MADRID, LAND);

    // AMSTERDAM
    addLink(hv, AMSTERDAM, COLOGNE, LAND);
    addLink(hv, AMSTERDAM, BRUSSELS, LAND);

    // ATHENS
    addLink(hv, ATHENS, VALONA, LAND);

    // BARCELONA
    addLink(hv, BARCELONA, SARAGOSSA, LAND);
    addLink(hv, BARCELONA, TOULOUSE, LAND);

    // BARI
    addLink(hv, BARI, NAPLES, LAND);
    addLink(hv, BARI, ROME, LAND);

    // BELGRADE
    addLink(hv, BELGRADE, SARAJEVO, LAND);
    addLink(hv, BELGRADE, ST_JOSEPH_AND_ST_MARYS, LAND);
    addLink(hv, BELGRADE, SZEGED, LAND);
    addLink(hv, BELGRADE, KLAUSENBURG, LAND);
    addLink(hv, BELGRADE, BUCHAREST, LAND);
    addLink(hv, BELGRADE, SOFIA, LAND);

    // BERLIN
    addLink(hv, BERLIN, LEIPZIG, LAND);
    addLink(hv, BERLIN, HAMBURG, LAND);
    addLink(hv, BERLIN, PRAGUE, LAND);

    // BORDEAUX
    addLink(hv, BORDEAUX, NANTES, LAND);
    addLink(hv, BORDEAUX, CLERMONT_FERRAND, LAND);
    addLink(hv, BORDEAUX, TOULOUSE, LAND);
    addLink(hv, BORDEAUX, SARAGOSSA, LAND);

    // BRUSSELS
    addLink(hv, BRUSSELS, AMSTERDAM, LAND);
    addLink(hv, BRUSSELS, COLOGNE, LAND);
    addLink(hv, BRUSSELS, STRASBOURG, LAND);
    addLink(hv, BRUSSELS, PARIS, LAND);
    addLink(hv, BRUSSELS, LE_HAVRE, LAND);

    // BUCHAREST
    addLink(hv, BUCHAREST, GALATZ, LAND);
    addLink(hv, BUCHAREST, CONSTANTA, LAND);
    addLink(hv, BUCHAREST, SOFIA, LAND);
    addLink(hv, BUCHAREST, BELGRADE, LAND);
    addLink(hv, BUCHAREST, KLAUSENBURG, LAND);

    // BUDAPEST
    addLink(hv, BUDAPEST, ZAGREB, LAND);
    addLink(hv, BUDAPEST, VIENNA, LAND);
    addLink(hv, BUDAPEST,KLAUSENBURG, LAND);
    addLink(hv, BUDAPEST, SZEGED, LAND);

    // CADIZ
    addLink(hv, CADIZ, GRANADA, LAND);
    addLink(hv, CADIZ, MADRID, LAND);
    addLink(hv, CADIZ, LISBON, LAND);

    // CASTLE_DRACULA
    addLink(hv, CASTLE_DRACULA, GALATZ, LAND);
    addLink(hv, CASTLE_DRACULA, KLAUSENBURG, LAND);

    // CLERMONT_FERRAND
    addLink(hv, CLERMONT_FERRAND, MARSEILLES, LAND);
    addLink(hv, CLERMONT_FERRAND, GENEVA, LAND);
    addLink(hv, CLERMONT_FERRAND, TOULOUSE, LAND);
    addLink(hv, CLERMONT_FERRAND, PARIS, LAND);
    addLink(hv, CLERMONT_FERRAND, BORDEAUX, LAND);
    addLink(hv, CLERMONT_FERRAND, NANTES, LAND);

    // COLOGNE
    addLink(hv, COLOGNE, STRASBOURG, LAND);
    addLink(hv, COLOGNE, BRUSSELS, LAND);
    addLink(hv, COLOGNE, FRANKFURT, LAND);
    addLink(hv, COLOGNE, HAMBURG, LAND);
    addLink(hv, COLOGNE, AMSTERDAM, LAND);
    addLink(hv, COLOGNE, LEIPZIG, LAND);

    // CONSTANTA
    addLink(hv, CONSTANTA, VARNA, LAND);
    addLink(hv, CONSTANTA, BUCHAREST, LAND);
    addLink(hv, CONSTANTA, GALATZ, LAND);

    // DUBLIN
    addLink(hv, DUBLIN, GALWAY, LAND);

    // EDINBURGH
    addLink(hv, EDINBURGH, MANCHESTER, LAND);

    // FLORENCE
    addLink(hv, FLORENCE, GENOA, LAND);
    addLink(hv, FLORENCE, VENICE, LAND);
    addLink(hv, FLORENCE, ROME, LAND);

    // FRANKFURT
    addLink(hv, FRANKFURT, STRASBOURG, LAND);
    addLink(hv, FRANKFURT, COLOGNE, LAND);
    addLink(hv, FRANKFURT, LEIPZIG, LAND);
    addLink(hv, FRANKFURT, NUREMBURG, LAND);

    // GALATZ
    addLink(hv, GALATZ, CONSTANTA, LAND);
    addLink(hv, GALATZ, BUCHAREST, LAND);
    addLink(hv, GALATZ, KLAUSENBURG, LAND);
    addLink(hv, GALATZ, CASTLE_DRACULA, LAND);

    // GALWAY
    addLink(hv, GALWAY, DUBLIN, LAND);

    // GENEVA
    addLink(hv, GENEVA, ZURICH, LAND);
    addLink(hv, GENEVA, STRASBOURG, LAND);
    addLink(hv, GENEVA, PARIS, LAND);
    addLink(hv, GENEVA, CLERMONT_FERRAND, LAND);
    addLink(hv, GENEVA, MARSEILLES, LAND);

    // GENOA
    addLink(hv, GENOA, VENICE, LAND);
    addLink(hv, GENOA, FLORENCE, LAND);
    addLink(hv, GENOA, MILAN, LAND);
    addLink(hv, GENOA, MARSEILLES, LAND);

    // GRANADA
    addLink(hv, GRANADA, ALICANTE, LAND);
    addLink(hv, GRANADA, MADRID, LAND);
    addLink(hv, GRANADA, CADIZ, LAND);

    // HAMBURG
    addLink(hv, HAMBURG, COLOGNE, LAND);
    addLink(hv, HAMBURG, BERLIN, LAND);
    addLink(hv, HAMBURG, LEIPZIG, LAND);

    // ST_JOSEPH_AND_ST_MARYS
    addLink(hv, ST_JOSEPH_AND_ST_MARYS, SARAJEVO, LAND);
    addLink(hv, ST_JOSEPH_AND_ST_MARYS, ZAGREB, LAND);
    addLink(hv, ST_JOSEPH_AND_ST_MARYS, SZEGED, LAND);
    addLink(hv, ST_JOSEPH_AND_ST_MARYS, BELGRADE, LAND);

    // KLAUSENBURG
    addLink(hv, KLAUSENBURG, BELGRADE, LAND);
    addLink(hv, KLAUSENBURG, SZEGED, LAND);
    addLink(hv, KLAUSENBURG, BUDAPEST, LAND);
    addLink(hv, KLAUSENBURG, CASTLE_DRACULA, LAND);
    addLink(hv, KLAUSENBURG, GALATZ, LAND);
    addLink(hv, KLAUSENBURG, BUCHAREST, LAND);

    // LE_HAVRE
    addLink(hv, LE_HAVRE, PARIS, LAND);
    addLink(hv, LE_HAVRE, BRUSSELS, LAND);
    addLink(hv, LE_HAVRE, NANTES, LAND);

    // LEIPZIG
    addLink(hv, LEIPZIG, FRANKFURT, LAND);
    addLink(hv, LEIPZIG, COLOGNE, LAND);
    addLink(hv, LEIPZIG, HAMBURG, LAND);
    addLink(hv, LEIPZIG, BERLIN, LAND);
    addLink(hv, LEIPZIG, NUREMBURG, LAND);

    // LISBON
    addLink(hv, LISBON, CADIZ, LAND);
    addLink(hv, LISBON, MADRID, LAND);
    addLink(hv, LISBON, SANTANDER, LAND);

    // LIVERPOOL
    addLink(hv, LIVERPOOL, SWANSEA, LAND);
    addLink(hv, LIVERPOOL, EDINBURGH, LAND);

    // LONDON
    addLink(hv, LONDON, PLYMOUTH, LAND);
    addLink(hv, LONDON, SWANSEA, LAND);
    addLink(hv, LONDON, MANCHESTER, LAND);

    // MADRID
    addLink(hv, MADRID, LISBON, LAND);
    addLink(hv, MADRID, CADIZ, LAND);
    addLink(hv, MADRID, GRANADA, LAND);
    addLink(hv, MADRID, ALICANTE, LAND);
    addLink(hv, MADRID, SANTANDER, LAND);
    addLink(hv, MADRID, SARAGOSSA, LAND);

    // MANCHESTER
    addLink(hv, MANCHESTER, LIVERPOOL, LAND);
    addLink(hv, MANCHESTER, EDINBURGH, LAND);
    addLink(hv, MANCHESTER, LONDON, LAND);

    // MARSEILLES
    addLink(hv, MARSEILLES, GENEVA, LAND);
    addLink(hv, MARSEILLES, ZURICH, LAND);
    addLink(hv, MARSEILLES, MILAN, LAND);
    addLink(hv, MARSEILLES, GENOA, LAND);
    addLink(hv, MARSEILLES, CLERMONT_FERRAND, LAND);
    addLink(hv, MARSEILLES, TOULOUSE, LAND);

    // MILAN
    addLink(hv, MILAN, GENOA, LAND);
    addLink(hv, MILAN, VENICE, LAND);
    addLink(hv, MILAN, MARSEILLES, LAND);
    addLink(hv, MILAN, ZURICH, LAND);
    addLink(hv, MILAN, MUNICH, LAND);

    // MUNICH
    addLink(hv, MUNICH, MILAN, LAND);
    addLink(hv, MUNICH, ZURICH, LAND);
    addLink(hv, MUNICH, STRASBOURG, LAND);
    addLink(hv, MUNICH, NUREMBURG, LAND);
    addLink(hv, MUNICH, VIENNA, LAND);
    addLink(hv, MUNICH, ZAGREB, LAND);
    addLink(hv, MUNICH, VENICE, LAND);

    // NANTES
    addLink(hv, NANTES, LE_HAVRE, LAND);
    addLink(hv, NANTES, PARIS, LAND);
    addLink(hv, NANTES, BORDEAUX, LAND);
    addLink(hv, NANTES, CLERMONT_FERRAND, LAND);

    // NAPLES
    addLink(hv, NAPLES, ROME, LAND);
    addLink(hv, NAPLES, BARI, LAND);

    // NUREMBURG
    addLink(hv, NUREMBURG, STRASBOURG, LAND);
    addLink(hv, NUREMBURG, FRANKFURT, LAND);
    addLink(hv, NUREMBURG, LEIPZIG, LAND);
    addLink(hv, NUREMBURG, PRAGUE, LAND);
    addLink(hv, NUREMBURG, MUNICH, LAND);

    // PARIS
    addLink(hv, PARIS, STRASBOURG, LAND);
    addLink(hv, PARIS, GENEVA, LAND);
    addLink(hv, PARIS, BRUSSELS, LAND);
    addLink(hv, PARIS, LE_HAVRE, LAND);
    addLink(hv, PARIS, NANTES, LAND);
    addLink(hv, PARIS, CLERMONT_FERRAND, LAND);

    // PLYMOUTH
    addLink(hv, PLYMOUTH, LONDON, LAND);

    // PRAGUE
    addLink(hv, PRAGUE, NUREMBURG, LAND);
    addLink(hv, PRAGUE, BERLIN, LAND);
    addLink(hv, PRAGUE, VIENNA, LAND);

    // ROME
    addLink(hv, ROME, FLORENCE, LAND);
    addLink(hv, ROME, NAPLES, LAND);
    addLink(hv, ROME, BARI, LAND);

    // SALONICA
    addLink(hv, SALONICA, VALONA, LAND);
    addLink(hv, SALONICA, SOFIA, LAND);

    // SANTANDER
    addLink(hv, SANTANDER, MADRID, LAND);
    addLink(hv, SANTANDER, LISBON, LAND);
    addLink(hv, SANTANDER, SARAGOSSA, LAND);

    // SARAGOSSA
    addLink(hv, SARAGOSSA, TOULOUSE, LAND);
    addLink(hv, SARAGOSSA, BARCELONA, LAND);
    addLink(hv, SARAGOSSA, BORDEAUX, LAND);
    addLink(hv, SARAGOSSA, ALICANTE, LAND);
    addLink(hv, SARAGOSSA, MADRID, LAND);
    addLink(hv, SARAGOSSA, SANTANDER, LAND);

    // SARAJEVO
    addLink(hv, SARAJEVO, VALONA, LAND);
    addLink(hv, SARAJEVO, ZAGREB, LAND);
    addLink(hv, SARAJEVO, ST_JOSEPH_AND_ST_MARYS, LAND);
    addLink(hv, SARAJEVO, BELGRADE, LAND);
    addLink(hv, SARAJEVO, SOFIA, LAND);

    // SOFIA
    addLink(hv, SOFIA, VARNA, LAND);
    addLink(hv, SOFIA, SALONICA, LAND);
    addLink(hv, SOFIA, VALONA, LAND);
    addLink(hv, SOFIA, SARAJEVO, LAND);
    addLink(hv, SOFIA, BELGRADE, LAND);
    addLink(hv, SOFIA, BUCHAREST, LAND);

    // STRASBOURG
    addLink(hv, STRASBOURG, ZURICH, LAND);
    addLink(hv, STRASBOURG, MUNICH, LAND);
    addLink(hv, STRASBOURG, GENEVA, LAND);
    addLink(hv, STRASBOURG, NUREMBURG, LAND);
    addLink(hv, STRASBOURG, FRANKFURT, LAND);
    addLink(hv, STRASBOURG, COLOGNE, LAND);
    addLink(hv, STRASBOURG, BRUSSELS, LAND);
    addLink(hv, STRASBOURG, PARIS, LAND);

    // SWANSEA
    addLink(hv, SWANSEA, LONDON, LAND);
    addLink(hv, SWANSEA, LIVERPOOL, LAND);

    // SZEGED
    addLink(hv, SZEGED, ST_JOSEPH_AND_ST_MARYS, LAND);
    addLink(hv, SZEGED, ZAGREB, LAND);
    addLink(hv, SZEGED, BUDAPEST, LAND);
    addLink(hv, SZEGED, KLAUSENBURG, LAND);
    addLink(hv, SZEGED, BELGRADE, LAND);

    // TOULOUSE
    addLink(hv, TOULOUSE, BORDEAUX, LAND);
    addLink(hv, TOULOUSE, CLERMONT_FERRAND, LAND);
    addLink(hv, TOULOUSE, MARSEILLES, LAND);
    addLink(hv, TOULOUSE, BARCELONA, LAND);
    addLink(hv, TOULOUSE, SARAGOSSA, LAND);

    // VALONA
    addLink(hv, VALONA, ATHENS, LAND);
    addLink(hv, VALONA, SALONICA, LAND);
    addLink(hv, VALONA, SOFIA, LAND);
    addLink(hv, VALONA, SARAJEVO, LAND);

    // VARNA
    addLink(hv, VARNA, SOFIA, LAND);
    addLink(hv, VARNA, CONSTANTA, LAND);

    // VENICE
    addLink(hv, VENICE, FLORENCE, LAND);
    addLink(hv, VENICE, GENOA, LAND);
    addLink(hv, VENICE, MILAN, LAND);
    addLink(hv, VENICE, MUNICH, LAND);

    // VIENNA
    addLink(hv, VIENNA, ZAGREB, LAND);
    addLink(hv, VIENNA, MUNICH, LAND);
    addLink(hv, VIENNA, PRAGUE, LAND);
    addLink(hv, VIENNA, BUDAPEST, LAND);

    // ZAGREB
    addLink(hv, ZAGREB, SARAJEVO, LAND);
    addLink(hv, ZAGREB, MUNICH, LAND);
    addLink(hv, ZAGREB, VIENNA, LAND);
    addLink(hv, ZAGREB, BUDAPEST, LAND);
    addLink(hv, ZAGREB, SZEGED, LAND);
    addLink(hv, ZAGREB, ST_JOSEPH_AND_ST_MARYS, LAND);

    // ZURICH
    addLink(hv, ZURICH, MILAN, LAND);
    addLink(hv, ZURICH, MUNICH, LAND);
    addLink(hv, ZURICH, GENEVA, LAND);
    addLink(hv, ZURICH, MARSEILLES, LAND);
    addLink(hv, ZURICH, STRASBOURG, LAND);

    /* RAIL Connections */

    // ALICANTE
    addLink(hv, ALICANTE, MADRID, RAIL);
    addLink(hv, ALICANTE, BARCELONA, RAIL);

    // BARCELONA
    addLink(hv, BARCELONA, ALICANTE, RAIL);
    addLink(hv, BARCELONA, SARAGOSSA, RAIL);

    // BARI
    addLink(hv, BARI, NAPLES, RAIL);

    // BELGRADE
    addLink(hv, BELGRADE, SZEGED, RAIL);
    addLink(hv, BELGRADE, SOFIA, RAIL);

    // BERLIN
    addLink(hv, BERLIN, HAMBURG, RAIL);
    addLink(hv, BERLIN, PRAGUE, RAIL);
    addLink(hv, BERLIN, LEIPZIG, RAIL);

    // BORDEAUX
    addLink(hv, BORDEAUX, SARAGOSSA, RAIL);
    addLink(hv, BORDEAUX, PARIS, RAIL);

    // BRUSSELS
    addLink(hv, BRUSSELS, COLOGNE, RAIL);
    addLink(hv, BRUSSELS, PARIS, RAIL);

    // BUCHAREST
    addLink(hv, BUCHAREST, SZEGED, RAIL);
    addLink(hv, BUCHAREST, GALATZ, RAIL);
    addLink(hv, BUCHAREST, CONSTANTA, RAIL);

    // BUDAPEST
    addLink(hv, BUDAPEST, VIENNA, RAIL);
    addLink(hv, BUDAPEST, SZEGED, RAIL);

    // COLOGNE
    addLink(hv, COLOGNE, BRUSSELS, RAIL);
    addLink(hv, COLOGNE, FRANKFURT, RAIL);

    // CONSTANTA
    addLink(hv, CONSTANTA, BUCHAREST, RAIL);

    // FLORENCE
    addLink(hv, FLORENCE, ROME, RAIL);
    addLink(hv, FLORENCE, MILAN, RAIL);

    // FRANKFURT
    addLink(hv, FRANKFURT, STRASBOURG, RAIL);
    addLink(hv, FRANKFURT, LEIPZIG, RAIL);
    addLink(hv, FRANKFURT, COLOGNE, RAIL);

    // GALATZ
    addLink(hv, GALATZ, BUCHAREST, RAIL);

    // HAMBURG
    addLink(hv, HAMBURG, BERLIN, RAIL);

    // LEIPZIG
    addLink(hv, LEIPZIG, FRANKFURT, RAIL);
    addLink(hv, LEIPZIG, NUREMBURG, RAIL);
    addLink(hv, LEIPZIG, BERLIN, RAIL);

    // LISBON
    addLink(hv, LISBON, MADRID, RAIL);

    // LIVERPOOL
    addLink(hv, LIVERPOOL, MANCHESTER, RAIL);

    // LONDON
    addLink(hv, LONDON, SWANSEA, RAIL);
    addLink(hv, LONDON, MANCHESTER, RAIL);

    // MADRID
    addLink(hv, MADRID, ALICANTE, RAIL);
    addLink(hv, MADRID, LISBON, RAIL);
    addLink(hv, MADRID, SANTANDER, RAIL);
    addLink(hv, MADRID, SARAGOSSA, RAIL);

    // MILAN
    addLink(hv, MILAN, GENEVA, RAIL);
    addLink(hv, MILAN, FLORENCE, RAIL);
    addLink(hv, MILAN, ZURICH, RAIL);
    addLink(hv, MILAN, GENOA, RAIL);

    // MANCHESTER
    addLink(hv, MANCHESTER, LONDON, RAIL);
    addLink(hv, MANCHESTER, LIVERPOOL, RAIL);
    addLink(hv, MANCHESTER, EDINBURGH, RAIL);

    // NAPLES
    addLink(hv, NAPLES, ROME, RAIL);
    addLink(hv, NAPLES, BARI, RAIL);

    // NUREMBURG
    addLink(hv, NUREMBURG, MUNICH, RAIL);
    addLink(hv, NUREMBURG, LEIPZIG, RAIL);

    // PARIS
    addLink(hv, PARIS, BORDEAUX, RAIL);
    addLink(hv, PARIS, MARSEILLES, RAIL);
    addLink(hv, PARIS, LE_HAVRE, RAIL);
    addLink(hv, PARIS, BRUSSELS, RAIL);

    // PRAGUE
    addLink(hv, PRAGUE, BERLIN, RAIL);
    addLink(hv, PRAGUE, VIENNA, RAIL);

    // ROME
    addLink(hv, ROME, FLORENCE, RAIL);
    addLink(hv, ROME, NAPLES, RAIL);

    // SALONICA
    addLink(hv, SALONICA, SOFIA, RAIL);

    // SANTANDER
    addLink(hv, SANTANDER, MADRID, RAIL);

    // SARAGOSSA
    addLink(hv, SARAGOSSA, MADRID, RAIL);
    addLink(hv, SARAGOSSA, BARCELONA, RAIL);
    addLink(hv, SARAGOSSA, BORDEAUX, RAIL);

    // SOFIA
    addLink(hv, SOFIA, BELGRADE, RAIL);
    addLink(hv, SOFIA, SALONICA, RAIL);
    addLink(hv, SOFIA, VARNA, RAIL);

    // STRASBOURG
    addLink(hv, STRASBOURG, FRANKFURT, RAIL);
    addLink(hv, STRASBOURG, ZURICH, RAIL);

    // SWANSEA
    addLink(hv, SWANSEA, LONDON, RAIL);

    // SZEGED
    addLink(hv, SZEGED, BUDAPEST, RAIL);
    addLink(hv, SZEGED, BELGRADE, RAIL);
    addLink(hv, SZEGED, BUCHAREST, RAIL);

    // VARNA
    addLink(hv, VARNA, SOFIA, RAIL);

    // VENICE
    addLink(hv, VENICE, VIENNA, RAIL);

    // VIENNA
    addLink(hv, VIENNA, PRAGUE, RAIL);
    addLink(hv, VIENNA, VENICE, RAIL);
    addLink(hv, VIENNA, BUDAPEST, RAIL);

    // ZURICH
    addLink(hv, ZURICH, MILAN, RAIL);
    addLink(hv, ZURICH, STRASBOURG, RAIL);

    /* SEA Connections */

    // NORTH_SEA
    addLink(hv, NORTH_SEA, ATLANTIC_OCEAN, SEA);
    addLink(hv, NORTH_SEA, ENGLISH_CHANNEL, SEA);
    addLink(hv, NORTH_SEA, AMSTERDAM, SEA);
    addLink(hv, NORTH_SEA, EDINBURGH, SEA);
    addLink(hv, NORTH_SEA, HAMBURG, SEA);

    // ENGLISH_CHANNEL
    addLink(hv, ENGLISH_CHANNEL, ATLANTIC_OCEAN, SEA);
    addLink(hv, ENGLISH_CHANNEL, NORTH_SEA, SEA);
    addLink(hv, ENGLISH_CHANNEL, LE_HAVRE, SEA);
    addLink(hv, ENGLISH_CHANNEL, LONDON, SEA);
    addLink(hv, ENGLISH_CHANNEL, PLYMOUTH, SEA);

    // IRISH_SEA
    addLink(hv, IRISH_SEA, ATLANTIC_OCEAN, SEA);
    addLink(hv, IRISH_SEA, DUBLIN, SEA);
    addLink(hv, IRISH_SEA, LIVERPOOL, SEA);
    addLink(hv, IRISH_SEA, SWANSEA, SEA);

    // ATLANTIC_OCEAN
    addLink(hv, ATLANTIC_OCEAN, NORTH_SEA, SEA);
    addLink(hv, ATLANTIC_OCEAN, BAY_OF_BISCAY, SEA);
    addLink(hv, ATLANTIC_OCEAN, IRISH_SEA, SEA);
    addLink(hv, ATLANTIC_OCEAN, ENGLISH_CHANNEL, SEA);
    addLink(hv, ATLANTIC_OCEAN, MEDITERRANEAN_SEA, SEA);
    addLink(hv, ATLANTIC_OCEAN, CADIZ, SEA);
    addLink(hv, ATLANTIC_OCEAN, GALWAY, SEA);
    addLink(hv, ATLANTIC_OCEAN, LISBON, SEA);

    // BAY_OF_BISCAY
    addLink(hv, BAY_OF_BISCAY, ATLANTIC_OCEAN, SEA);
    addLink(hv, BAY_OF_BISCAY, BORDEAUX, SEA);
    addLink(hv, BAY_OF_BISCAY, NANTES, SEA);
    addLink(hv, BAY_OF_BISCAY, SANTANDER, SEA);

    // MEDITERRANEAN_SEA
    addLink(hv, MEDITERRANEAN_SEA, ATLANTIC_OCEAN, SEA);
    addLink(hv, MEDITERRANEAN_SEA, TYRRHENIAN_SEA, SEA);
    addLink(hv, MEDITERRANEAN_SEA, ALICANTE, SEA);
    addLink(hv, MEDITERRANEAN_SEA, BARCELONA, SEA);
    addLink(hv, MEDITERRANEAN_SEA, CAGLIARI, SEA);
    addLink(hv, MEDITERRANEAN_SEA, MARSEILLES, SEA);

    // TYRRHENIAN SEA
    addLink(hv, TYRRHENIAN_SEA, MEDITERRANEAN_SEA, SEA);
    addLink(hv, TYRRHENIAN_SEA, IONIAN_SEA, SEA);
    addLink(hv, TYRRHENIAN_SEA, CAGLIARI, SEA);
    addLink(hv, TYRRHENIAN_SEA, GENOA, SEA);
    addLink(hv, TYRRHENIAN_SEA, NAPLES, SEA);
    addLink(hv, TYRRHENIAN_SEA, ROME, SEA);

    // IONIAN SEA
    addLink(hv, IONIAN_SEA, TYRRHENIAN_SEA, SEA);
    addLink(hv, IONIAN_SEA, ADRIATIC_SEA, SEA);
    addLink(hv, IONIAN_SEA, BLACK_SEA, SEA);
    addLink(hv, IONIAN_SEA, ATHENS, SEA);
    addLink(hv, IONIAN_SEA, SALONICA, SEA);
    addLink(hv, IONIAN_SEA, VALONA, SEA);

    // ADRIATIC SEA
    addLink(hv, ADRIATIC_SEA, IONIAN_SEA, SEA);
    addLink(hv, ADRIATIC_SEA, BARI, SEA);
    addLink(hv, ADRIATIC_SEA, VENICE, SEA);

    // BLACK SEA
    addLink(hv, BLACK_SEA, IONIAN_SEA, SEA);
    addLink(hv, BLACK_SEA, CONSTANTA, SEA);
    addLink(hv, BLACK_SEA, VARNA, SEA);

    /* CITIES->SEA Connections */

    addLink(hv, CONSTANTA, BLACK_SEA, SEA);
    addLink(hv, VARNA, BLACK_SEA, SEA);
    addLink(hv, SALONICA, IONIAN_SEA, SEA);
    addLink(hv, ATHENS, IONIAN_SEA, SEA);
    addLink(hv, VALONA, IONIAN_SEA, SEA);
    addLink(hv, BARI, ADRIATIC_SEA, SEA);
    addLink(hv, VENICE, ADRIATIC_SEA, SEA);
    addLink(hv, NAPLES, TYRRHENIAN_SEA, SEA);
    addLink(hv, ROME, TYRRHENIAN_SEA, SEA);
    addLink(hv, GENOA, TYRRHENIAN_SEA, SEA);
    addLink(hv, CAGLIARI, TYRRHENIAN_SEA, SEA);
    addLink(hv, CAGLIARI, MEDITERRANEAN_SEA, SEA);
    addLink(hv, MARSEILLES, MEDITERRANEAN_SEA, SEA);
    addLink(hv, BARCELONA, MEDITERRANEAN_SEA, SEA);
    addLink(hv, ALICANTE, MEDITERRANEAN_SEA, SEA);
    addLink(hv, CADIZ, ATLANTIC_OCEAN, SEA);
    addLink(hv, LISBON, ATLANTIC_OCEAN, SEA);
    addLink(hv, GALWAY, ATLANTIC_OCEAN, SEA);
    addLink(hv, SANTANDER, BAY_OF_BISCAY, SEA);
    addLink(hv, BORDEAUX, BAY_OF_BISCAY, SEA);
    addLink(hv, NANTES, BAY_OF_BISCAY, SEA);
    addLink(hv, LONDON, ENGLISH_CHANNEL, SEA);
    addLink(hv, PLYMOUTH, ENGLISH_CHANNEL, SEA);
    addLink(hv, LE_HAVRE, ENGLISH_CHANNEL, SEA);
    addLink(hv, SWANSEA, IRISH_SEA, SEA);
    addLink(hv, LIVERPOOL, IRISH_SEA, SEA);
    addLink(hv, DUBLIN, IRISH_SEA, SEA);
    addLink(hv, EDINBURGH, NORTH_SEA, SEA);
    addLink(hv, AMSTERDAM, NORTH_SEA, SEA);
    addLink(hv, HAMBURG, NORTH_SEA, SEA);
}

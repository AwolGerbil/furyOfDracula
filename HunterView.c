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
static char locNum (char* locCode);
static int playerTurns (HunterView hv, int player);

//Graph-related functions
static void addLink (HunterView hv, int start, int finish, int type);
static void makeGraph (HunterView hv);

static void canReachInN (HunterView currentView, LocationID start, int locs[], int n, int type);

typedef struct hunterView {
    char turns; // Number of Turns
    int score; // Current Score

    // Move Tracker
    char move[NUM_PLAYERS][MAX_ROUNDS];

    // Location Tracker
    char location[NUM_PLAYERS][MAX_ROUNDS];

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
    for (i=0;i<NUM_PLAYERS;i++) {
        if (i == PLAYER_DRACULA) {
            hv->health[i][0] = GAME_START_BLOOD_POINTS;
        } else {
            hv->health[i][0] = GAME_START_HUNTER_LIFE_POINTS;
        }
        hv->location[i][0] = UNKNOWN_LOCATION;
    }

    // Iterate through Player Turns
    int turn, round;

    for (turn = 0; turn < hv->turns; turn++) {
        round = ((turn) / 5) + 1;

        if (round >= 1 && turn % NUM_PLAYERS == 0) {
            hv->health[PLAYER_DRACULA][round] = hv->health[PLAYER_DRACULA][round-1];
        }

        if (turn % NUM_PLAYERS == PLAYER_DRACULA) { // Dracula's Move
            draculaMove(hv, &pastPlays[turn * 8], round);
        } else { // Hunter's Move
            hunterMove(hv, &pastPlays[turn * 8], round);
        }
    }

    //Initialise adjacent matrix
    for (i = 0; i < NUM_MAP_LOCATIONS; i++) {
        for (j = 0; j < NUM_MAP_LOCATIONS; j++) {
            hv->adjMatrix[i][j] = NO_EDGE;
        }
    }

    // Populate Adjacency Matrix
    makeGraph(hv);

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

    // Update Score for Dracula's Turn
    hv -> score -= SCORE_LOSS_DRACULA_TURN;

    // Update Move for Dracula's Turn
    char buffer[3];
    buffer[0] = play[1];
    buffer[1] = play[2];
    buffer[2] = 0;
    char currMove = locNum(buffer); // Could be Hide/Double Back/TP
    hv->move[PLAYER_DRACULA][round] = currMove;

    // Update Location for Dracula's Turn
    char currLocation = currMove;
    char offset = 0;
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
    hv->health[hunter][round] = hv->health[hunter][round-1];

    // Update Move and Location for Hunter's Turn
    char buffer[3];
    buffer[0] = play[1];
    buffer[1] = play[2];
    buffer[2] = 0;
    char currLocation = locNum(buffer);

    char currMove = currLocation;
    if (round && currLocation == hv->location[hunter][round - 1]) {
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
            hv->health[PLAYER_DRACULA][round] -= LIFE_LOSS_HUNTER_ENCOUNTER;
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
static char locNum (char* locCode) {
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
    char retVal;
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
    if (getRound(hv) == 0) {
        return 0;
    }

    int retVal = getRound(hv);
    if (((getCurrentPlayer(hv) + 4) % 5) < player) {
        retVal--;
    }

    return retVal;
}

// Frees all memory previously allocated for the HunterView
// toBeDeleted. toBeDeleted should not be accessed after the call.
void disposeHunterView (HunterView toBeDeleted) {
    free(toBeDeleted);
}

Round getRound (HunterView currentView) {
    if(currentView->turns == 0){
        return 0;
    }

    return ((currentView->turns - 1) / 5) + 1;
}

PlayerID getCurrentPlayer (HunterView currentView) {
    return ((currentView->turns) % 5);
}

int getScore (HunterView currentView) {
    return currentView->score;
}

int getHealth (HunterView currentView, PlayerID player) {
    return currentView->health[player][getRound(currentView)];
}

LocationID getLocation(HunterView currentView, PlayerID player) {
    if (player >= PLAYER_LORD_GODALMING
        && player <= PLAYER_MINA_HARKER) { // If Player is a Hunter
        if (currentView->isInHospital[player][playerTurns(currentView, player)]) {
            return ST_JOSEPH_AND_ST_MARYS;
        } else {
            return currentView->location[player][playerTurns(currentView, player)];
        }
    } else { // If Player is Dracula
        return currentView->move[player][playerTurns(currentView, player)];
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
            trail[i] = currentView->move[player][pTurns - i];
        } else { // Hunter
            trail[i] = currentView->location[player][pTurns - i];
        }
    }
}

LocationID* connectedLocations (HunterView currentView,
                               int * numLocations, LocationID from,
                               PlayerID player, Round round,
                               int road, int rail, int sea) {
    int locs[NUM_MAP_LOCATIONS];
    int i;
    for(i=0;i<NUM_MAP_LOCATIONS;i++){
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
    for (i=0;i<NUM_MAP_LOCATIONS;i++) {
        if(locs[i] != -1){
            *numLocations++;
        }
    }

    LocationID *retVal = malloc(sizeof(LocationID) * (*numLocations));

    int j = 0;
    for (i=0;i<NUM_MAP_LOCATIONS;i++) {
        if (locs[i] != -1) {
            retVal[j] = i;
            j++;
        }
    }

    return retVal;
}

//Locs stores the min. number of moves to get to a location
//-1 if unreachable, 0 if reachable in n, ...
static void canReachInN (HunterView currentView, LocationID start, int locs[], int n, int type) {
    locs[start] = n;

    if (n == 0) {
        return;
    }

    int i;
    int edge;
    for (i=0;i<NUM_MAP_LOCATIONS;i++) {
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

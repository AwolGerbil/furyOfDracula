#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "game.h"
#include "HunterView.h"
#include "hunter.h"

void decideMove (HunterView gameState) {
    int player = getCurrentPlayer(gameState);
    int location = getLocation(gameState, player);
    int nextLoc;

    if (!getRound(gameState)) { // First round
        if (player == 0) {
            registerBestPlay(locCode(MUNICH), "");
        } else if (player == 1) {
            registerBestPlay(locCode(STRASBOURG), "");
        } else if (player == 2) {
            registerBestPlay(locCode(SARAGOSA), "");
        } else if (player == 3) {
            registerBestPlay(locCode(KLAUSENBURG), "");
        }
    } else {
        registerBestPLay(locCode(getLocation(gameState, getCurrentPlayer)));
    }
}

static char* locCode (int locID) {
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
    return locCodes[locID];
}

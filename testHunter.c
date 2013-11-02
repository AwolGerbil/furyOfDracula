#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "game.h"
#include "HunterView.h"
#include "hunter.h"

int main (int argc, char* argv[]) {
    playerMessage messages[] = {""};
    HunterView testView;
    
    printf("Test 1: First Round\n");
    
    printf("Player 0\n\n");
    printf("Make HunterView\n");
    testView = newHunterView("", messages);
    decideMove(testView);
    printf("Dispose HunterView\n\n");
    disposeHunterView(testView);
    
    printf("Player 1\n\n");
    printf("Make HunterView\n");
    testView = newHunterView("GMU....", messages);
    decideMove(testView);
    printf("Dispose HunterView\n\n");
    disposeHunterView(testView);
    
    printf("Player 2\n\n");
    printf("Make HunterView\n");
    testView = newHunterView("GMU.... SST....", messages);
    decideMove(testView);
    printf("Dispose HunterView\n\n");
    disposeHunterView(testView);
    
    printf("Player 3\n\n");
    printf("Make HunterView\n");
    testView = newHunterView("GMU.... SST.... HSR....", messages);
    decideMove(testView);
    printf("Dispose HunterView\n\n");
    disposeHunterView(testView);
    
    printf("Test 2: Random Rounds\n");
    
    printf("Player 0\n\n");
    printf("Make HunterView\n");
    testView = newHunterView("GMU.... SST.... HSR.... MKL.... DC?.V..", messages);
    decideMove(testView);
    printf("Dispose HunterView\n\n");
    disposeHunterView(testView);
    
    printf("Player 0\n\n");
    printf("Make HunterView\n");
    testView = newHunterView("GMU.... SST.... HSR.... MKL.... DC?.V..", messages);
    decideMove(testView);
    printf("Dispose HunterView\n\n");
    disposeHunterView(testView);
    
    printf("Player 0\n\n");
    printf("Make HunterView\n");
    testView = newHunterView("GMU.... SST.... HSR.... MKL.... DC?.V..", messages);
    decideMove(testView);
    printf("Dispose HunterView\n\n");
    disposeHunterView(testView);
    
    return 0;
}

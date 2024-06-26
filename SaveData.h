#pragma once

#include "Player.h"
#include "Util.h"

struct GameState 
{
    PlayerData playerData;
    Coord playerCoord;
    UINT walkingSteps;
    float totalTime;
    std::map<ItemID, UINT> items;
    
};
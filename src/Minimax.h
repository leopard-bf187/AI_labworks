
#pragma once

#include "GameTree.h"

#include <cstddef>

struct MinimaxResult
{
    int value = 0;
    unsigned int visitedNodes = 0;
    unsigned int visitedLeaves = 0;
};

MinimaxResult RunMinimax(const GameTree& tree);

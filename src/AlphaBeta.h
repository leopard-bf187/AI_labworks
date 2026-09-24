
#pragma once

#include "GameTree.h"

#include <cstddef>

enum class TraversalOrder
{
    Forward,
    Reverse
};

struct AlphaBetaResult
{
    int value = 0;
    unsigned int visitedNodes = 0;
    unsigned int visitedLeaves = 0;
    unsigned int cutoffCount = 0;
    unsigned int prunedBranches = 0;
};

AlphaBetaResult RunAlphaBeta(const GameTree& tree, TraversalOrder order = TraversalOrder::Forward);

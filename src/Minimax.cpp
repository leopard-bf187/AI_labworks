
#include "Minimax.h"

#include <algorithm>
#include <limits>

namespace
{
    int EvaluateNode(const GameTree& tree, unsigned int nodeIndex, bool maximizingPlayer, MinimaxResult& result)
    {
        const GameNode& node = tree.GetNode(nodeIndex);
        ++result.visitedNodes;

        if (node.IsLeaf())
        {
            ++result.visitedLeaves;
            return node.value;
        }

        int bestValue = maximizingPlayer ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();

        for (unsigned int childIndex : node.children)
        {
            const int childValue = EvaluateNode(tree, childIndex, !maximizingPlayer, result);

            bestValue = maximizingPlayer ? std::max(bestValue, childValue) : std::min(bestValue, childValue);
        }

        return bestValue;
    }
}

MinimaxResult RunMinimax(const GameTree& tree)
{
    MinimaxResult result;

    result.value = EvaluateNode(tree, tree.GetRootIndex(), true, result);

    return result;
}

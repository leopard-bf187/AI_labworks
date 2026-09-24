
#include "AlphaBeta.h"

#include <algorithm>
#include <limits>

namespace
{
    int EvaluateNodeAlphaBeta(const GameTree& tree, unsigned int nodeIndex, bool maximizingPlayer, int alpha, int beta, TraversalOrder order, AlphaBetaResult& result)
    {
        const GameNode& node = tree.GetNode(nodeIndex);
        ++result.visitedNodes;

        if (node.IsLeaf())
        {
            ++result.visitedLeaves;
            return node.value;
        }

        int bestValue = maximizingPlayer ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
        const unsigned int childCount = node.children.size();

        for (unsigned int position = 0; position < childCount; ++position)
        {
            const unsigned int orderedPosition = (order == TraversalOrder::Forward) ? position : childCount - position - 1;
            const unsigned int childIndex = node.children[orderedPosition];

            const int childValue = EvaluateNodeAlphaBeta(tree, childIndex, !maximizingPlayer, alpha, beta, order, result);

            if (maximizingPlayer)
            {
                bestValue = std::max(bestValue, childValue);
                alpha = std::max(alpha, bestValue);
            }
            else
            {
                bestValue = std::min(bestValue, childValue);
                beta = std::min(beta, bestValue);
            }

            const unsigned int remainingChildren = childCount - position - 1;

            if (alpha >= beta && remainingChildren > 0)
            {
                ++result.cutoffCount;
                result.prunedBranches += remainingChildren;
                break;
            }
        }

        return bestValue;
    }
}

AlphaBetaResult RunAlphaBeta(const GameTree& tree, TraversalOrder order)
{
    AlphaBetaResult result;

    result.value = EvaluateNodeAlphaBeta(tree, tree.GetRootIndex(), true, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), order, result);

    return result;
}

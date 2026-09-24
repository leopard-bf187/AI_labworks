
#include "AlphaBeta.h"
#include "GameTree.h"
#include "Minimax.h"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <stdexcept>

#define CHECK(condition) do { if (!(condition)) throw std::runtime_error("TraversalTests: " #condition); } while (false)

void TestOrdersOnSameTree()
{
    unsigned int differentVisits = 0;

    for (unsigned int depth = 1; depth <= 7; ++depth)
    {
        for (unsigned int width : {2u, 3u, 4u})
        {
            for (std::uint32_t seed = 0; seed < 10; ++seed)
            {
                const GameTree tree(depth, width, seed);

                const MinimaxResult reference = RunMinimax(tree);

                const AlphaBetaResult forward = RunAlphaBeta(tree, TraversalOrder::Forward);
                const AlphaBetaResult reverse = RunAlphaBeta(tree, TraversalOrder::Reverse);
                const AlphaBetaResult repeatedForward = RunAlphaBeta(tree, TraversalOrder::Forward);

                CHECK(forward.value == reference.value);
                CHECK(reverse.value == reference.value);

                CHECK(forward.visitedNodes <= reference.visitedNodes);
                CHECK(reverse.visitedNodes <= reference.visitedNodes);

                CHECK(forward.visitedLeaves <= reference.visitedLeaves);
                CHECK(reverse.visitedLeaves <= reference.visitedLeaves);

                CHECK(forward.visitedNodes == repeatedForward.visitedNodes);
                CHECK(forward.visitedLeaves == repeatedForward.visitedLeaves);
                CHECK(forward.cutoffCount == repeatedForward.cutoffCount);
                CHECK(forward.prunedBranches == repeatedForward.prunedBranches);
                CHECK(forward.value == repeatedForward.value);

                if (forward.visitedNodes != reverse.visitedNodes)
                {
                    ++differentVisits;
                }
            }
        }
    }

    CHECK(differentVisits > 0);
}

void TestVariant6()
{
    const GameTree tree(7, 3, 42);

    const MinimaxResult reference = RunMinimax(tree);

    const AlphaBetaResult forward = RunAlphaBeta(tree, TraversalOrder::Forward);
    const AlphaBetaResult reverse = RunAlphaBeta(tree, TraversalOrder::Reverse);

    CHECK(reference.value == forward.value);
    CHECK(reference.value == reverse.value);

    CHECK(reference.visitedNodes == 3280);
    CHECK(reference.visitedLeaves == 2187);

    CHECK(forward.visitedNodes <= reference.visitedNodes);
    CHECK(reverse.visitedNodes <= reference.visitedNodes);
}

int main()
{
    TestOrdersOnSameTree();
    TestVariant6();

    std::cout << "TraversalTests: PASS (same values, differing traversal statistics, unchanged tree)\n";

    return 0;
}

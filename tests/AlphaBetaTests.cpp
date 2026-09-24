
#include "AlphaBeta.h"
#include "GameTree.h"
#include "Minimax.h"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <stdexcept>

#define CHECK(condition) do { if (!(condition)) throw std::runtime_error("AlphaBetaTests: " #condition); } while (false)

void TestSingleLevel()
{
    const GameTree tree(1, 3, 42);

    const MinimaxResult reference = RunMinimax(tree);
    const AlphaBetaResult result = RunAlphaBeta(tree);

    CHECK(result.value == reference.value);
    CHECK(result.visitedNodes == tree.GetNodeCount());
    CHECK(result.visitedLeaves == tree.GetLeafCount());
    CHECK(result.cutoffCount == 0);
    CHECK(result.prunedBranches == 0);
}

void TestManyTrees()
{
    unsigned int treesWithPruning = 0;

    for (unsigned int depth = 2; depth <= 5; ++depth)
    {
        for (unsigned int width : {2u, 3u, 4u})
        {
            for (std::uint32_t seed = 0; seed < 20; ++seed)
            {
                const GameTree tree(depth, width, seed);

                const MinimaxResult reference = RunMinimax(tree);
                const AlphaBetaResult result = RunAlphaBeta(tree);

                CHECK(result.value == reference.value);

                CHECK(result.visitedNodes <= reference.visitedNodes);
                CHECK(result.visitedLeaves <= reference.visitedLeaves);
                CHECK(result.visitedNodes >= result.visitedLeaves);

                CHECK(result.cutoffCount <= result.prunedBranches);
                CHECK(result.prunedBranches <= tree.GetNodeCount() - result.visitedNodes);

                CHECK(result.visitedNodes < reference.visitedNodes || result.prunedBranches == 0);
                CHECK(result.visitedNodes == reference.visitedNodes || result.prunedBranches > 0);

                if (result.prunedBranches > 0)
                {
                    ++treesWithPruning;
                }
            }
        }
    }

    CHECK(treesWithPruning > 0);
}

void TestVariant6()
{
    const GameTree tree(7, 3, 42);

    const MinimaxResult reference = RunMinimax(tree);
    const AlphaBetaResult result = RunAlphaBeta(tree);

    CHECK(reference.visitedNodes == 3280);
    CHECK(reference.visitedLeaves == 2187);

    CHECK(result.value == reference.value);

    CHECK(result.visitedNodes < reference.visitedNodes);
    CHECK(result.visitedLeaves < reference.visitedLeaves);

    CHECK(result.cutoffCount > 0);
    CHECK(result.prunedBranches > 0);
}

int main()
{
    TestSingleLevel();
    TestManyTrees();
    TestVariant6();

    std::cout << "AlphaBetaTests: PASS (same result as Minimax, valid pruning counters, multiple depths/widths/seeds, variant 6)\n";

    return 0;
}

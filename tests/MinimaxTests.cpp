
#include "GameTree.h"
#include "Minimax.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>

#define CHECK(condition) do { if (!(condition)) throw std::runtime_error("MinimaxTests: " #condition); } while (false)

int BottomUpReference(const GameTree& tree)
{
    std::vector<int> values(tree.GetNodeCount(), 0);

    for (unsigned int index = tree.GetNodeCount(); index-- > 0;)
    {
        const GameNode& node = tree.GetNode(index);

        if (node.IsLeaf())
        {
            values[index] = node.value;
            continue;
        }

        const bool maximizingPlayer = (node.depth % 2 == 0);

        int best = maximizingPlayer ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();

        for (unsigned int childIndex : node.children)
        {
            best = maximizingPlayer ? std::max(best, values.at(childIndex)) : std::min(best, values.at(childIndex));
        }

        values[index] = best;
    }

    return values.at(tree.GetRootIndex());
}

void TestSmallTree()
{
    const GameTree tree(1, 3, 42);
    const MinimaxResult result = RunMinimax(tree);

    const GameNode& root = tree.GetNode(tree.GetRootIndex());

    int expected = std::numeric_limits<int>::min();

    for (unsigned int childIndex : root.children)
    {
        expected = std::max(expected, tree.GetNode(childIndex).value);
    }

    CHECK(result.value == expected);
    CHECK(result.visitedNodes == 4);
    CHECK(result.visitedLeaves == 3);
}

void TestDifferentDepthsAndSeeds()
{
    for (unsigned int depth = 1; depth <= 4; ++depth)
    {
        for (std::uint32_t seed : {0u, 42u, 2026u})
        {
            const GameTree tree(depth, 3, seed);
            const MinimaxResult result = RunMinimax(tree);

            CHECK(result.value == BottomUpReference(tree));
            CHECK(result.visitedNodes == tree.GetNodeCount());
            CHECK(result.visitedLeaves == tree.GetLeafCount());
            CHECK(result.value >= -100 && result.value <= 100);
        }
    }
}

void TestVariant6()
{
    const GameTree tree(7, 3, 42);
    const MinimaxResult result = RunMinimax(tree);

    CHECK(result.value == BottomUpReference(tree));
    CHECK(result.visitedNodes == 3280);
    CHECK(result.visitedLeaves == 2187);
}

int main()
{
    TestSmallTree();
    TestDifferentDepthsAndSeeds();
    TestVariant6();

    std::cout << "MinimaxTests: PASS (small tree, independent bottom-up reference, multiple seeds, variant 6)\n";

    return 0;
}

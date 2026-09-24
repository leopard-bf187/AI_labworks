
#include "GameTree.h"

#include <cstddef>
#include <iostream>
#include <stdexcept>

#define CHECK(condition) do { if (!(condition)) throw std::runtime_error("GameTreeTests: " #condition); } while (false)

int main()
{
    const GameTree tree(7, 3, 42);
    const GameTree sameSeedTree(7, 3, 42);

    CHECK(tree.GetRootIndex() == 0);
    CHECK(tree.GetNodeCount() == 3280);
    CHECK(tree.GetLeafCount() == 2187);
    CHECK(tree.GetLevelCounts().size() == 8);

    unsigned int expectedLevelCount = 1;

    for (unsigned int depth = 0; depth <= 7; ++depth)
    {
        CHECK(tree.GetLevelCounts()[depth] == expectedLevelCount);
        expectedLevelCount *= 3;
    }

    unsigned int countedLeaves = 0;

    for (unsigned int index = 0; index < tree.GetNodeCount(); ++index)
    {
        const GameNode& node = tree.GetNode(index);
        const GameNode& repeatedNode = sameSeedTree.GetNode(index);

        CHECK(node.depth == repeatedNode.depth);
        CHECK(node.children == repeatedNode.children);

        if (node.IsLeaf())
        {
            ++countedLeaves;

            CHECK(node.depth == 7);
            CHECK(node.value >= -100 && node.value <= 100);
            CHECK(node.value == repeatedNode.value);
        }
        else
        {
            CHECK(node.depth < 7);
            CHECK(node.children.size() == 3);

            for (unsigned int childIndex : node.children)
            {
                CHECK(childIndex < tree.GetNodeCount());
                CHECK(tree.GetNode(childIndex).depth == node.depth + 1);
            }
        }
    }

    CHECK(countedLeaves == tree.GetLeafCount());

    std::cout << "GameTreeTests: PASS (structure, node counts, leaf ranges, seed reproducibility)\n";

    return 0;
}

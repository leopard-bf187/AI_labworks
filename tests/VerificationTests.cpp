
#include "AlphaBeta.h"
#include "GameTree.h"
#include "Minimax.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

#define CHECK(condition) do { if (!(condition)) throw std::runtime_error(std::string("VerificationTests: ") + #condition + " at line " + std::to_string(__LINE__)); } while (false)

namespace
{
    unsigned int checkedTrees = 0;

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

            const bool maximizing = node.depth % 2 == 0;

            int best = maximizing ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();

            for (unsigned int child : node.children)
            {
                best = maximizing ? std::max(best, values[child]) : std::min(best, values[child]);
            }

            values[index] = best;
        }

        return values[tree.GetRootIndex()];
    }

    void VerifyTree(const GameTree& tree)
    {
        const int expected = BottomUpReference(tree);

        const MinimaxResult minimax = RunMinimax(tree);

        const AlphaBetaResult forward = RunAlphaBeta(tree, TraversalOrder::Forward);
        const AlphaBetaResult reverse = RunAlphaBeta(tree, TraversalOrder::Reverse);

        CHECK(minimax.value == expected);
        CHECK(forward.value == expected);
        CHECK(reverse.value == expected);

        CHECK(minimax.visitedNodes == tree.GetNodeCount());
        CHECK(minimax.visitedLeaves == tree.GetLeafCount());

        for (const AlphaBetaResult& result : {forward, reverse})
        {
            CHECK(result.visitedNodes >= result.visitedLeaves);
            CHECK(result.visitedNodes <= tree.GetNodeCount());
            CHECK(result.visitedLeaves <= tree.GetLeafCount());

            CHECK(result.cutoffCount <= result.prunedBranches);
            CHECK(result.prunedBranches <= tree.GetNodeCount() - result.visitedNodes);

            CHECK(result.visitedNodes == tree.GetNodeCount() || result.prunedBranches > 0);
        }

        ++checkedTrees;
    }

    void TestKnownTrees()
    {
        const GameTree onePly(1, 3, std::vector<int>{-100, 0, 100});

        CHECK(BottomUpReference(onePly) == 100);
        VerifyTree(onePly);

        const GameTree twoPly(2, 2, std::vector<int>{3, 5, 2, 9});

        CHECK(BottomUpReference(twoPly) == 3);
        VerifyTree(twoPly);

        const GameTree threePly(3, 2, std::vector<int>{3, 5, 2, 9, 4, 6, 7, 8});

        CHECK(BottomUpReference(threePly) == 6);
        VerifyTree(threePly);

        for (int value : {-100, -1, 0, 1, 100})
        {
            const GameTree equalLeaves(3, 3, std::vector<int>(27, value));

            CHECK(BottomUpReference(equalLeaves) == value);
            VerifyTree(equalLeaves);
        }

        const GameTree negativeValues(2, 3, std::vector<int>{-100, -90, -80, -70, -60, -50, -40, -30, -20});

        CHECK(BottomUpReference(negativeValues) == -40);
        VerifyTree(negativeValues);

        const GameTree forwardFavorable(2, 2, std::vector<int>{5, 6, 1, 2});
        const GameTree reverseFavorable(2, 2, std::vector<int>{1, 2, 5, 6});

        CHECK(RunAlphaBeta(forwardFavorable, TraversalOrder::Forward).visitedNodes < RunAlphaBeta(forwardFavorable, TraversalOrder::Reverse).visitedNodes);

        CHECK(RunAlphaBeta(reverseFavorable, TraversalOrder::Reverse).visitedNodes < RunAlphaBeta(reverseFavorable, TraversalOrder::Forward).visitedNodes);

        VerifyTree(forwardFavorable);
        VerifyTree(reverseFavorable);
    }

    void TestInvalidInput()
    {
        bool rejectedShort = false;
        bool rejectedLong = false;

        try
        {
            const GameTree tree(2, 2, std::vector<int>{1, 2, 3});
            (void)tree;
        }
        catch (const std::invalid_argument&)
        {
            rejectedShort = true;
        }

        try
        {
            const GameTree tree(2, 2, std::vector<int>{1, 2, 3, 4, 5});
            (void)tree;
        }
        catch (const std::invalid_argument&)
        {
            rejectedLong = true;
        }

        CHECK(rejectedShort && rejectedLong);
    }

    void TestExhaustive(unsigned int depth, unsigned int width)
    {
        unsigned int leafCount = 1;

        for (unsigned int level = 0; level < depth; ++level)
        {
            leafCount *= width;
        }

        std::vector<int> leaves(leafCount, -1);

        unsigned int combinations = 1;

        for (unsigned int i = 0; i < leafCount; ++i)
        {
            combinations *= 3;
        }

        for (unsigned int combination = 0; combination < combinations; ++combination)
        {
            unsigned int code = combination;

            for (unsigned int i = 0; i < leafCount; ++i)
            {
                leaves[i] = static_cast<int>(code % 3) - 1;
                code /= 3;
            }

            VerifyTree(GameTree(depth, width, leaves));
        }
    }

    void TestRandomTrees()
    {
        for (unsigned int depth : {1u, 2u, 3u, 5u, 7u})
        {
            for (unsigned int width : {2u, 3u, 4u})
            {
                for (std::uint32_t seed = 0; seed < 20; ++seed)
                {
                    VerifyTree(GameTree(depth, width, seed));
                }
            }
        }
    }
}

int main()
{
    TestKnownTrees();
    TestInvalidInput();

    TestExhaustive(2, 2);
    TestExhaustive(3, 2);
    TestExhaustive(2, 3);

    TestRandomTrees();

    std::cout << "VerificationTests: PASS (" << checkedTrees << " trees; known, exhaustive, randomized, both traversal orders)\n";

    return 0;
}

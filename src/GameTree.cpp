
#include "GameTree.h"

#include <limits>
#include <stdexcept>

GameTree::GameTree(unsigned int maxDepth, unsigned int branchingFactor, std::uint32_t seed)
    : maxDepth_(maxDepth), branchingFactor_(branchingFactor), seed_(seed), levelCounts_(maxDepth + 1, 0)
{
    if (maxDepth == 0 || branchingFactor < 2)
        throw std::invalid_argument("Tree depth must be >= 1 and branching factor must be >= 2");

    unsigned int totalNodes = 0;
    unsigned int nodesOnLevel = 1;

    for (unsigned int level = 0; level <= maxDepth_; ++level)
    {
        if (nodesOnLevel > std::numeric_limits<unsigned int>::max() - totalNodes)
            throw std::overflow_error("Tree is too large");

        totalNodes += nodesOnLevel;

        if (level != maxDepth_)
        {
            if (nodesOnLevel > std::numeric_limits<unsigned int>::max() / branchingFactor_)
                throw std::overflow_error("Tree is too large");

            nodesOnLevel *= branchingFactor_;
        }
    }

    if (totalNodes > 1'000'000)
        throw std::invalid_argument("Tree is too large for this lab project (max 1000000 nodes)");

    nodes_.reserve(totalNodes);

    std::mt19937 rng(seed_);
    std::uniform_int_distribution<int> distribution(-100, 100);

    GenerateNode(0, rng, distribution);
}

GameTree::GameTree(unsigned int maxDepth, unsigned int branchingFactor, const std::vector<int>& leafValues)
    : GameTree(maxDepth, branchingFactor, std::uint32_t{0})
{
    if (leafValues.size() != leafCount_)
        throw std::invalid_argument("Incorrect leaf value count");

    unsigned int nextLeaf = 0;

    for (GameNode& node : nodes_)
    {
        if (node.IsLeaf())
            node.value = leafValues[nextLeaf++];
    }
}

unsigned int GameTree::GenerateNode(unsigned int depth, std::mt19937& rng, std::uniform_int_distribution<int>& distribution)
{
    const unsigned int index = nodes_.size();

    nodes_.emplace_back();
    nodes_[index].depth = depth;

    ++levelCounts_[depth];

    if (depth == maxDepth_)
    {
        nodes_[index].value = distribution(rng);
        ++leafCount_;

        return index;
    }

    nodes_[index].children.reserve(branchingFactor_);

    for (unsigned int child = 0; child < branchingFactor_; ++child)
    {
        const unsigned int childIndex = GenerateNode(depth + 1, rng, distribution);
        nodes_[index].children.push_back(childIndex);
    }

    return index;
}


#pragma once

#include <cstddef>
#include <cstdint>
#include <random>
#include <vector>

struct GameNode
{
    std::vector<unsigned int> children;
    unsigned int depth = 0;
    int value = 0;

    bool IsLeaf() const { return children.empty(); }
};

class GameTree
{
public:
    GameTree(unsigned int maxDepth, unsigned int branchingFactor, std::uint32_t seed);
    GameTree(unsigned int maxDepth, unsigned int branchingFactor, const std::vector<int>& leafValues);

    const GameNode& GetNode(unsigned int index) const { return nodes_.at(index); }
    unsigned int GetRootIndex() const { return 0; }
    unsigned int GetMaxDepth() const { return maxDepth_; }
    unsigned int GetBranchingFactor() const { return branchingFactor_; }
    unsigned int GetNodeCount() const { return nodes_.size(); }
    unsigned int GetLeafCount() const { return leafCount_; }
    std::uint32_t GetSeed() const { return seed_; }
    const std::vector<unsigned int>& GetLevelCounts() const { return levelCounts_; }

private:
    unsigned int GenerateNode(unsigned int depth, std::mt19937& rng, std::uniform_int_distribution<int>& distribution);

    unsigned int maxDepth_;
    unsigned int branchingFactor_;
    std::uint32_t seed_;
    unsigned int leafCount_ = 0;

    std::vector<GameNode> nodes_;
    std::vector<unsigned int> levelCounts_;
};

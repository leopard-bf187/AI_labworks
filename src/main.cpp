
#include "AlphaBeta.h"
#include "Benchmark.h"
#include "GameTree.h"
#include "Minimax.h"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iomanip>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    try
    {
        if (argc == 2 && std::string(argv[1]) == "--benchmark")
        {
            constexpr unsigned int treeCount = 30;
            constexpr unsigned int repetitions = 200;

            const std::vector<ExperimentRow> rows = RunExperiments(treeCount, repetitions);

            PrintExperimentSummary(rows, repetitions);
            WriteExperimentCsv(rows, "benchmark_results.csv");

            std::cout << "Detailed results: benchmark_results.csv\n";

            return 0;
        }

        if (argc != 1)
        {
            std::cerr << "Usage: AILab2 [--benchmark]\n";
            return 2;
        }

        constexpr unsigned int depth = 7;
        constexpr unsigned int branchingFactor = 3;
        constexpr std::uint32_t seed = 42;

        const GameTree tree(depth, branchingFactor, seed);

        std::cout << "AI Laboratory Work 2 - Final\n";
        std::cout << "Tree depth: " << tree.GetMaxDepth() << ", width: " << tree.GetBranchingFactor() << ", seed: " << tree.GetSeed() << '\n';
        std::cout << "Nodes: " << tree.GetNodeCount() << ", leaves: " << tree.GetLeafCount() << ", leaf range: [-100, 100]\n\n";

        const auto minimaxStart = std::chrono::steady_clock::now();
        const MinimaxResult minimax = RunMinimax(tree);
        const auto minimaxEnd = std::chrono::steady_clock::now();

        const auto forwardStart = std::chrono::steady_clock::now();
        const AlphaBetaResult forward = RunAlphaBeta(tree, TraversalOrder::Forward);
        const auto forwardEnd = std::chrono::steady_clock::now();

        const auto reverseStart = std::chrono::steady_clock::now();
        const AlphaBetaResult reverse = RunAlphaBeta(tree, TraversalOrder::Reverse);
        const auto reverseEnd = std::chrono::steady_clock::now();

        const double minimaxTime = std::chrono::duration<double, std::micro>(minimaxEnd - minimaxStart).count();
        const double forwardTime = std::chrono::duration<double, std::micro>(forwardEnd - forwardStart).count();
        const double reverseTime = std::chrono::duration<double, std::micro>(reverseEnd - reverseStart).count();

        std::cout << std::left << std::setw(25) << "Metric" << std::right << std::setw(14) << "Minimax" << std::setw(14) << "AB forward" << std::setw(14) << "AB reverse" << '\n';

        std::cout << std::left << std::setw(25) << "Root value" << std::right << std::setw(14) << minimax.value << std::setw(14) << forward.value << std::setw(14) << reverse.value << '\n';

        std::cout << std::left << std::setw(25) << "Visited nodes" << std::right << std::setw(14) << minimax.visitedNodes << std::setw(14) << forward.visitedNodes << std::setw(14) << reverse.visitedNodes << '\n';

        std::cout << std::left << std::setw(25) << "Visited leaves" << std::right << std::setw(14) << minimax.visitedLeaves << std::setw(14) << forward.visitedLeaves << std::setw(14) << reverse.visitedLeaves << '\n';

        std::cout << std::left << std::setw(25) << "Cutoff events" << std::right << std::setw(14) << 0 << std::setw(14) << forward.cutoffCount << std::setw(14) << reverse.cutoffCount << '\n';

        std::cout << std::left << std::setw(25) << "Pruned child branches" << std::right << std::setw(14) << 0 << std::setw(14) << forward.prunedBranches << std::setw(14) << reverse.prunedBranches << '\n';

        std::cout << std::left << std::setw(25) << "Time (microseconds)" << std::right << std::fixed << std::setprecision(3) << std::setw(14) << minimaxTime << std::setw(14) << forwardTime << std::setw(14) << reverseTime << '\n';

        const bool valuesMatch = minimax.value == forward.value && minimax.value == reverse.value;

        std::cout << "\nVerification: " << (valuesMatch ? "PASS" : "FAIL") << '\n';
        std::cout << "Run with --benchmark for multi-tree averaged timings and CSV.\n";

        return valuesMatch ? 0 : 1;
    }
    catch (const std::exception& error)
    {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}

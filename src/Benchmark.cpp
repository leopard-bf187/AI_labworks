
#include "Benchmark.h"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace
{
    volatile std::uint64_t g_benchmarkSink = 0;

    TimedResult MeasureMinimax(const GameTree& tree, unsigned int repetitions)
    {
        TimedResult timed;

        const MinimaxResult warmup = RunMinimax(tree);
        const auto start = std::chrono::steady_clock::now();

        for (unsigned int i = 0; i < repetitions; ++i)
        {
            const MinimaxResult result = RunMinimax(tree);

            g_benchmarkSink += static_cast<std::uint64_t>(result.visitedNodes) + static_cast<std::uint64_t>(result.value + 101);
        }

        const auto end = std::chrono::steady_clock::now();

        timed.value = warmup.value;
        timed.visitedNodes = warmup.visitedNodes;
        timed.visitedLeaves = warmup.visitedLeaves;

        timed.microseconds = std::chrono::duration<double, std::micro>(end - start).count() / static_cast<double>(repetitions);

        return timed;
    }

    TimedResult MeasureAlphaBeta(const GameTree& tree, TraversalOrder order, unsigned int repetitions)
    {
        TimedResult timed;

        const AlphaBetaResult warmup = RunAlphaBeta(tree, order);
        const auto start = std::chrono::steady_clock::now();

        for (unsigned int i = 0; i < repetitions; ++i)
        {
            const AlphaBetaResult result = RunAlphaBeta(tree, order);

            g_benchmarkSink += static_cast<std::uint64_t>(result.visitedNodes) + static_cast<std::uint64_t>(result.value + 101);
        }

        const auto end = std::chrono::steady_clock::now();

        timed.value = warmup.value;
        timed.visitedNodes = warmup.visitedNodes;
        timed.visitedLeaves = warmup.visitedLeaves;
        timed.cutoffCount = warmup.cutoffCount;
        timed.prunedBranches = warmup.prunedBranches;

        timed.microseconds = std::chrono::duration<double, std::micro>(end - start).count() / static_cast<double>(repetitions);

        return timed;
    }
}

ExperimentRow RunExperiment(std::uint32_t seed, unsigned int repetitions)
{
    if (repetitions == 0)
        throw std::invalid_argument("Repetitions must be positive");

    const GameTree tree(7, 3, seed);

    ExperimentRow row;
    row.seed = seed;

    switch (seed % 3)
    {
        case 0:
        {
            row.minimax = MeasureMinimax(tree, repetitions);
            row.forward = MeasureAlphaBeta(tree, TraversalOrder::Forward, repetitions);
            row.reverse = MeasureAlphaBeta(tree, TraversalOrder::Reverse, repetitions);
            break;
        }

        case 1:
        {
            row.forward = MeasureAlphaBeta(tree, TraversalOrder::Forward, repetitions);
            row.reverse = MeasureAlphaBeta(tree, TraversalOrder::Reverse, repetitions);
            row.minimax = MeasureMinimax(tree, repetitions);
            break;
        }

        default:
        {
            row.reverse = MeasureAlphaBeta(tree, TraversalOrder::Reverse, repetitions);
            row.minimax = MeasureMinimax(tree, repetitions);
            row.forward = MeasureAlphaBeta(tree, TraversalOrder::Forward, repetitions);
            break;
        }
    }

    if (row.minimax.value != row.forward.value || row.minimax.value != row.reverse.value)
        throw std::runtime_error("Algorithm results differ for seed " + std::to_string(seed));

    return row;
}

std::vector<ExperimentRow> RunExperiments(unsigned int treeCount, unsigned int repetitions)
{
    if (treeCount == 0 || repetitions == 0 || treeCount > static_cast<unsigned int>(std::numeric_limits<std::uint32_t>::max()))
        throw std::invalid_argument("Invalid experiment parameters");

    std::vector<ExperimentRow> rows;
    rows.reserve(treeCount);

    for (unsigned int seed = 0; seed < treeCount; ++seed)
        rows.push_back(RunExperiment(static_cast<std::uint32_t>(seed), repetitions));

    return rows;
}

void PrintExperimentSummary(const std::vector<ExperimentRow>& rows, unsigned int repetitions)
{
    if (rows.empty())
        throw std::invalid_argument("Cannot summarize empty experiment");

    struct Totals
    {
        double nodes = 0.0;
        double leaves = 0.0;
        double cutoffs = 0.0;
        double branches = 0.0;
        double microseconds = 0.0;
    } totals[3];

    unsigned int forwardFewer = 0;
    unsigned int reverseFewer = 0;
    unsigned int tied = 0;

    for (const ExperimentRow& row : rows)
    {
        const TimedResult* values[] = {&row.minimax, &row.forward, &row.reverse};

        for (unsigned int algorithm = 0; algorithm < 3; ++algorithm)
        {
            totals[algorithm].nodes += static_cast<double>(values[algorithm]->visitedNodes);
            totals[algorithm].leaves += static_cast<double>(values[algorithm]->visitedLeaves);
            totals[algorithm].cutoffs += static_cast<double>(values[algorithm]->cutoffCount);
            totals[algorithm].branches += static_cast<double>(values[algorithm]->prunedBranches);
            totals[algorithm].microseconds += values[algorithm]->microseconds;
        }

        if (row.forward.visitedNodes < row.reverse.visitedNodes)
            ++forwardFewer;
        else if (row.forward.visitedNodes > row.reverse.visitedNodes)
            ++reverseFewer;
        else
            ++tied;
    }

    const char* names[] = {"Minimax", "AB forward", "AB reverse"};
    const double count = static_cast<double>(rows.size());

    std::cout << "Trees: " << rows.size() << ", repetitions per algorithm/tree: " << repetitions << '\n';
    std::cout << "Averages (one full search per invocation):\n";

    std::cout << std::left << std::setw(14) << "Algorithm" << std::right << std::setw(14) << "Nodes" << std::setw(14) << "Leaves" << std::setw(14) << "Cutoffs" << std::setw(14) << "Branches" << std::setw(16) << "Time (us)" << '\n';

    for (unsigned int algorithm = 0; algorithm < 3; ++algorithm)
    {
        std::cout << std::left << std::setw(14) << names[algorithm] << std::right << std::fixed << std::setprecision(2) << std::setw(14) << totals[algorithm].nodes / count << std::setw(14) << totals[algorithm].leaves / count << std::setw(14) << totals[algorithm].cutoffs / count << std::setw(14) << totals[algorithm].branches / count << std::setw(16) << totals[algorithm].microseconds / count << '\n';
    }

    std::cout << "Fewer nodes: forward " << forwardFewer << ", reverse " << reverseFewer << ", tie " << tied << '\n';
    std::cout << "All root values matched: PASS\n";
}

void WriteExperimentCsv(const std::vector<ExperimentRow>& rows, const std::string& filename)
{
    std::ofstream output(filename);

    if (!output)
        throw std::runtime_error("Cannot open CSV file: " + filename);

    output << "seed,algorithm,root_value,visited_nodes,visited_leaves,cutoff_events,pruned_child_branches,time_us\n";
    output << std::fixed << std::setprecision(6);

    for (const ExperimentRow& row : rows)
    {
        const TimedResult* results[] = {&row.minimax, &row.forward, &row.reverse};
        const char* names[] = {"minimax", "alpha_beta_forward", "alpha_beta_reverse"};

        for (unsigned int algorithm = 0; algorithm < 3; ++algorithm)
        {
            const TimedResult& result = *results[algorithm];

            output << row.seed << ',' << names[algorithm] << ',' << result.value << ',' << result.visitedNodes << ',' << result.visitedLeaves << ',' << result.cutoffCount << ',' << result.prunedBranches << ',' << result.microseconds << '\n';
        }
    }

    if (!output)
        throw std::runtime_error("Cannot write CSV file: " + filename);
}

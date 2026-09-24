
#pragma once

#include "AlphaBeta.h"
#include "Minimax.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

struct TimedResult
{
    int value = 0;
    unsigned int visitedNodes = 0;
    unsigned int visitedLeaves = 0;
    unsigned int cutoffCount = 0;
    unsigned int prunedBranches = 0;
    double microseconds = 0.0;
};

struct ExperimentRow
{
    std::uint32_t seed = 0;
    TimedResult minimax;
    TimedResult forward;
    TimedResult reverse;
};

ExperimentRow RunExperiment(std::uint32_t seed, unsigned int repetitions);
std::vector<ExperimentRow> RunExperiments(unsigned int treeCount, unsigned int repetitions);
void PrintExperimentSummary(const std::vector<ExperimentRow>& rows, unsigned int repetitions);
void WriteExperimentCsv(const std::vector<ExperimentRow>& rows, const std::string& filename);

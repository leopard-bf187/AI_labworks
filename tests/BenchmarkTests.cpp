
#include "Benchmark.h"

#include <iostream>
#include <stdexcept>
#include <vector>

#define CHECK(condition) do { if (!(condition)) throw std::runtime_error("BenchmarkTests: " #condition); } while (false)

int main()
{
    const std::vector<ExperimentRow> rows = RunExperiments(3, 2);

    CHECK(rows.size() == 3);

    for (const ExperimentRow& row : rows)
    {
        CHECK(row.minimax.value == row.forward.value);
        CHECK(row.minimax.value == row.reverse.value);

        CHECK(row.minimax.visitedNodes == 3280);
        CHECK(row.minimax.visitedLeaves == 2187);

        CHECK(row.forward.visitedNodes <= row.minimax.visitedNodes);
        CHECK(row.reverse.visitedNodes <= row.minimax.visitedNodes);

        CHECK(row.minimax.microseconds >= 0.0);
        CHECK(row.forward.microseconds >= 0.0);
        CHECK(row.reverse.microseconds >= 0.0);
    }

    bool rejectedZeroRepetitions = false;

    try
    {
        (void)RunExperiment(42, 0);
    }
    catch (const std::invalid_argument&)
    {
        rejectedZeroRepetitions = true;
    }

    CHECK(rejectedZeroRepetitions);

    std::cout << "BenchmarkTests: PASS (consistent results, nonnegative timings, invalid parameter)\n";

    return 0;
}

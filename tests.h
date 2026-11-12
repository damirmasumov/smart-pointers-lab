#pragma once
#include <vector>
#include <cstddef>

struct LoadTestResult {
    size_t elements_count;
    double time_raw;
    double time_shrd;
    double time_stl;
};

void RunFunctionalTests();
LoadTestResult RunSingleLoadTest(size_t elements_count);
std::vector<LoadTestResult> RunAllLoadTests();
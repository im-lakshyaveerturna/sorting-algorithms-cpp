#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

// Shared support for the separate execution-time tasks.
constexpr unsigned RANDOM_SEED = 20261005;

inline std::ofstream resultFile(const std::string& filename) {
    std::filesystem::create_directories("results");
    std::ofstream file("results/" + filename);
    if (!file) throw std::runtime_error("Cannot open results/" + filename);
    file << std::fixed << std::setprecision(6);
    return file;
}

template <typename T>
void printArray(const std::vector<T>& values) {
    for (const T& value : values) std::cout << value << ' ';
    std::cout << '\n';
}

// Copying, reference sorting, validation and printing are outside the timer.
// Allocations needed by the algorithm itself are included in its execution time.
template <typename T, typename Sort>
double averageTime(const std::vector<T>& input, Sort sort, int trials,
                   std::ofstream& samples, const std::string& label) {
    if (trials < 1) throw std::invalid_argument("Trials must be positive");
    auto expected = input;
    std::sort(expected.begin(), expected.end());
    auto warmup = input;
    sort(warmup);
    if (warmup != expected) throw std::runtime_error("Warmup failed");
    double total = 0;
    for (int trial = 1; trial <= trials; ++trial) {
        auto values = input;
        auto start = std::chrono::steady_clock::now();
        sort(values);
        auto end = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
        if (values != expected) throw std::runtime_error("Incorrect sorted output");
        total += elapsed;
        samples << label << ',' << input.size() << ',' << trial << ',' << elapsed << '\n';
    }
    return total / trials;
}

template <typename T, typename Sort>
int smallTimingExperiment(const std::string& name, const std::vector<T>& input,
                          Sort sort) {
    try {
        const int trials = 1000;
        auto samples = resultFile(name + "_timing_trials.csv");
        samples << "algorithm,n,trial,milliseconds\n";
        double elapsed = averageTime(input, sort, trials, samples, name);
        auto summary = resultFile(name + "_timing.csv");
        summary << "algorithm,n,trials,average_milliseconds\n"
                << name << ',' << input.size() << ',' << trials << ',' << elapsed << '\n';
        auto sorted = input;
        sort(sorted);
        std::cout << "Input (" << input.size() << " elements): ";
        printArray(input);
        std::cout << "Sorted: ";
        printArray(sorted);
        std::cout << std::fixed << std::setprecision(6)
                  << "Average execution time over " << trials << " trials: "
                  << elapsed << " ms\n";
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}

#endif

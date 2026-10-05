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

// Count only comparisons between array values, not loop/index conditions.
using ComparisonSort = void (*)(std::vector<int>&, std::uint64_t&);
constexpr unsigned RANDOM_SEED = 20261005;

inline std::ofstream resultFile(const std::string& filename) {
    std::filesystem::create_directories("results");
    std::ofstream file("results/" + filename);
    if (!file) throw std::runtime_error("Cannot open results/" + filename);
    file << std::fixed << std::setprecision(6);
    return file;
}

inline int positiveArgument(const char* argument, int maximum) {
    std::string text(argument);
    std::size_t used = 0;
    int value = std::stoi(text, &used);
    if (used != text.size() || value < 1 || value > maximum)
        throw std::invalid_argument("Argument must be between 1 and " +
                                    std::to_string(maximum));
    return value;
}

// Usage: ./program [number_of_trials] [size1 size2 ...]
inline int comparisonExperiment(const std::string& name, ComparisonSort sort,
                                int argc, char* argv[]) {
    try {
        int trials = argc > 1 ? positiveArgument(argv[1], 1000) : 10;
        std::vector<int> sizes = {100, 500, 1000, 2000, 5000, 10000};
        if (argc > 2) {
            sizes.clear();
            for (int i = 2; i < argc; ++i)
                sizes.push_back(positiveArgument(argv[i], 100000));
        }
        auto summary = resultFile(name + "_comparisons.csv");
        auto samples = resultFile(name + "_comparison_trials.csv");
        summary << "n,trials,average_comparisons,n_log2_n,n_log10_n\n";
        samples << "n,trial,comparisons\n";
        std::mt19937 generator(RANDOM_SEED);
        std::cout << name << ": " << trials << " trials per size\n"
                  << "n,average_comparisons,n_log2_n,n_log10_n\n"
                  << std::fixed << std::setprecision(2);
        for (int n : sizes) {
            long double total = 0;
            for (int trial = 1; trial <= trials; ++trial) {
                std::vector<int> values(n);
                std::iota(values.begin(), values.end(), 1);
                std::shuffle(values.begin(), values.end(), generator);
                std::uint64_t comparisons = 0;
                sort(values, comparisons);
                // The input is exactly the permutation 1..n: check every value.
                for (int i = 0; i < n; ++i)
                    if (values[i] != i + 1)
                        throw std::runtime_error("Incorrect sorted output");
                total += comparisons;
                samples << n << ',' << trial << ',' << comparisons << '\n';
            }
            double average = static_cast<double>(total / trials);
            double log2Curve = n * std::log2(n);
            double log10Curve = n * std::log10(n);
            summary << n << ',' << trials << ',' << average << ','
                    << log2Curve << ',' << log10Curve << '\n';
            std::cout << n << ',' << average << ',' << log2Curve << ','
                      << log10Curve << '\n';
        }
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
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

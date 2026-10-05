#include "experiment.h"

// Separate insertion sort task: no comparison counter in the timed algorithm.
void insertionSortTimed(std::vector<int>& values) {
    for (int i = 1; i < static_cast<int>(values.size()); ++i) {
        int key = values[i];
        int j = i - 1;
        while (j >= 0 && values[j] > key) {
            values[j + 1] = values[j];
            --j;
        }
        values[j + 1] = key;
    }
}

#ifndef SORTING_TEST
int main() {
    try {
        const int n = 15000, trials = 5;
        std::vector<int> best(n);
        std::iota(best.begin(), best.end(), 1);
        auto worst = best;
        std::reverse(worst.begin(), worst.end());
        auto samples = resultFile("insertion_sort_timing_trials.csv");
        samples << "case,n,trial,milliseconds\n";
        auto summary = resultFile("insertion_sort_timing.csv");
        summary << "case,n,trials,average_milliseconds\n";
        double bestTime = averageTime(best, insertionSortTimed, trials, samples, "best");
        // Use a fresh random permutation for each average-case trial.
        std::mt19937 generator(RANDOM_SEED);
        double randomTotal = 0;
        auto warmup = best;
        std::shuffle(warmup.begin(), warmup.end(), generator);
        insertionSortTimed(warmup);
        for (int trial = 1; trial <= trials; ++trial) {
            auto values = best;
            std::shuffle(values.begin(), values.end(), generator);
            auto start = std::chrono::steady_clock::now();
            insertionSortTimed(values);
            auto end = std::chrono::steady_clock::now();
            if (values != best) throw std::runtime_error("Incorrect sorted output");
            double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
            randomTotal += elapsed;
            samples << "average," << n << ',' << trial << ',' << elapsed << '\n';
        }
        double average = randomTotal / trials;
        double worstTime = averageTime(worst, insertionSortTimed, trials, samples, "worst");
        summary << "best," << n << ',' << trials << ',' << bestTime << '\n'
                << "average," << n << ',' << trials << ',' << average << '\n'
                << "worst," << n << ',' << trials << ',' << worstTime << '\n';
        std::cout << std::fixed << std::setprecision(6)
                  << "Insertion sort: " << n << " elements, " << trials << " trials\n"
                  << "Best case (ascending): " << bestTime << " ms\n"
                  << "Average case (random): " << average << " ms\n"
                  << "Worst case (descending): " << worstTime << " ms\n";
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
#endif

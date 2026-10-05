#include "experiment.h"

// Lomuto partition: use the last value as pivot.
int partitionValues(std::vector<int>& values, int low, int high,
                    std::uint64_t& comparisons) {
    int pivot = values[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        ++comparisons;
        if (values[j] <= pivot) std::swap(values[++i], values[j]);
    }
    std::swap(values[i + 1], values[high]);
    return i + 1;
}

void quickRange(std::vector<int>& values, int low, int high,
                std::uint64_t& comparisons) {
    // Recurse on the smaller partition, iterate over the larger one.
    // This keeps stack depth O(log n), even with a poor pivot.
    while (low < high) {
        int pivotIndex = partitionValues(values, low, high, comparisons);
        if (pivotIndex - low < high - pivotIndex) {
            quickRange(values, low, pivotIndex - 1, comparisons);
            low = pivotIndex + 1;
        } else {
            quickRange(values, pivotIndex + 1, high, comparisons);
            high = pivotIndex - 1;
        }
    }
}

void quickSort(std::vector<int>& values, std::uint64_t& comparisons) {
    quickRange(values, 0, static_cast<int>(values.size()) - 1, comparisons);
}

#ifndef SORTING_TEST
int main(int argc, char* argv[]) {
    return comparisonExperiment("quick_sort", quickSort, argc, argv);
}
#endif

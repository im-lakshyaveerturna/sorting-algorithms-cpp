#include "experiment.h"

void insertionSort(std::vector<int>& values, std::uint64_t& comparisons) {
    for (int i = 1; i < static_cast<int>(values.size()); ++i) {
        int key = values[i];
        int j = i - 1;
        while (j >= 0) {
            ++comparisons;  // Count even when values[j] > key is false.
            if (values[j] <= key) break;
            values[j + 1] = values[j];
            --j;
        }
        values[j + 1] = key;
    }
}

#ifndef SORTING_TEST
int main(int argc, char* argv[]) {
    return comparisonExperiment("insertion_sort", insertionSort, argc, argv);
}
#endif

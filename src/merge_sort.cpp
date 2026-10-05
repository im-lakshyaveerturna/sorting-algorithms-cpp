#include "experiment.h"

void mergeParts(std::vector<int>& values, int left, int middle, int right,
                std::uint64_t& comparisons) {
    std::vector<int> first(values.begin() + left, values.begin() + middle + 1);
    std::vector<int> second(values.begin() + middle + 1, values.begin() + right + 1);
    std::size_t i = 0, j = 0;
    int k = left;
    while (i < first.size() && j < second.size()) {
        ++comparisons;
        if (first[i] <= second[j]) values[k++] = first[i++];
        else values[k++] = second[j++];
    }
    // Copying remaining elements does not compare array values.
    while (i < first.size()) values[k++] = first[i++];
    while (j < second.size()) values[k++] = second[j++];
}

void mergeRange(std::vector<int>& values, int left, int right,
                std::uint64_t& comparisons) {
    if (left >= right) return;
    int middle = left + (right - left) / 2;
    mergeRange(values, left, middle, comparisons);
    mergeRange(values, middle + 1, right, comparisons);
    mergeParts(values, left, middle, right, comparisons);
}

void mergeSort(std::vector<int>& values, std::uint64_t& comparisons) {
    mergeRange(values, 0, static_cast<int>(values.size()) - 1, comparisons);
}

#ifndef SORTING_TEST
int main(int argc, char* argv[]) {
    return comparisonExperiment("merge_sort", mergeSort, argc, argv);
}
#endif

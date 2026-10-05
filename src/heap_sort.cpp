#include "experiment.h"

void heapify(std::vector<int>& values, int heapSize, int root,
             std::uint64_t& comparisons) {
    int largest = root;
    int left = 2 * root + 1;
    int right = 2 * root + 2;
    if (left < heapSize) {
        ++comparisons;
        if (values[left] > values[largest]) largest = left;
    }
    if (right < heapSize) {
        ++comparisons;
        if (values[right] > values[largest]) largest = right;
    }
    if (largest != root) {
        std::swap(values[root], values[largest]);
        heapify(values, heapSize, largest, comparisons);
    }
}

void heapSort(std::vector<int>& values, std::uint64_t& comparisons) {
    int n = static_cast<int>(values.size());
    for (int i = n / 2 - 1; i >= 0; --i) heapify(values, n, i, comparisons);
    for (int end = n - 1; end > 0; --end) {
        std::swap(values[0], values[end]);
        heapify(values, end, 0, comparisons);
    }
}

#ifndef SORTING_TEST
int main(int argc, char* argv[]) {
    return comparisonExperiment("heap_sort", heapSort, argc, argv);
}
#endif

#include "experiment.h"

// Stable counting sort for nonnegative integers with a modest value range.
void countingSort(std::vector<int>& values) {
    if (values.empty()) return;
    int maximum = 0;
    for (int value : values) {
        if (value < 0 || value > 1000000)
            throw std::invalid_argument("Counting sort supports integers from 0 to 1000000");
        maximum = std::max(maximum, value);
    }
    std::vector<std::size_t> count(static_cast<std::size_t>(maximum) + 1, 0);
    for (int value : values) ++count[value];
    for (int value = 1; value <= maximum; ++value) count[value] += count[value - 1];
    std::vector<int> output(values.size());
    for (int i = static_cast<int>(values.size()) - 1; i >= 0; --i)
        output[--count[values[i]]] = values[i];
    values = output;
}

#ifndef SORTING_TEST
int main() {
    const std::vector<int> input = {170, 45, 75, 90, 802, 24, 2, 66, 0, 999,
        501, 18, 39, 120, 300, 5, 75, 42, 610, 7, 88, 456, 321, 11, 200};
    return smallTimingExperiment("counting_sort", input, countingSort);
}
#endif

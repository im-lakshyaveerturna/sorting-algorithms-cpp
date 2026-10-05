#include "experiment.h"

// LSD radix sort for nonnegative integers, using stable counting by digit.
void radixSort(std::vector<int>& values) {
    if (values.empty()) return;
    int maximum = 0;
    for (int value : values) {
        if (value < 0) throw std::invalid_argument("Radix sort requires nonnegative integers");
        maximum = std::max(maximum, value);
    }
    std::vector<int> output(values.size());
    // 64-bit exponent prevents overflow after the last digit of INT_MAX.
    for (std::int64_t exponent = 1; maximum / exponent > 0; exponent *= 10) {
        int count[10] = {};
        for (int value : values) ++count[(value / exponent) % 10];
        for (int digit = 1; digit < 10; ++digit) count[digit] += count[digit - 1];
        for (int i = static_cast<int>(values.size()) - 1; i >= 0; --i) {
            int digit = static_cast<int>((values[i] / exponent) % 10);
            output[--count[digit]] = values[i];
        }
        values = output;
    }
}

#ifndef SORTING_TEST
int main() {
    const std::vector<int> input = {170, 45, 75, 90, 802, 24, 2, 66, 0, 999,
        501, 18, 39, 120, 300, 5, 75, 42, 610, 7, 88, 456, 321, 11, 200};
    return smallTimingExperiment("radix_sort", input, radixSort);
}
#endif

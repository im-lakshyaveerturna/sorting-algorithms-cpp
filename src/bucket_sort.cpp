#include "experiment.h"

void bucketSort(std::vector<double>& values) {
    const int numberOfBuckets = 10;
    std::vector<std::vector<double>> buckets(numberOfBuckets);
    for (double value : values) {
        if (!std::isfinite(value) || value < 0.0 || value >= 1.0)
            throw std::invalid_argument("Bucket sort requires finite values in [0, 1)");
        int index = static_cast<int>(value * numberOfBuckets);
        index = std::min(index, numberOfBuckets - 1);
        buckets[index].push_back(value);
    }
    std::size_t next = 0;
    for (auto& bucket : buckets) {
        // Insertion sort within each bucket.
        for (int i = 1; i < static_cast<int>(bucket.size()); ++i) {
            double key = bucket[i];
            int j = i - 1;
            while (j >= 0 && bucket[j] > key) {
                bucket[j + 1] = bucket[j];
                --j;
            }
            bucket[j + 1] = key;
        }
        for (double value : bucket) values[next++] = value;
    }
}

#ifndef SORTING_TEST
int main() {
    // The same values used by radix/counting sort, scaled by 1000.
    const std::vector<double> input = {0.170, 0.045, 0.075, 0.090, 0.802,
        0.024, 0.002, 0.066, 0.000, 0.999, 0.501, 0.018, 0.039, 0.120,
        0.300, 0.005, 0.075, 0.042, 0.610, 0.007, 0.088, 0.456, 0.321,
        0.011, 0.200};
    return smallTimingExperiment("bucket_sort", input, bucketSort);
}
#endif

#define SORTING_TEST
#include "../src/insertion_sort.cpp"
#include "../src/merge_sort.cpp"
#include "../src/quick_sort.cpp"
#include "../src/heap_sort.cpp"
#include "../src/insertion_sort_timing.cpp"
#include "../src/radix_sort.cpp"
#include "../src/counting_sort.cpp"
#include "../src/bucket_sort.cpp"
#include <climits>
#include <limits>
#include <vector>
#include <random>
#include <numeric>
#include <stdexcept>
#include <string>

constexpr unsigned RANDOM_SEED = 20261005;

void insertionTimedCheck(std::vector<int>& values) {
    insertionSortTimed(values.data(), static_cast<int>(values.size()));
}

void radixCheck(std::vector<int>& values) {
    if (!radixSort(values.data(), static_cast<int>(values.size())))
        throw std::runtime_error("Radix rejected valid input");
}

void countingCheck(std::vector<int>& values) {
    if (!countingSort(values.data(), static_cast<int>(values.size())))
        throw std::runtime_error("Counting rejected valid input");
}

void bucketCheck(std::vector<double>& values) {
    if (!bucketSort(values.data(), static_cast<int>(values.size())))
        throw std::runtime_error("Bucket rejected valid input");
}

void require(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}

template <typename Sort, typename T>
void checkSort(Sort sort, std::vector<T> values) {
    auto expected = values;
    std::sort(expected.begin(), expected.end());
    sort(values);
    require(values == expected, "Output differs from std::sort");
}

int main() {
    try {
        using ArraySort = void (*)(int[], int, int&);
        ArraySort sorts[] = {insertionSort, mergeSort, quickSort, heapSort};
        std::mt19937 generator(RANDOM_SEED);
        for (ArraySort sort : sorts) {
            auto check = [sort](std::vector<int>& values) {
                int count = 0;
                sort(values.data(), static_cast<int>(values.size()), count);
            };
            for (const auto& input : std::vector<std::vector<int>>{
                    {}, {1}, {2, 1}, {5, 5, 5}, {0, -3, 8, -3, 0},
                    {INT_MAX, 0, INT_MIN, INT_MAX}})
                checkSort(check, input);
            // Exhaustively check every permutation through seven elements.
            for (int n = 0; n <= 7; ++n) {
                std::vector<int> values(n);
                std::iota(values.begin(), values.end(), 0);
                do { checkSort(check, values); }
                while (std::next_permutation(values.begin(), values.end()));
            }
            for (int trial = 0; trial < 50; ++trial) {
                std::vector<int> values(200);
                for (int& value : values)
                    value = std::uniform_int_distribution<int>(-100, 100)(generator);
                checkSort(check, values);
            }
        }
        // Independent exact-count cases catch off-by-one instrumentation errors.
        for (int n = 0; n <= 30; ++n) {
            std::vector<int> values(n);
            std::iota(values.begin(), values.end(), 0);
            int count = 0;
            insertionSort(values.data(), n, count);
            require(count == 0, "Insertion best shift count");
            std::reverse(values.begin(), values.end());
            count = 0;
            insertionSort(values.data(), n, count);
            require(count == n * (n - 1) / 2, "Insertion worst shift count");
            count = 0;
            quickSort(values.data(), n, count);
            require(count == n * (n - 1) / 2, "Quick sorted count");
        }
        std::vector<int> four = {4, 3, 2, 1};
        int count = 0;
        mergeSort(four.data(), 4, count);
        require(count == 4, "Merge four-element count");
        std::vector<int> three = {3, 1, 2};
        count = 0;
        heapSort(three.data(), 3, count);
        require(count == 3, "Heap three-element count");

        // Under the supplied convention, insertion's counter equals the
        // original number of inversions, even when duplicate values occur.
        for (int trial = 0; trial < 50; ++trial) {
            std::vector<int> values(100);
            for (int& value : values)
                value = std::uniform_int_distribution<int>(0, 10)(generator);
            int inversions = 0;
            for (int i = 0; i < 100; ++i)
                for (int j = i + 1; j < 100; ++j)
                    if (values[i] > values[j]) ++inversions;
            count = 0;
            insertionSort(values.data(), 100, count);
            require(count == inversions, "Insertion counter must equal inversions");
        }

        for (const auto& input : std::vector<std::vector<int>>{
                {}, {0}, {0, 0, 0}, {9, 0, 2, 2, 100, 11}, {1000000, 0, 1}}) {
            checkSort(insertionTimedCheck, input);
            checkSort(radixCheck, input);
            checkSort(countingCheck, input);
        }
        checkSort(radixCheck, std::vector<int>{INT_MAX, 0, INT_MAX - 1, 9});
        for (int trial = 0; trial < 50; ++trial) {
            std::vector<int> integers(200);
            std::vector<double> fractions(200);
            for (int i = 0; i < 200; ++i) {
                integers[i] = std::uniform_int_distribution<int>(0, 1000)(generator);
                fractions[i] = std::uniform_real_distribution<double>(0.0, 1.0)(generator);
            }
            checkSort(insertionTimedCheck, integers);
            checkSort(radixCheck, integers);
            checkSort(countingCheck, integers);
            checkSort(bucketCheck, fractions);
        }
        for (const auto& input : std::vector<std::vector<double>>{
                {}, {0}, {0.5, 0.5}, {0.0, std::nextafter(1.0, 0.0), 0.1, 0.01}})
            checkSort(bucketCheck, input);
        int negative[] = {-1}, tooLarge[] = {1000001};
        require(!radixSort(negative, 1), "Radix must reject negative values");
        require(!countingSort(negative, 1), "Counting must reject negative values");
        require(!countingSort(tooLarge, 1), "Counting range limit");
        for (double invalid : {-0.1, 1.0, std::numeric_limits<double>::infinity(),
                               std::numeric_limits<double>::quiet_NaN()})
        {
            double value[] = {invalid};
            require(!bucketSort(value, 1), "Bucket must reject invalid values");
        }
        std::vector<double> fullBucket(1000, 0.5);
        checkSort(bucketCheck, fullBucket);
        fullBucket.push_back(0.5);
        checkSort(bucketCheck, fullBucket);
        for (int caseNumber = 0; caseNumber < 3; ++caseNumber) {
            std::vector<int> values(15000);
            prepareInsertionInput(values.data(), 15000, caseNumber);
            if (caseNumber == 0) require(std::is_sorted(values.begin(), values.end()), "Best input order");
            if (caseNumber == 2) require(std::is_sorted(values.rbegin(), values.rend()), "Worst input order");
            std::sort(values.begin(), values.end());
            for (int i = 0; i < 15000; ++i)
                require(values[i] == i + 1, "Timing input must be a full permutation");
        }
        std::cout << "All correctness, comparison-count and input-domain checks passed.\n";
    } catch (const std::exception& error) {
        std::cerr << "Test failure: " << error.what() << '\n';
        return 1;
    }
    return 0;
}

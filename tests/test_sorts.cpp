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

template <typename Action>
void expectInvalid(Action action) {
    bool rejected = false;
    try { action(); } catch (const std::invalid_argument&) { rejected = true; }
    require(rejected, "Expected invalid input rejection");
}

int main() {
    try {
        ComparisonSort sorts[] = {insertionSort, mergeSort, quickSort, heapSort};
        std::mt19937 generator(RANDOM_SEED);
        for (ComparisonSort sort : sorts) {
            auto check = [sort](std::vector<int>& values) {
                std::uint64_t count = 0;
                sort(values, count);
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
            std::uint64_t count = 0;
            insertionSort(values, count);
            require(count == static_cast<std::uint64_t>(std::max(0, n - 1)), "Insertion best count");
            std::reverse(values.begin(), values.end());
            count = 0;
            insertionSort(values, count);
            require(count == static_cast<std::uint64_t>(n) * (n ? n - 1 : 0) / 2, "Insertion worst count");
            count = 0;
            quickSort(values, count);
            require(count == static_cast<std::uint64_t>(n) * (n ? n - 1 : 0) / 2, "Quick sorted count");
        }
        std::vector<int> four = {4, 3, 2, 1};
        std::uint64_t count = 0;
        mergeSort(four, count);
        require(count == 4, "Merge four-element count");
        std::vector<int> three = {3, 1, 2};
        count = 0;
        heapSort(three, count);
        require(count == 3, "Heap three-element count");

        for (const auto& input : std::vector<std::vector<int>>{
                {}, {0}, {0, 0, 0}, {9, 0, 2, 2, 100, 11}, {1000000, 0, 1}}) {
            checkSort(insertionSortTimed, input);
            checkSort(radixSort, input);
            checkSort(countingSort, input);
        }
        checkSort(radixSort, std::vector<int>{INT_MAX, 0, INT_MAX - 1, 9});
        for (int trial = 0; trial < 50; ++trial) {
            std::vector<int> integers(200);
            std::vector<double> fractions(200);
            for (int i = 0; i < 200; ++i) {
                integers[i] = std::uniform_int_distribution<int>(0, 1000)(generator);
                fractions[i] = std::uniform_real_distribution<double>(0.0, 1.0)(generator);
            }
            checkSort(insertionSortTimed, integers);
            checkSort(radixSort, integers);
            checkSort(countingSort, integers);
            checkSort(bucketSort, fractions);
        }
        for (const auto& input : std::vector<std::vector<double>>{
                {}, {0}, {0.5, 0.5}, {0.0, std::nextafter(1.0, 0.0), 0.1, 0.01}})
            checkSort(bucketSort, input);
        expectInvalid([] { std::vector<int> v{-1}; radixSort(v); });
        expectInvalid([] { std::vector<int> v{-1}; countingSort(v); });
        expectInvalid([] { std::vector<int> v{1000001}; countingSort(v); });
        for (double invalid : {-0.1, 1.0, std::numeric_limits<double>::infinity(),
                               std::numeric_limits<double>::quiet_NaN()})
            expectInvalid([invalid] { std::vector<double> v{invalid}; bucketSort(v); });
        std::cout << "All correctness, comparison-count and input-domain checks passed.\n";
    } catch (const std::exception& error) {
        std::cerr << "Test failure: " << error.what() << '\n';
        return 1;
    }
    return 0;
}

#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <cmath>
#include <vector>

using namespace std;

// Aim: Sort fractions in [0, 1) using textbook bucket sort.
void insertionSortBucket(vector<double> &bucket) {
    for (int i = 1; i < static_cast<int>(bucket.size()); i++) {
        double key = bucket[i];
        int j = i - 1;
        while (j >= 0 && bucket[j] > key) {
            bucket[j + 1] = bucket[j];
            j--;
        }
        bucket[j + 1] = key;
    }
}

bool bucketSort(double arr[], int n) {
    if (n < 0) return false;
    if (n == 0) return true;
    for (int i = 0; i < n; i++)
        if (!isfinite(arr[i]) || arr[i] < 0.0 || arr[i] >= 1.0) return false;

    // Create n bucket lists; vector stores a variable number of values per list.
    vector<double> *buckets = new vector<double>[n];
    for (int i = 0; i < n; i++) {
        int index = static_cast<int>(n * arr[i]);
        if (index >= n) index = n - 1;
        buckets[index].push_back(arr[i]);
    }
    int next = 0;
    for (int i = 0; i < n; i++) {
        insertionSortBucket(buckets[i]);
        for (int j = 0; j < static_cast<int>(buckets[i].size()); j++)
            arr[next++] = buckets[i][j];
    }
    delete[] buckets;
    return true;
}

#ifndef SORTING_TEST
int main() {
    const int n = 25, trials = 1000;
    double input[n] = {0.170, 0.045, 0.075, 0.090, 0.802, 0.024, 0.002, 0.066, 0.000, 0.999,
        0.501, 0.018, 0.039, 0.120, 0.300, 0.005, 0.075, 0.042, 0.610, 0.007,
        0.088, 0.456, 0.321, 0.011, 0.200};
    double expected[n], arr[n];
    for (int i = 0; i < n; i++) expected[i] = input[i];
    sort(expected, expected + n); // Reference only; never substitutes for our sort.

    cout << "Input (" << n << " elements): ";
    for (int i = 0; i < n; i++) cout << input[i] << ' ';
    cout << '\n';
    // Warm up once before collecting measurements.
    for (int i = 0; i < n; i++) arr[i] = input[i];
    if (!bucketSort(arr, n) || !equal(arr, arr + n, expected)) {
        cerr << "Invalid input or incorrect sorted output" << endl;
        return 1;
    }
    cout << "Sorted: ";
    for (int i = 0; i < n; i++) cout << arr[i] << ' ';
    cout << '\n';

    filesystem::create_directories("results");
    ofstream samples("results/bucket_sort_timing_trials.csv");
    ofstream summary("results/bucket_sort_timing.csv");
    if (!samples || !summary) {
        cerr << "Cannot open result files" << endl;
        return 1;
    }
    samples << fixed << setprecision(6);
    summary << fixed << setprecision(6);
    samples << "algorithm,n,trial,milliseconds\n";
    double totalMilliseconds = 0;
    for (int trial = 1; trial <= trials; trial++) {
        for (int i = 0; i < n; i++) arr[i] = input[i];
        auto start = chrono::steady_clock::now();
        bool success = bucketSort(arr, n);
        auto end = chrono::steady_clock::now();
        double milliseconds = chrono::duration<double, milli>(end - start).count();
        // Copying, printing and verification are outside the timed section.
        if (!success || !equal(arr, arr + n, expected)) {
            cerr << "Incorrect sorted output" << endl;
            return 1;
        }
        totalMilliseconds += milliseconds;
        samples << "bucket_sort," << n << ',' << trial << ',' << milliseconds << '\n';
    }
    double averageMilliseconds = totalMilliseconds / trials;
    summary << "algorithm,n,trials,average_milliseconds\n"
            << "bucket_sort," << n << ',' << trials << ',' << averageMilliseconds << '\n';
    cout << fixed << setprecision(6)
         << "Average execution time over " << trials << " trials: "
         << averageMilliseconds << " ms" << endl;
    return 0;
}
#endif

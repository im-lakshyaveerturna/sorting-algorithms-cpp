#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <cmath>

using namespace std;

// Aim: Sort nonnegative integers using stable counting sort.
// Method: Count frequencies, form prefix sums, and place elements backwards.
bool countingSort(int arr[], int n) {
    if (n < 0) return false;
    if (n == 0) return true;
    int maximum = arr[0];
    for (int i = 0; i < n; i++) {
        // Keep the frequency table within a reasonable memory limit.
        if (arr[i] < 0 || arr[i] > 1000000) return false;
        if (arr[i] > maximum) maximum = arr[i];
    }
    int *count = new int[maximum + 1](); // Initialize every frequency to zero.
    int *output = new int[n];
    for (int i = 0; i < n; i++) count[arr[i]]++;
    for (int value = 1; value <= maximum; value++) count[value] += count[value - 1];
    for (int i = n - 1; i >= 0; i--) {
        output[count[arr[i]] - 1] = arr[i];
        count[arr[i]]--;
    }
    for (int i = 0; i < n; i++) arr[i] = output[i];
    delete[] count;
    delete[] output;
    return true;
}

#ifndef SORTING_TEST
int main() {
    const int n = 25, trials = 1000;
    int input[n] = {170, 45, 75, 90, 802, 24, 2, 66, 0, 999,
        501, 18, 39, 120, 300, 5, 75, 42, 610, 7, 88, 456, 321, 11, 200};
    int expected[n], arr[n];
    for (int i = 0; i < n; i++) expected[i] = input[i];
    sort(expected, expected + n); // Reference only; never substitutes for our sort.

    cout << "Input (" << n << " elements): ";
    for (int i = 0; i < n; i++) cout << input[i] << ' ';
    cout << '\n';
    // Warm up once before collecting measurements.
    for (int i = 0; i < n; i++) arr[i] = input[i];
    if (!countingSort(arr, n) || !equal(arr, arr + n, expected)) {
        cerr << "Invalid input or incorrect sorted output" << endl;
        return 1;
    }
    cout << "Sorted: ";
    for (int i = 0; i < n; i++) cout << arr[i] << ' ';
    cout << '\n';

    filesystem::create_directories("results");
    ofstream samples("results/counting_sort_timing_trials.csv");
    ofstream summary("results/counting_sort_timing.csv");
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
        bool success = countingSort(arr, n);
        auto end = chrono::steady_clock::now();
        double milliseconds = chrono::duration<double, milli>(end - start).count();
        // Copying, printing and verification are outside the timed section.
        if (!success || !equal(arr, arr + n, expected)) {
            cerr << "Incorrect sorted output" << endl;
            return 1;
        }
        totalMilliseconds += milliseconds;
        samples << "counting_sort," << n << ',' << trial << ',' << milliseconds << '\n';
    }
    double averageMilliseconds = totalMilliseconds / trials;
    summary << "algorithm,n,trials,average_milliseconds\n"
            << "counting_sort," << n << ',' << trials << ',' << averageMilliseconds << '\n';
    cout << fixed << setprecision(6)
         << "Average execution time over " << trials << " trials: "
         << averageMilliseconds << " ms" << endl;
    return 0;
}
#endif

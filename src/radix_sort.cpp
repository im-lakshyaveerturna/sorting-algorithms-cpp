#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <cmath>

using namespace std;

// Aim: Sort nonnegative integers using LSD radix sort.
// Method: Apply stable counting sort to units, tens, hundreds, ... digits.
void sortByDigit(int arr[], int n, long long exponent) {
    int count[10] = {0};
    int *output = new int[n];
    for (int i = 0; i < n; i++) count[(arr[i] / exponent) % 10]++;
    for (int digit = 1; digit < 10; digit++) count[digit] += count[digit - 1];
    // Traversing backwards preserves the order of equal digits (stability).
    for (int i = n - 1; i >= 0; i--) {
        int digit = static_cast<int>((arr[i] / exponent) % 10);
        output[count[digit] - 1] = arr[i];
        count[digit]--;
    }
    for (int i = 0; i < n; i++) arr[i] = output[i];
    delete[] output;
}

bool radixSort(int arr[], int n) {
    if (n < 0) return false;
    if (n == 0) return true;
    int maximum = arr[0];
    for (int i = 0; i < n; i++) {
        if (arr[i] < 0) return false;
        if (arr[i] > maximum) maximum = arr[i];
    }
    // long long prevents overflow when exponent passes INT_MAX's last digit.
    for (long long exponent = 1; maximum / exponent > 0; exponent *= 10)
        sortByDigit(arr, n, exponent);
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
    if (!radixSort(arr, n) || !equal(arr, arr + n, expected)) {
        cerr << "Invalid input or incorrect sorted output" << endl;
        return 1;
    }
    cout << "Sorted: ";
    for (int i = 0; i < n; i++) cout << arr[i] << ' ';
    cout << '\n';

    filesystem::create_directories("results");
    ofstream samples("results/radix_sort_timing_trials.csv");
    ofstream summary("results/radix_sort_timing.csv");
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
        bool success = radixSort(arr, n);
        auto end = chrono::steady_clock::now();
        double milliseconds = chrono::duration<double, milli>(end - start).count();
        // Copying, printing and verification are outside the timed section.
        if (!success || !equal(arr, arr + n, expected)) {
            cerr << "Incorrect sorted output" << endl;
            return 1;
        }
        totalMilliseconds += milliseconds;
        samples << "radix_sort," << n << ',' << trial << ',' << milliseconds << '\n';
    }
    double averageMilliseconds = totalMilliseconds / trials;
    summary << "algorithm,n,trials,average_milliseconds\n"
            << "radix_sort," << n << ',' << trials << ',' << averageMilliseconds << '\n';
    cout << fixed << setprecision(6)
         << "Average execution time over " << trials << " trials: "
         << averageMilliseconds << " ms" << endl;
    return 0;
}
#endif

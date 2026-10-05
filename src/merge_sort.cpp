#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <filesystem>

using namespace std;

// Aim: Implement recursive merge sort and count value comparisons.
// Divide into two halves, sort each half, and merge. Time O(n log n).

void mergeParts(int arr[], int left, int middle, int right, int &comparisons) {
    int temp[1000];
    int i = left, j = middle + 1, k = left;
    while (i <= middle && j <= right) {
        comparisons++; // One comparison between values in the two halves.
        if (arr[i] <= arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }
    while (i <= middle) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    for (int index = left; index <= right; index++) arr[index] = temp[index];
}

void mergeRange(int arr[], int left, int right, int &comparisons) {
    if (left >= right) return;
    int middle = left + (right - left) / 2;
    mergeRange(arr, left, middle, comparisons);
    mergeRange(arr, middle + 1, right, comparisons);
    mergeParts(arr, left, middle, right, comparisons);
}

void mergeSort(int arr[], int n, int &comparisons) {
    mergeRange(arr, 0, n - 1, comparisons);
}

#ifndef SORTING_TEST
int main() {
    // A fixed seed lets all four programs use the same random arrays.
    // Use srand(time(0)) with <ctime> if new arrays on every run are wanted.
    srand(20261005);
    filesystem::create_directories("results");
    ofstream summary("results/merge_sort_comparisons.csv");
    ofstream samples("results/merge_sort_comparison_trials.csv");
    ofstream output("results/merge_sort_output.txt");
    if (!summary || !samples || !output) {
        cerr << "Cannot open result files" << endl;
        return 1;
    }
    summary << fixed << setprecision(6);
    output << fixed << setprecision(2);
    cout << fixed << setprecision(2);
    summary << "n,trials,average_comparisons,n_log2_n,n_log10_n\n";
    samples << "n,trial,comparisons\n";

    for (int size = 30; size <= 1000; size += 10) {
        int totalComparisons = 0;
        for (int instance = 0; instance < 10; instance++) {
            // Standard C++ fixed-capacity arrays; only the first size values are used.
            int arr[1000], expected[1000];
            for (int i = 0; i < size; i++) {
                arr[i] = rand() % 1000;
                expected[i] = arr[i];
            }
            // std::sort is only a reference to check the lab algorithm's output.
            sort(expected, expected + size);
            int comparisons = 0;
            mergeSort(arr, size, comparisons);
            if (!equal(arr, arr + size, expected)) {
                cerr << "Incorrect sorted output" << endl;
                return 1;
            }
            totalComparisons += comparisons;
            samples << size << ',' << instance + 1 << ',' << comparisons << '\n';
        }
        double averageComparisons = static_cast<double>(totalComparisons) / 10.0;
        cout << size << " " << averageComparisons << endl;
        output << size << " " << averageComparisons << '\n';
        summary << size << ",10," << averageComparisons << ','
                << size * log2(size) << ',' << size * log10(size) << '\n';
    }
    return 0;
}
#endif

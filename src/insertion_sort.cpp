#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <filesystem>

using namespace std;

// Aim: Implement insertion sort and measure the supplied shift counter.
// Insert each key into its sorted prefix. Best O(n); average/worst O(n^2).

// Match the supplied lab example: count successful shifts only.
void insertionSort(int arr[], int n, int &comparisons) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            comparisons++;
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

#ifndef SORTING_TEST
int main() {
    // A fixed seed lets all four programs use the same random arrays.
    // Use srand(time(0)) with <ctime> if new arrays on every run are wanted.
    srand(20261005);
    filesystem::create_directories("results");
    ofstream summary("results/insertion_sort_comparisons.csv");
    ofstream samples("results/insertion_sort_comparison_trials.csv");
    ofstream output("results/insertion_sort_output.txt");
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
            insertionSort(arr, size, comparisons);
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

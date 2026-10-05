#include <iostream>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <filesystem>

using namespace std;

// Aim: Measure insertion sort for best, average and worst cases, n = 15000.
void insertionSortTimed(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// caseNumber: 0 = ascending, 1 = random permutation, 2 = descending.
void prepareInsertionInput(int arr[], int n, int caseNumber) {
    for (int i = 0; i < n; i++) {
        if (caseNumber == 2) arr[i] = n - i;
        else arr[i] = i + 1;
    }
    if (caseNumber == 1) {
        // Fisher-Yates shuffle gives distinct elements in a random order.
        for (int i = n - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
}

#ifndef SORTING_TEST
int main() {
    const int n = 15000, trials = 5;
    int arr[n]; // n is a compile-time constant, so this is standard C++.
    const char *caseNames[] = {"best", "average", "worst"};
    srand(20261005);
    filesystem::create_directories("results");
    ofstream samples("results/insertion_sort_timing_trials.csv");
    ofstream summary("results/insertion_sort_timing.csv");
    if (!samples || !summary) {
        cerr << "Cannot open result files" << endl;
        return 1;
    }
    samples << fixed << setprecision(6);
    summary << fixed << setprecision(6);
    cout << fixed << setprecision(6);
    samples << "case,n,trial,milliseconds\n";
    summary << "case,n,trials,average_milliseconds\n";
    cout << "Insertion sort: " << n << " elements, " << trials << " trials\n";
    for (int caseNumber = 0; caseNumber < 3; caseNumber++) {
        prepareInsertionInput(arr, n, caseNumber);
        insertionSortTimed(arr, n); // One warmup per case.
        double totalMilliseconds = 0;
        for (int trial = 1; trial <= trials; trial++) {
            prepareInsertionInput(arr, n, caseNumber);
            auto start = chrono::steady_clock::now();
            insertionSortTimed(arr, n);
            auto end = chrono::steady_clock::now();
            double milliseconds = chrono::duration<double, milli>(end - start).count();
            // The input is a permutation of 1..n: verify every sorted value.
            for (int i = 0; i < n; i++) {
                if (arr[i] != i + 1) {
                    cerr << "Incorrect sorted output" << endl;
                    return 1;
                }
            }
            totalMilliseconds += milliseconds;
            samples << caseNames[caseNumber] << ',' << n << ',' << trial
                    << ',' << milliseconds << '\n';
        }
        double averageMilliseconds = totalMilliseconds / trials;
        summary << caseNames[caseNumber] << ',' << n << ',' << trials
                << ',' << averageMilliseconds << '\n';
        cout << caseNames[caseNumber] << " case: " << averageMilliseconds << " ms\n";
    }
    return 0;
}
#endif

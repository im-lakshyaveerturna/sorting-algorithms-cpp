#include <iostream>
#include <ctime>
#include <vector>
#include <stdexcept>
using namespace std;

void countsort(int arr[], int n) {
    if (n < 0) throw invalid_argument("Array size cannot be negative");
    if (n == 0) return;
    for (int i = 0; i < n; ++i)
        if (arr[i] < 0 || arr[i] > 1000000)
            throw invalid_argument("Counting sort requires values in [0, 1000000]");

    int maxval = arr[0];

    for (int i = 1; i < n; i++) {
        if (arr[i] > maxval)
            maxval = arr[i];
    }

    vector<int> count(maxval + 1, 0);

    for (int i = 0; i < n; i++)
        count[arr[i]]++;

    for (int i = 1; i <= maxval; i++)
        count[i] += count[i - 1];

    vector<int> ans(n);

    for (int i = n - 1; i >= 0; i--) {
        ans[count[arr[i]] - 1] = arr[i];
        count[arr[i]]--;
    }

    for (int i = 0; i < n; i++)
        arr[i] = ans[i];
}

int main() {

    int arr[20] = {
        12, 5, 18, 3, 9,
        15, 1, 20, 7, 11,
        4, 16, 8, 2, 19,
        6, 14, 10, 13, 17
    };

    int n = 20;

    cout << "Original Array:" << endl;

    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";

    clock_t start = clock();

    countsort(arr, n);

    clock_t end = clock();

    cout << "\n\nSorted Array:" << endl;

    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";

    double executionTime =
        ((double)(end - start) / CLOCKS_PER_SEC) * 1000;

    cout << "\n\nExecution Time: "
         << executionTime
         << " milliseconds" << endl;

    return 0;
}
// Original Array:
// 12 5 18 3 9 15 1 20 7 11 4 16 8 2 19 6 14 10 13 17

// Sorted Array:
// 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20

// Execution Time: 0 milliseconds
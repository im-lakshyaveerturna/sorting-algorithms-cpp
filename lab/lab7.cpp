#include <iostream>
#include <ctime>
#include <vector>
#include <cmath>
#include <stdexcept>

using namespace std;

void insertion_sort(float bucket[], int size)
{
    for (int i = 1; i < size; i++)
    {
        float key = bucket[i];
        int j = i - 1;

        while (j >= 0 && bucket[j] > key)
        {
            bucket[j + 1] = bucket[j];
            j--;
        }

        bucket[j + 1] = key;
    }
}

// Bucket Sort
void bucket_sort(float arr[], int n)
{
    if (n < 0) throw invalid_argument("Array size cannot be negative");
    if (n == 0) return;
    for (int i = 0; i < n; ++i)
        if (!isfinite(arr[i]) || arr[i] < 0 || arr[i] >= 1)
            throw invalid_argument("Bucket sort requires finite values in [0, 1)");
    // Size buckets from n instead of assuming exactly 20 elements.
    vector<vector<float>> buckets(n);
    vector<int> bucketSize(n, 0);

    for (int i = 0; i < n; i++)
    {
        int bi = int(static_cast<double>(n) * arr[i]);

        buckets[bi].push_back(arr[i]);
        bucketSize[bi]++;
    }

    for (int i = 0; i < n; i++)
    {
        insertion_sort(buckets[i].data(), bucketSize[i]);
    }
    int index = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < bucketSize[i]; j++)
        {
            arr[index] = buckets[i][j];
            index++;
        }
    }
}

int main()
{
    float arr[20] = {
        0.78, 0.17, 0.39, 0.26, 0.72,
        0.94, 0.21, 0.12, 0.23, 0.68,
        0.55, 0.31, 0.89, 0.45, 0.63,
        0.08, 0.97, 0.36, 0.51, 0.15
    };

    int n = 20;

    cout << "Original Array:" << endl;

    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";

    clock_t start = clock();

    bucket_sort(arr, n);

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
// 0.78 0.17 0.39 0.26 0.72 0.94 0.21 0.12 0.23 0.68 0.55 0.31 0.89 0.45 0.63 0.08 0.97 0.36 0.51 0.15 

// Sorted Array:
// 0.08 0.12 0.15 0.17 0.21 0.23 0.26 0.31 0.36 0.39 0.45 0.51 0.55 0.63 0.68 0.72 0.78 0.89 0.94 0.97 

// Execution Time: 0 milliseconds
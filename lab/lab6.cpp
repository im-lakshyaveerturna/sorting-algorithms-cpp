#include <iostream>
#include <ctime>
#include <vector>
#include <stdexcept>
using namespace std;

int getMax(int arr[], int n)
{
    if (n <= 0) throw invalid_argument("getMax requires a nonempty array");
    int mx = arr[0];

    for (int i = 1; i < n; i++)
        if (arr[i] > mx)
            mx = arr[i];

    return mx;
}

void countSort(int arr[], int n, long long exp)
{
    vector<int> output(n);
    int i, count[10] = {0};

    for (i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;

    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (i = n - 1; i >= 0; i--)
    {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    for (i = 0; i < n; i++)
        arr[i] = output[i];
}

void radixsort(int arr[], int n)
{
    if (n < 0) throw invalid_argument("Array size cannot be negative");
    if (n == 0) return;
    for (int i = 0; i < n; ++i)
        if (arr[i] < 0) throw invalid_argument("Radix sort requires nonnegative integers");
    int m = getMax(arr, n);

    for (long long exp = 1; m / exp > 0; exp *= 10)
        countSort(arr, n, exp);
}

void print(int arr[], int n)
{
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";
}

int main()
{
    int arr[20] = {
        170, 45, 75, 90, 802,
        24, 2, 66, 123, 56,
        999, 321, 87, 43, 210,
        654, 12, 345, 78, 501
    };

    int n = 20;

    cout << "Original Array:" << endl;
    print(arr, n);

    clock_t start = clock();

    radixsort(arr, n);

    clock_t end = clock();

    cout << "\n\nSorted Array:" << endl;
    print(arr, n);

    double executionTime =
        ((double)(end - start) / CLOCKS_PER_SEC) * 1000;

    cout << "\n\nExecution Time: "
         << executionTime
         << " milliseconds" << endl;

    return 0;
}
// Original Array:
// 170 45 75 90 802 24 2 66 123 56 999 321 87 43 210 654 12 345 78 501 

// Sorted Array:
// 2 12 24 43 45 56 66 75 78 87 90 123 170 210 321 345 501 654 802 999 

// Execution Time: 0 milliseconds
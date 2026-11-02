#include <iostream>
#include <cstdlib>   
#include <ctime>    
#include <vector>    

using namespace std;

void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

void heapify(vector<int> &arr, int n, int i, int &comparisons) {
    int largest = i;     
    int left = 2 * i + 1;  
    int right = 2 * i + 2; 
    if (left < n) {
        comparisons++; 
        if (arr[left] > arr[largest]) {
            largest = left;
        }
    }
    if (right < n) {
        comparisons++; 
        if (arr[right] > arr[largest]) {
            largest = right;
        }
    }
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest, comparisons);
    }
}

void heapSort(vector<int> &arr, int n, int &comparisons) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i, comparisons);
    }
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0, comparisons);
    }
}

int main() {
    srand(time(0)); 

    for (int size = 30; size <= 1000; size += 10) {
        int totalComparisons = 0;
        for (int instance = 0; instance < 10; instance++) {
            vector<int> arr(size); 
            for (int i = 0; i < size; i++) {
                arr[i] = (static_cast<long long>(rand()) + i + instance) % 1000;
            }
            int comparisons = 0;
            heapSort(arr, size, comparisons);
            totalComparisons += comparisons;
        }
        double averageComparisons = totalComparisons / 10.0;
        // cout << size << " " << averageComparisons << endl;
        cout << averageComparisons << "\n" ;
    }

    return 0;
}
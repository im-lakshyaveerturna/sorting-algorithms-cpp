#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

int partition(vector<int>& arr, int low, int high , int& comparisons) {
    int pivot = arr[high];

    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        comparisons++;
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);  
    return i + 1;
}

void quickSort(vector<int>& arr, int low, int high, int& comparisons) {
  
    if (low < high) {
        int pi = partition(arr, low, high , comparisons);
        quickSort(arr, low, pi - 1, comparisons);
        quickSort(arr, pi + 1, high, comparisons);
    }
}
int main () {
    srand(time(0)) ;
    for (int length = 30 ; length <= 1000 ; length += 10) {
        int totalComparisons = 0 ;

        for (int instance = 0 ; instance < 10 ; instance++) {
            vector<int> array(length) ;

            for (int index = 0 ; index < length ; index++) {
                array[index] = (static_cast<long long>(rand()) + index + instance) % 1000 ;
            }
            int instanceComparisons = 0;
            quickSort(array, 0, array.size() - 1 , instanceComparisons) ;
            totalComparisons += instanceComparisons ;
        }
        // cout << "Length: " << length << ", Average Comparisons: " << (double)totalComparisons / 10 << "\n" ;
        cout << (double)totalComparisons/10 << "\n" ;
    }
    return 0 ;
}
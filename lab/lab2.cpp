#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

void merge(vector<int>& arr, int left,int mid, int right, int& comparisons){
                         
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0;
    int k = left;

    while (i < n1 && j < n2) {
        comparisons++ ;
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}
int mergeSort(vector<int>& arr, int left, int right , int& comparisons) {
    
    if (left >= right)
        return 0;

    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid, comparisons);
    mergeSort(arr, mid + 1, right, comparisons);
    merge(arr, left, mid, right , comparisons);
    return 0;
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
            mergeSort(array, 0, array.size() - 1 , instanceComparisons) ;
            totalComparisons += instanceComparisons ;
        }
        cout << "Length: " << length << ", Average Comparisons: " << (double)totalComparisons / 10 << "\n" ;
    }
    return 0 ;
}
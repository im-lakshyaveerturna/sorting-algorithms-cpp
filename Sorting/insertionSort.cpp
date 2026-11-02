#include <iostream>
#include <vector>
using namespace std ;

int insertionSort (vector<int> & list) { // void insertionSort (vector<int> & list) {
    int comparisons = 0 ;
    int lengthOfArray = list.size() ;
    for (int i = 1 ; i < lengthOfArray ; ++i) {
        int key = list[i] ;
        int j = i-1 ;
        while (j >= 0 && list [j] > key) {
            comparisons += 1 ;
            list[j+1] = list[j] ;
            j-- ;
        }
        comparisons += 1 ;
        list[j+1] = key ;
    }
    // cout << "Number of comparisons are : " << comparisons << "\n" ;
    return comparisons ;
}
int main () {
    vector<int> array = {12,11,13,5,6} ;
    int result =insertionSort(array) ;
    cout << "Sorted array is : " ;
    for (int i = 0 ; i < static_cast<int>(array.size()) ; ++i) {
        cout << array[i] << " " ;
    }
    cout << "\n" ;
    cout << "Number of comparisons are : " << result << "\n" ;
    return 0 ;
}
// Number of comparisons are : 11
// Sorted array is : 5 6 11 12 13 
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std ;

int insertionSort (vector<int> & list) { 
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
    return comparisons ;
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
            totalComparisons += insertionSort(array) ;
        }
        cout << "Length: " << length << ", Average Comparisons: " << (double)totalComparisons / 10 << "\n" ;
    }
    return 0 ;
}
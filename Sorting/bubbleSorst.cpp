#include <iostream>
#include <vector>
using namespace std ;
void bubbleSort (vector<int> & array) {
    int lengthOfArray = array.size() ;
    bool swappedOrNot ;
    for (int i = 0 ; i < lengthOfArray - 1 ; ++i) {
        swappedOrNot = false ;
        for (int j = 0 ; j < lengthOfArray - i - 1 ; ++ j) {
            if (array[j] > array[j+1]) {
                swap(array[j+1] , array[j]) ;
                swappedOrNot = true ;
            }
        }
        if (! swappedOrNot) break ;
    } 
}
int main () {
    vector<int> array = {5,6,1,3} ;
    bubbleSort(array) ;
    cout << "Sorted array is : " ; // Sorted array is : 1 3 5 6 
    for (const int & value : array) {
        cout << value << " " ;
    }
    return 0 ;
}
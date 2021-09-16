#include <iostream>
#include <random>
#include <string>
#include <utility>

using std::cout;
using std::default_random_engine;
using std::endl;
using std::string;
using std::swap;
using std::uniform_int_distribution;

default_random_engine DRE;

// Given two random access iterators, the partition function
//   - picks a random element
//   - partitions the range so that everything to the left of the pivot is less
//     than the pivot and everything to the right is greater than or equal to
//     the pivot
//   - returns an iterator to the pivot
template <typename RandomAccessIt>
RandomAccessIt partition(RandomAccessIt begin, RandomAccessIt end) {
    // Base case: end - begin <= 1
    if (end - begin <= 1) {
        return begin;
    }
    // Randomly pick a pivot
    uniform_int_distribution<int> uniform_dist(0, end - begin - 1);
    int pivot_index = uniform_dist(DRE);
    // Swap the pivot so it's at end - 1.
    RandomAccessIt pivot = end - 1;
    swap(*(begin + pivot_index), *pivot);
    // Partition using the pivot
    RandomAccessIt left = begin, right = pivot - 1;
    while (left <= right) {
        if (*left < *pivot) {
            ++left;
        }
        else {
            swap(*left, *right);
            --right;
        }
    }
    // Put the pivot in the middle and return an iterator to where the pivot is
    swap(*left, *pivot);
    return left;
}

template <typename RandomAccessIt>
RandomAccessIt foo(RandomAccessIt begin, RandomAccessIt end, int i) {
    // if end == begin stop
    if (end - begin <= 0) {
        return end;
    }
    RandomAccessIt pivot = partition(begin, end);
    int pivot_index = pivot - begin; //where pivot is in the array
    if (pivot_index < i) { //pivot is to the left of the current element i
        //recurse, pick a new partition
        return foo(pivot + 1, end, i - pivot_index - 1);
        //return foo(pivot, end, i - pivot_index);
    }
    else if (pivot_index > i) { //pivot is to the right of the current element i
        return foo(begin, pivot, i);
    }
    else { // if (pivot_index == i) //pivot is the current element i
        return pivot;
    }
}

int main() {
    string letters = "qwertyuiopasdfghjklzxcvbnm";
    cout << letters << endl;
    // loop through all of the letters by char
    for (int i = 0; i < letters.size(); ++i) {
        //dereference return from function foo, called on the whole range of letters, removing the first letter each time this code is called 
        cout << i << ' ' << *foo(letters.begin(), letters.end(), i) << endl;
    }
    return 0;
}
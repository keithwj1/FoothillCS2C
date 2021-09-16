#pragma once
// C++ implementation of Radix Sort
#include <iostream>
#include <vector>
using byte = signed char;
static const int bucket_size = int(UCHAR_MAX) + 1;
static const int bucket_size_offset = bucket_size / 2;
template <typename T>
void print_bytes(T& object) {
    T* ptr = &object;
    byte* start = reinterpret_cast<byte*>(ptr);
    byte* end = start + sizeof(T) / sizeof(byte);
    int size = end - start;
    std::cout << "Number of Bytes = " << size << std::endl;
    for (int i = size; i >= 0; --i) {
        
    }
}

// A utility function to get maximum value in arr[]
template <typename T>
size_t getMax(std::vector<T> &input, int n){
    size_t max = 0;
    for (auto& elem : input) {
        //TODO: handle pointers to data types
        size_t cur = sizeof(elem);
        if (cur > max) {
            max = cur;
        }
    }
    return max;
}




// A utility function to print an array
void print(int arr[], int n)
{
    for (int i = 0; i < n; i++)
        std::cout << arr[i] << " ";
}

// Driver Code
template <typename T>
int geek_sort(std::vector<T> &input){
    radixsort(input, input.size());
    return 0;

    int arr[] = { 170, 45, 75, 90, 802, 24, 2, 66 };
    int n = sizeof(arr) / sizeof(arr[0]);
    // n is just # of elements. call std::vector.size()
    // Function Call
    radixsort(arr, n);
    print(arr, n);
    return 0;
}

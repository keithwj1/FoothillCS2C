// quiz8.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>



template<typename T>
void insert_sort(std::vector<T> &input)
{
    T key;
    for (size_t i = 1; i < input.size(); ++i) {
        key = input[i];
        int j = i - 1;
        for (j; j >= 0 && input[j] > key;--j) {
            input[j + 1] = input[j];
        }
        input[j + 1] = key;
    }
}

template<typename T>
void swap(T* x, T* y){
    T temp = *x;
    *x = *y;
    *y = temp;
}

template<typename T>
void bubble_sort(std::vector<T> &input)
{
    int size = input.size();
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (input[j] > input[j + 1]) {
                swap(&input[j], &input[j + 1]);
            }
        }
    }
}

template<typename T>
void question3(std::vector<T> & input) {
    //std::sort(input.begin(), input.end());
    bubble_sort(input);
}

template<typename T>
void question4(std::vector<T>& input) {
    //std::sort(input.begin(), input.end());
    insert_sort(input);
}


int main()
{
    std::vector<std::string> input = { "b","a","f","l","k","n","z","x","y" };
    question3(input);
    //question4(input);
    for (auto& elem : input) {
        std::cout << elem << ", ";
    }
    std::cout << std::endl;
}
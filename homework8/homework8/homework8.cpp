// homework8.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "nonstd_sort.h"
#include "radix_sort.h"

template <typename T>
void print(std::vector<T>& input) {
    for (auto& elem : input) {
        std::cout << elem << ", ";
    }
    std::cout << std::endl;
}

int main()
{
    std::cout << "Hello World!\n";
    std::vector<std::string> input = { "b","a","f","l","k","n","z","x","y","as","dsas","ehasdjfhxzcnvbasj" };
    nonstd::cust_sort(input);
    print(input);
    std::vector<int> int_input = {1,6,2,1,7,3,5,8,32,2,NULL,34,23423,5567,3234,1327,56,234,1233,546,456234,23,123, -123,-4,-122};
    nonstd::cust_sort(int_input);
    print(int_input);
    std::cout << std::endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

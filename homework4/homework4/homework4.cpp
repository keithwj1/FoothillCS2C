// homework4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdlib.h>
#include "multiset.h"
int main()
{
    nonstd::MultiSet<int> myset;
    int max = 101;
    for (int j = 0; j < 100; ++j) {
        for (int i = 0; i < max; ++i) {
            myset.insert(i);
        }
    }
    for (int j = 0; j < 10000; ++j) {
        myset.insert(rand() % max);
    }
    std::cout << "List Generation Complete" << std::endl;
    myset.print();
    for (int i = 0; i < max; ++i) {
        std::cout << i << " has " << myset.count(i) << " occurances." << std::endl;
    }
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

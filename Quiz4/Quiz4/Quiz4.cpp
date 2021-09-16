// Quiz4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "quiz4_include.h"
#include <set>
#include <string>
#include <fstream>

int main()
{
    std::map<int, long> values;
    for (int i = 0; i < 5; i++) {
        auto val = num_hilbert_curve_corners(i, values);
        std::cout << "value=" << val << std::endl;
   }
    std::set<std::string> question4 = { "First","sEcond","lasT","ALL","none","SoMe","sjkdfhsksdjkf","the" };
    remove_upper_case(question4);
    for (auto it : question4) {
        std::cout << "All Lower = " << it << std::endl;
    }
    std::ifstream f("C:/Users/kjones/source/school/Foothill-CS2C/Quiz4/Quiz4/romeo-and-juliet_TXT_FolgerShakespeare.txt");
    //C:\Users\kjones\source\school\Foothill-CS2C\Quiz4\Quiz4
    std::map<char,int> cur_map = counting_chars(&f);
    for (auto cur : cur_map) {
        std::cout << cur.first << "=" << cur.second << std::endl;
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

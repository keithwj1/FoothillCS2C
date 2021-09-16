// quiz7.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>


#include <queue>
#include <vector>
#include <assert.h> 
// Given a vector of elements in any order, return the 5th smallest element
// For example:
//   std::vector<char> chars = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'};
//   assert(fifth_smallest_element(chars) == 'e');
template <typename T>
const T& fifth_smallest_element(const std::vector<T>& elements) {
    // Fill in this part of the function, using an std::priority_queue
    std::priority_queue<T> cur;
    for (auto& elem : elements) {
        cur.push(elem);
    }
    while (!cur.empty()) {
        std::cout << cur.top() << std::endl;
        if (cur.size() == 5) {
            //Have to return the actual element in the vector because of & operator.
            auto it = std::find(elements.begin(), elements.end(), cur.top());
            if (it != elements.end()) {
                return *it;
            }
        }
        cur.pop();
    }
    return *elements.end();
}

void question5() {
    std::vector<char> chars = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i' };
    auto fifth = fifth_smallest_element(chars);
    assert(fifth == 'e');
}

int main()
{
    //std::cout << "Hello World!\n";
    question5();
    return 0;
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

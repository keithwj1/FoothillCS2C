// quiz5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "quiz5.h"

int question2() {
    AVLTree<int, int> avl;
    int max = 50;
    for (int i = 0; i < max; ++i) {
        //avl[i] = i;
        avl.insert_no_balance(i, avl.root_);
    }
    avl.print(avl.root());
    std::cout << std::endl;
    std::cout << "Is avl valid tree=" << is_valid_avltree(avl.root()) << std::endl;
    std::cout << "avl tree height=" << calculate_height(avl.root()) << std::endl;
    AVLTree<int, int> aavl;
    for (int i = 0; i < max; ++i) {
        aavl[i] = i;
    }
    std::cout << "Is aavl valid tree=" << is_valid_avltree(aavl.root()) << std::endl;
    std::cout << "aavl tree height=" << calculate_height(aavl.root()) << std::endl;
    std::cout << "faster aavl tree height=" << calculate_height_fast(aavl.root()) << std::endl;
    return 0;
}
int question1() {
    AVLTree<int, int> avl;
    int max = 987654321;
    for (int i = 0; i < max; ++i) {
        avl[i] = i;
    }
    std::cout << "avl tree height=" << calculate_height(avl.root()) << std::endl;
    std::cout << "avl root balance factor=" << avl.root()->balance_factor() << std::endl;
    return 0;
}

int main()
{
    std::cout << "Hello World!\n";
    question1();
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

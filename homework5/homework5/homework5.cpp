// homework5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdlib.h>
#include "avl_tree_unique.h"

int main()
{
    
    int max = 50;
    /*AVLTree <int, int> MyTree;
    for (int j = 0; j < 5; ++j) {
        for (int i = 0; i < max; ++i) {
            MyTree.insert(i, i);
        }
    }
    MyTree.print();
    for (int i = 0; i < max; ++i) {
        std::cout << i << " has " << MyTree.count(i) << " occurances." << std::endl;
    }
    std::cout << "Tree Height is " << MyTree.get_height(MyTree.root()) << std::endl;
    MyTree.resort();
    MyTree.print();
    std::cout << "Tree Height is " << MyTree.get_height(MyTree.root()) << std::endl;*/

    AVLTree <int, int> OperatorTree;
        for (int i = 0; i < max; ++i) {
            OperatorTree[i] = i;
        }
    OperatorTree.print(OperatorTree.root());
    std::cout << std::endl;
    std::cout << "Tree Height is " << OperatorTree.height(OperatorTree.root()) << std::endl;
    //avl.full_balance();

    AVLTree<int, int> avl;
    avl[14] = 20;
    avl[24] = 1;
    avl[12] = 23;
    avl[13] = 9;
    std::cout << "Balance Factor of node 14 should be -1, but is " << avl.get_node(14)->balance_factor() << std::endl;

    AVLTree<int, int> new_avl;
    new_avl[14] = 20;
    new_avl[24] = 1;
    new_avl[12] = 23;
    std::cout << "Balance Factor of node 14 should be 0, but is " << new_avl.get_node(14)->balance_factor() << std::endl;

    AVLTree<int, int> bavl;
    bavl[14] = 20;
    bavl[24] = 1;
    bavl[12] = 23;
    bavl[13] = 9;
    bavl[12] = 23;
    bavl[23] = 8;
    bavl[21] = 18;
    std::cout << "Balance Factor of node 14 should be 1, but is " << bavl.get_node(14)->balance_factor() << std::endl;
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

#include <iostream>

#include "log_n_vector.h"

using std::cout;
using std::endl;

int main() {
    //LogNVector<int> v = { 0, 11, 22, 33, 44, 55, 66, 77, 88, 99, 110, 121 };
    LogNVector<int> v = { 00, 10, 11, 20, 21, 22, 23, 30, 31, 32, 33, 34 };
    cout << "v[0] == " << v[0] << ",    &v[0] == " << &v[0] << endl;
    for (int j = 1; j < v.size(); ++j) {
        // All the elements in the same array should have sequential addresses.
        // For example, &v[2] == &v[1] + 1
        // However, there's no guarantee that &v[1] == &v[0] + 1.
        cout << "v[" << j << "] == " << v[j] << ",    "
            << "&v[" << j << "] - &v[" << j - 1 << "] == "
            << &v[j] - &v[j - 1] << endl;
    }
    return 0;
}
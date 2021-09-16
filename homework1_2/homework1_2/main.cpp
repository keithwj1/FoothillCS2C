#include <cmath>
#include <iostream>

#include "binary_search_function.h"

using namespace std;

double square(double x) {
    return x * x;
}

int main() {
    for (double x = 0; x < 10; x += 1.0) {
        // Approximate the square root of x by binary searching the square function.
        // For any positive number, we know the square root of x is between 0 and x.
        double approx_sqrt_x = binary_search_function(square, x, 0.0001, 0.0, x);
        cout << "sqrt(" << x << ") ≈ " << approx_sqrt_x << endl;
    }
    return 0;
}
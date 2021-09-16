#include "binary_search_function.h"

double binary_search_function(
    const std::function<double(double)>& f,  // function to binary search
    double value,  // value we are searching for
    double max_err,  // maximum allowed error of value being passed to f
    double lower_bound,
    double upper_bound) {

    // your code here...
    auto diff = std::abs(upper_bound - lower_bound);
    auto test_value = lower_bound + (diff / 2.0);
    if (diff <= max_err) {
        return test_value;
    }
    if (f(test_value) > value) {
        return binary_search_function(f, value, max_err, lower_bound, test_value);
    }
    else {
        return binary_search_function(f, value, max_err, test_value, upper_bound);
    }
}

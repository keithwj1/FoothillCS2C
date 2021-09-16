#pragma once
#include <cfloat>  // DBL_MAX
#include <functional>  // std::function

double binary_search_function(
    const std::function<double(double)>& f,  // function to binary search
    double value,  // value we are searching for
    // max_err is how close we want our answer to be. For example,
    //   if we passed max_err=0.0, then binary_search_function returns exactly
    //   the right answer*
    double max_err = 0.001,  // maximum allowed error of value being passed to f
    double lower_bound = -DBL_MAX,
    double upper_bound = DBL_MAX);
#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>
long num_hilbert_curve_corners(int n, std::map<int, long> &values) {
    auto iter = values.find(n);
    if (iter == values.end())
    {
        if (n == 0) {
            return values[n] = 0;
        }
        return values[n] =  4 * num_hilbert_curve_corners(n - 1,values) + (n % 2 == 0 ? 4 : 2);
    }
    else {
        return iter->second;
    }
}

void remove_upper_case(std::set<std::string> &set) {
    std::vector<std::string> to_delete;
    for (auto it : set){
        for (auto c : it) {
            if (isupper(c)) {
                to_delete.push_back(it);
                break;
            }
        }
    }
    for (auto cur : to_delete) {
        set.erase(cur);
    }
}

std::map<char, int> counting_chars(std::ifstream *file) {
    std::map<char, int> return_map;
    std::string word;
    while (*file >> word)
    {
        for (auto c : word) {
            return_map[c]++;
        }
    }
    return return_map;
}
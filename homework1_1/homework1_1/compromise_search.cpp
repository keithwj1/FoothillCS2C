#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "compromise_search.h"

using namespace std;

double square(double x) {
    return x * x;
}

int main() {
    vector<string> words = { "apple", "banana", "mango", "tangerine", "zuchini" };
    vector<string> words_to_search(words);
    words_to_search.push_back("aaa");
    words_to_search.push_back("bbb");
    words_to_search.push_back("yyy");
    words_to_search.push_back("zzz");
    for (const string& w : words_to_search) {
        for (size_t small_size = 0; small_size <= words.size(); ++small_size) {
            auto word_iterator = compromise_search(words.begin(), words.end(), w, small_size);
            cout
                << w << " was"
                << (word_iterator == words.end() ? " not" : "")
                << " found" << endl;
        }
    }
    cout << endl;
    return 0;
}
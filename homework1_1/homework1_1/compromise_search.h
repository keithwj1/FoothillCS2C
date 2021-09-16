#pragma once
// Compromise Search is a compromise between Binary Search and Linear Search:
//   We use Binary Search until we get to small_size elements, then we switch to Linear Search.
//   Binary Search between low and high until the gap between low and high is <= small_size,
//   then use std::find on the range (from low to high).
// For example,
// vector<string> words = {"aaa", "bbb", "ccc", "ddd", "eee"};
// cout << *compromise_search(words.begin(), words.end(), "bbb", 3) << endl;
// will print "bbb\n".
// compromise_search will
// - calculate the middle
// - see that "bbb" < "ccc" (and update the upper bound)
// - see that the remaining range (from "aaa" to "ccc" exclusive)
//   has <= small_size (3) elements
// - use std::find to search through the remaining range
//   (from "aaa" to "ccc" exclusive)
//
// RandomAccessIt is a random-access iterator, like vector<string>::begin().
// low is inclusive and high is exclusive.


template<class RandomAccessIt, class T>
RandomAccessIt my_binary_search(RandomAccessIt& low, RandomAccessIt& high, const T& value, const int& small_size) {
    int diff = high - low;
    RandomAccessIt i = low + diff/2;
    if (*i == value) {
        return i;
    }
    if (diff <= small_size) {
        throw 200;
    }
    if (*i > value) {
        high = i;
        return my_binary_search(low, high, value, small_size);
    }
    else if (diff == 1) {
        throw 404;
    }
    else {
        low = i;
        return my_binary_search(low, high, value, small_size);
    }
}

template<class RandomAccessIt, class T>
RandomAccessIt my_search(RandomAccessIt &low, RandomAccessIt &high, const T &value) {
    for (RandomAccessIt i = low; i != high; ++i) {
        if (*i == value) {
            return i;
        }
    }
    return high;
}

template<class RandomAccessIt, class T>
RandomAccessIt compromise_search(RandomAccessIt low, RandomAccessIt high, const T& value, const int& small_size) {
    // your code here...
    // store original high value because test template uses that as a failure
    const RandomAccessIt orig_high = high;
    const int diff = high - low;
    if (diff <= small_size) {
        //high == orig_high in this case, just return high
        return my_search(low, high, value);
    }
    else{
        try {
            return my_binary_search(low, high, value, small_size);
        }
        catch (int e) {
            switch (e) {
                case 200:{
                    //binary search reached small_size
                    //used an error so that orig_high doesn't have to be passed
                    auto val = my_search(low, high, value);
                    if (val == high) { return orig_high; }
                    return val;
                    break;
                }
                case 404 : {
                    //whole list was searched but nothing was found
                    return orig_high;
                    break;
                }
            }
        }
    }

    return orig_high;
}


#pragma once
#ifndef _log_n_vector_h_
#define _log_n_vector_h_

#include <cmath>
#include <memory>
#include <vector>

template <typename T>
class LogNVector {
    // These member variables are suggested and not required!
    // Feel free to use change the variable names or types, as long as you
    // follow the spirit of the assignment.
    std::vector<std::unique_ptr<T[]> > arrays_;
    int size_, capacity_;
public:
    LogNVector() : size_(0), capacity_(0) {
    }
    LogNVector(const LogNVector& other) : LogNVector() {
        capacity_ = 0;
        size_ = 0;
        for (int i = 0; i < other.size(); ++i)
        {
            this->push_back(other[i]);
        }
    }
    LogNVector(std::initializer_list<T> ilist) : LogNVector() {
        for (auto value : ilist) {
            this->push_back(value);
        }
    }
    ~LogNVector() {
        // TODO
    }

    int size() const noexcept {
        return size_;
    }
    int capacity() const noexcept {
        return capacity_;
    }
    void push_back(const T& value) {
        size_++;
        if (capacity_ < size_) {
            arrays_.push_back(std::make_unique<T[]>(capacity_+1));
            capacity_ = capacity_ + capacity_ + 1;
        }
        int put = size_ - 1 - (capacity_) / 2;
        arrays_.back()[put] = value;
    }
    const T& operator[](int index) const {
        int array_location = log2(index + 1);
        int elem_location = index + 1 - pow(2, array_location);
        return arrays_[array_location][elem_location];
    }
    T& operator[](int index) {
        int array_location = log2(index + 1);
        int elem_location = index + 1 - pow(2, array_location);
        return arrays_[array_location][elem_location];
    }
};

#endif // _log_n_vector_h_
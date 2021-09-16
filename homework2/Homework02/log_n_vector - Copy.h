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
    //std::vector<T*[]> arrays_;
    std::vector < std::unique_ptr<T[]>> arrays_;
    int size_; //current # of values
    int capacity_; //current max # of values
public:
    LogNVector() : size_(0),capacity_(0) {
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
        for (int i = 0; i < size_; i++) {
            
        }
    }

    int size() const noexcept {
        return size_;
    }
    int capacity() const noexcept {
        return capacity_;
    }
    void push_back(const T& value) {
        size_++;
        if (capacity_ < size_){
            //add a new v
            auto new_array = std::make_unique<T[]>(capacity_);
            //std::cout << "Creating new array of size " << capacity_ << std::endl;
            capacity_ = capacity_+ capacity_ + 1;
            arrays_.push_back(std::move(new_array));
        }
        //now add value to last in list
        //calculate where to put it in the new array
        //int array_num = (int)log2(size_);
        int put = size_ - 1 - (capacity_)/2;
        //std::cout << "Adding Value " << value << " To array " << array_num << " in position " << put << std::endl;
        arrays_.back()[put] = value;        
    }
    const T& operator[](int index) const {
        // TODO
        int array_location = (int)log2(index + 1);
        int elem_location = index + 1 - (int)pow(2, array_location);
        return arrays_[array_location][elem_location];
    }
    T& operator[](int index) {
        // TODO
        int array_location = (int)log2(index+1);
        int elem_location = index +1 - (int)pow(2, array_location);
        return arrays_[array_location][elem_location];
    }
};

#endif // _log_n_vector_h_
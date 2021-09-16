#pragma once

#include <algorithm>
#include <vector>

namespace nonstd {

    template<typename T>
    void swap(T* x, T* y) {
        T temp = *x;
        *x = *y;
        *y = temp;
    }

    template<typename T>
    void bubble_sort(std::vector<T>& input){
        int size = input.size();
        for (int i = 0; i < size - 1; ++i) {
            for (int j = 0; j < size - i - 1; ++j) {
                if (input[j] > input[j + 1]) {
                    swap(&input[j], &input[j + 1]);
                }
            }
        }
    }

    template <class T> void swap(T& a, T& b){
        T c(std::move(a)); a = std::move(b); b = std::move(c);
    }

    template<class RandomIt>
    void bubble_sort_interior(RandomIt begin, RandomIt end) {
        if ((begin + 1) == end) { return; }
        if (*begin > * (begin + 1)) {
            auto temp = *begin;
            *begin = *(begin + 1);
            *(begin + 1) = temp;
        }
        return bubble_sort_interior(++begin, end);
    }
    template<class RandomIt>
    void bubble_sort(RandomIt begin, RandomIt end) {
        if (begin == end) {return;}
        bubble_sort_interior(begin, end);
        return bubble_sort(++begin, end);
            
    }
    template<typename T>
    void heapify(std::vector<T>& input, int n, int i){
        int largest = i;
        int left_child = 2 * i + 1;
        int right_child = 2 * i + 2;

        if (left_child < n && input[left_child] > input[largest]) {
            largest = left_child;
        }
        if (right_child < n && input[right_child] > input[largest]) {
            largest = right_child;
        }
        if (largest != i) {
            std::swap(input[i], input[largest]);

            heapify(input, n, largest);
        }
    }

    
    template<typename T>
    void heap_sort(std::vector<T>& input) {
        size_t size = input.size();
        for (int i = size / 2 - 1; i >= 0; --i) {
            heapify(input, size, i);
        }
        for (int i = size - 1; i > 0; --i) {
            std::swap(input[0], input[i]);
            heapify(input, i, 0);
        }
    }
    template<class RandomIt>
    void heapify(RandomIt begin, RandomIt end, RandomIt cur, int n) {
        RandomIt largest = cur;
        if (begin + 1 < cur) {
            RandomIt left = cur - 1;
            if (*left > *cur) {
                largest = left;
            }
        }
        if (begin + 2 < cur) {
            RandomIt right = cur - 2;
            if (*right > *cur) {
                largest = right;
            }
        }
        if (*largest != *cur) {
            //std::swap(input[i], input[largest]);
            std::swap(*cur, *largest);
            heapify(begin,end, largest,n);
        }
    }


    template<class RandomIt>
    void heap_sort(RandomIt begin, RandomIt end) {
        size_t size = end - begin;
        for (int i = size / 2 - 1; i >= 0; --i) {
            //heapify(begin,end, size, i);
            heapify(begin, end, begin + i,size);
        }
        for (int i = size - 1; i > 0; --i) {
            //std::swap(input[0], input[i]);
            std::swap(*begin, *(begin + i));
            //heapify(begin,end, i, 0);
            heapify(begin, end, begin + i,i);
        }
    }
    //I feel like this is cheating :/
    template <class Iterator>
    void HeapSort(Iterator begin, Iterator end) {
        std::make_heap(begin, end);
        while (begin != end)
            std::pop_heap(begin, end--);

    }
    template <typename Comparable>
    void sort(std::vector<Comparable>& comparables) {
        //heap_sort(comparables);
        //heap_sort(comparables.begin(), comparables.end());
        HeapSort(comparables.begin(), comparables.end());
    }

} // namespace nonstd
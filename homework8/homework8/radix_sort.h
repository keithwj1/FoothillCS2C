#pragma once

#include <algorithm>
#include <vector>
#include <list>
#include <cstddef>
#include <exception>
#include <type_traits>
namespace nonstd {
    using byte = char;
    static const int bucket_size = int(UCHAR_MAX)+1;
    static const int bucket_size_offset = bucket_size / 2;
    //static const int bucket_size_offset = 0;
    class out_of_range : public std::exception {
        virtual const char* what() const throw() {
            return "You have moved the pointer passed the object";
        }
    } out_of_range;
    template<typename T> 
    const byte* get_bytes_start(const T& object){
        const byte* begin = reinterpret_cast<const byte*>(std::addressof(object));
        return begin;
    }
    template<typename Base, typename T>
    inline bool instanceof(T*) {
        return std::is_base_of<Base, T>::value;
    }
    template <typename T>
    class HasBegin{
    private:
        typedef char YesType[1];
        typedef char NoType[2];

        template <typename C> static YesType& test(decltype(&C::Begin));
        template <typename C> static NoType& test(...);


    public:
        enum { value = sizeof(test<T>(0)) == sizeof(YesType) };
    };
    template <typename T>
    void radix_sort_forward(std::vector<T>& input, int index, int max) {
        if (input.empty()) { return; }
        if (input.size() < 2) { return; }
        if (index < 0 || index >= max) { return; }
        std::vector<std::vector<T>> buckets(bucket_size);
        for (auto& elem : input) {
            T* ptr = &elem;
            byte* start = reinterpret_cast<byte*>(ptr);
            byte* cur = start + index;
            byte* end = start + (sizeof(T) / sizeof(byte));
            if (cur > end || cur < start) {
                //TODO: remove this code, it is only here for testing
                //well you searched passed the size of the object, oops
                throw out_of_range;
            }
            buckets[bucket_size_offset + *cur].push_back(elem);
        }
        input.clear();
        for (auto& cur_bucket : buckets) {
            //second round needs to be fixed :/
            //std::sort(cur_bucket.begin(), cur_bucket.end());
            for (auto& elem : cur_bucket) {
                input.push_back(elem);
            }
            cur_bucket.clear();
        }
        radix_sort_forward(input, ++index, max);
    }

    template <typename T>
    void radix_sort_backward(std::vector<T>& input,int index, int max) {
        if (input.empty()) { return; }
        if (index < 0) { return; }
        if (index >= max) { return; }
        std::vector<std::vector<T>> buckets(bucket_size);        
        for (auto& elem : input) {
            T* ptr = &elem;
            byte* start = reinterpret_cast<byte*>(std::addressof(elem));
            //byte* cur = start + index;
            byte* end = start + max + 1;
            byte* cur = end - index;
            if (cur > end || cur < start) {
                //TODO: remove this code, it is only here for testing
                //well you searched passed the size of the object, oops
                throw out_of_range;
            }
            buckets[bucket_size_offset +*cur].push_back(elem);
        }
        input.clear();
        for (auto& cur_bucket : buckets) {
            if (cur_bucket.empty()) { continue; }
            //std::sort(cur_bucket.begin(), cur_bucket.end());
            for (auto& elem : cur_bucket) {
                input.push_back(elem);
            }
            cur_bucket.clear();
        }
        radix_sort_backward(input, ++index,max);
    }

    void radix_sort(std::vector<std::string>& input) {
        using T = std::string;
        const size_t char_size = sizeof(byte);
        //char size is 1, but I'm leaving this here to show the intent of the code.

        std::vector<std::vector<T>> buckets(bucket_size);
        const size_t input_size = sizeof(T);
        size_t max_rounds = (input_size / char_size);

        for (int round = 0; round < int(max_rounds); ++round) {
            //for (int round = max_rounds; round >= 0; --round) {
            for (std::string & elem : input) {
                byte* start = reinterpret_cast<byte*>(std::addressof(elem));
                //byte* cur = start + index;
                byte* end = start + max_rounds + 1;
                byte* cur = end - round;
                if (HasBegin<T>::value) {
                    auto* ptr = &(*elem.begin());
                    start = reinterpret_cast<byte*>(ptr);
                    byte* end = reinterpret_cast<byte*>(&*(elem.end()));
                    cur = end - round;
                    
                }
                if (cur > end || cur < start) {
                    //TODO: remove this code, it is only here for testing
                    //well you searched passed the size of the object, oops
                    throw out_of_range;
                }
                buckets[bucket_size_offset + *cur].push_back(elem);
            }
            input.clear();
            for (auto& cur_bucket : buckets) {
                //second round needs to be fixed :/
                //std::sort(cur_bucket.begin(), cur_bucket.end());
                for (auto& elem : cur_bucket) {
                    input.push_back(elem);
                }
                cur_bucket.clear();
            }
        }

    }

    template <typename T>
    void radix_sort(std::vector<T> &input) {
        const size_t char_size = sizeof(byte);
        //char size is 1, but I'm leaving this here to show the intent of the code.
        
        std::vector<std::vector<T>> buckets(bucket_size);
        const size_t input_size = sizeof(T);
        size_t max_rounds = (input_size / char_size);
        if (HasBegin<T>::value) {
            max_rounds = input.begin() - input.end();
        }
        for (int round = 0; round < int(max_rounds); ++round) {
        //for (int round = max_rounds; round >= 0; --round) {
            for (auto& elem : input) {
                byte* start = reinterpret_cast<byte*>(std::addressof(elem));
                //byte* cur = start + index;
                byte* end = start + max_rounds + 1;
                byte* cur = end - round;
                if (HasBegin<T>::value){
                    auto* ptr = &elem;
                    start = reinterpret_cast<byte*>(ptr);
                    byte* cur = start + round;
                }
                if (cur > end || cur < start) {
                    //TODO: remove this code, it is only here for testing
                    //well you searched passed the size of the object, oops
                    throw out_of_range;
                }
                buckets[bucket_size_offset +*cur].push_back(elem);
            }
            input.clear();
            for (auto& cur_bucket : buckets) {
                //second round needs to be fixed :/
                //std::sort(cur_bucket.begin(), cur_bucket.end());
                for (auto& elem : cur_bucket) {
                    input.push_back(elem);
                }
                cur_bucket.clear();
            }
        }
        
    }

    template <typename Comparable>
    void cust_sort(std::vector<Comparable>& comparables) {
        //std::sort(comparables.begin(), comparables.end());
        const int max_rounds = sizeof(Comparable)/ sizeof(byte);
        //radix_sort_backward(comparables,0,max_rounds);
        radix_sort(comparables);
    }

} // namespace nonstd
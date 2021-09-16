// Example filter program for (Bloom) Filter assignment.
// Note that this filter works but the output filter is too big.
#include <iostream>
#include <fstream>
#include <set>
#include <stdexcept>
#include <string>
#include "bloom_filter.h"
#include <vector>

static const int BITSET_SIZE = 8192;

int test_read_write() {
    std::ofstream write_filter_bin;
    std::string test_string = "AJKSDHKJAHSDKHDBhbvjhxkcbvasdhfiuewrhq3yefhxcbvljkxbcvlasyd8qy3e";
    write_filter_bin.open("test.bin");
    write_filter_bin << test_string;
    write_filter_bin.close();

    std::fstream read_filter_bin;
    
    std::string lines;
    read_filter_bin.open("test.bin", std::ios::in); //open a file to perform read operation using file object
    if (read_filter_bin.is_open()) {   //checking whether the file is open
        std::string line;
        while (std::getline(read_filter_bin, line)) { //read data from file object and put it into string.
            std::cout << line << std::endl; //print the data of the string
            lines += line;
        }
        read_filter_bin.close(); //close the file object.
    }
    std::cout << "Write Lines =" << test_string << std::endl;
    std::cout << "Read Lines =" << lines << std::endl;
    std::cout << (lines == test_string) << std::endl;
    return 0;
}


int test_full_program() {
    ///Write file block
    std::ofstream filter_bin;
    my_bloom_filter<BITSET_SIZE> write_bloom_filter;
    std::ifstream text_file;
    text_file.open("fruits.txt");
    write_bloom_filter.build_filter(text_file);
    text_file.close();
    std::ofstream out("test.bin");
    std::streambuf* coutbuf = std::cout.rdbuf(); //save old buf
    std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!
    write_bloom_filter.write_file();
    std::cout.rdbuf(coutbuf); //reset to standard output again
    out.close();
    //read file block
    my_bloom_filter<BITSET_SIZE> read_bloom_filter;
    std::fstream newfile;
    newfile.open("test.bin", std::ios::in); //open a file to perform read operation using file object
    read_bloom_filter.build_from_filter_file(newfile);
    newfile.close();
    //read_bloom_filter.fill_bits_from_string(lines);
    std::cout << "Write Bits=" << std::endl << write_bloom_filter.get_bits() << std::endl;
    std::cout << "Read Bits=" << std::endl << read_bloom_filter.get_bits() << std::endl;
    std::cout << (write_bloom_filter.get_bits() == read_bloom_filter.get_bits()) << std::endl;
    return 0;
}

int test_conversion() {
    //convert bits to bytes
    std::string raw_bits = "000000000100000000000000000000000001000000000000000000000010000000000000000000000000000000000000000000000000000000000000000000000000000010000001000010000000000000000000000000100010000000000000000100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000010000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000001000000000000000000000000001000000000000000010000000000000000000000000100001000000000000000000000000000010000000001000000000000000000000000000000000000100100000000000000000000000000000001000000000000000000000100001000000000100000000000000000000000000000000000000000000000100000000000000000000000000000000000000000110000000000000000001000000010000000000000000000000000000000000000000000000000000000000000000000000000001000000000000000000000000100000000000100000000000000000100010000100000000000000000000000000000000000000000000000000001000000000110000000000000000000000000000000000010010000000000000010000000000000000000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010000000000010000000000000000000000000000000000000000001000000000000010000000000000000000000000000000000000000000000000000000000000000000000000001000000100000000000100000000000000000000000000000000000000000000000000000000100000100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000001000000000000000000100000000000000000100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001000000000001000000000000000000000000000000000000000001000010000000000000000000000000000000000000100000000000000100000000000000000000000000000000000000000000000000000000000000000000000000000000000000100000100000000000000000000000000000000000001000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000000000000000000001000000000001000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100000000000000000000000001000000000000000000100100000000000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000100000000000010000000000000001000000000000011000000000000000000000000000000000000000000000000000000000110010010000000000000000000000000000000000000000000000000000001000000000010000000000000000000000000000000000000000";
    std::bitset<BITSET_SIZE> bits(raw_bits);
    
    std::stringstream bits_to_bytes;
    /* This code is better but it isn't mine :(
    std::bitset<BITSET_SIZE> mask(0xff);
    
    for (int i = 0; i < BITSET_SIZE / 8; ++i) {
        auto byte = static_cast<uint8_t>(((bits >> (8 * i))& mask).to_ulong());
        std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
        bits_to_bytes << byte;
    }*/
    for (size_t k = 0; k < bits.size() / 8; ++k) {
        std::bitset<8> cur;
        for (size_t i = 0; i < cur.size(); ++i) {
            cur[i] = bits[(k * 8) + i];
        }
        uint8_t byte = static_cast<uint8_t>(cur.to_ulong());
        bits_to_bytes << byte;
    }
    std::string sbits = bits_to_bytes.str();
    std::cout << sbits << std::endl;
    //convert bytes to bits;

    my_bloom_filter<BITSET_SIZE> bloom_filter;
    bloom_filter.fill_bits_from_string(sbits);
    std::cout << (bits == bloom_filter.get_bits()) << std::endl;
    return 0;
}
int test_filter() {
    auto filter = my_bloom_filter<BITSET_SIZE>();
    filter.insert("apple");
    filter.insert("grape");
    filter.insert("strawberry");
    filter.insert("bob");
    filter.insert("green");
    filter.insert("red");
    filter.insert("orange");
    std::vector<std::string> search = { "bob","apple","grape","green","red","strawberry","orange","sky","apples", "purple" };
    for (auto &elem : search) {
        if (filter.maybe_contains(elem)) {
            std::cout << "Maybe Contains " << elem << std::endl;
        }
        else {
            std::cout << "Does Not Contain " << elem << std::endl;
        }
    }
    return 0;
}

int print_args(int &argc, const char* argv[]) {
    for (int i = 0; i < argc; ++i) {
        std::cout << "argv[" << i << "] is " << argv[i] << std::endl;
    }
    std::cout << "argc is " << argc << std::endl;
    std::cout << "Pres Enter to continue:" << std::endl;
    std::cin.get();
    return 0;
}

int bloom_filter_hw(int &argc, const char* argv[]) {
    if (argc > 1) { // if I've been given a filename
        std::ifstream filter_file(argv[1]);
        auto filter = my_bloom_filter<BITSET_SIZE>();
        std::string line;
        // Read in the file to create my filter object
        filter.build_from_filter_file(filter_file);
        // Read lines from std::cin
        //   output the line if it "passes" the filter
        for (std::string line; std::getline(std::cin, line); ) {
            if (filter.maybe_contains(line)) {
                std::cout << line << std::endl;
            }
            
        }
        if (!std::cin.eof()) {
            throw std::runtime_error("Error reading from std::cin.");
        }
    }
    else { // else I've NOT been given a filename
   // Create an empty filter
        // Create an empty filter
        auto filter = my_bloom_filter<BITSET_SIZE>();
        // Read lines from std::cin and add each line to filter
        filter.build_filter(std::cin);
        // Output filter to std::cout
        filter.write_file();
    }
    return 0;
}

void build_filter(std::istream& is, std::set<std::string>* filter) {
    for (std::string line; std::getline(is, line); ) {
        filter->insert(line);
    }
    if (!is.eof()) {
        throw std::runtime_error("Error reading from istream.");
    }
}

int orignal_filter(int &argc, const char* argv[]) {
    if (argc > 1) { // if I've been given a filename
        std::ifstream filter_file(argv[1]);
        std::set<std::string> filter_lines;
        std::string line;
        // Read in the file to create my filter object
        build_filter(filter_file, &filter_lines);
        // Read lines from std::cin
        //   output the line if it "passes" the filter
        for (std::string line; std::getline(std::cin, line); ) {
            if (filter_lines.find(line) != filter_lines.end()) {
                std::cout << line << std::endl;
            }
        }
        if (!std::cin.eof()) {
            throw std::runtime_error("Error reading from std::cin.");
        }
    }
    else { // else I've NOT been given a filename
   // Create an empty filter
        std::set<std::string> filter_lines;
        // Read lines from std::cin and add each line to filter
        build_filter(std::cin, &filter_lines);
        // Output filter to std::cout
        for (const std::string& filter_line : filter_lines) {
            std::cout << filter_line << std::endl;
        }
    }
    return 0;
}

int main(int argc, const char* argv[]) {
    //return test_full_program();
    //return test_read_write();
    //return test_filter();
    //return test_conversion();
    return bloom_filter_hw(argc, argv);
}
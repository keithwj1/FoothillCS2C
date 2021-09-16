#pragma once
#include <iostream>
#include <bitset>
#include <utility>
#include <string>
#include <stdexcept>
#include <set>
#include <sstream>

#ifndef INCLUDE_BLOOM_FILTER_H
#define INCLUDE_BLOOM_FILTER_H
template <size_t NUM_BITS>
class my_bloom_filter {
private:
	std::bitset<NUM_BITS> bits;
	
	
public:
	my_bloom_filter() {}
	~my_bloom_filter() {}
	const std::bitset<NUM_BITS> get_bits() const {
		return bits;
	}
	const unsigned int hash(const std::string& value) const {
		/*std::hash<std::string> hasher;
		auto hashed = hasher(value);
		return hashed % bits.size();*/
		return std::hash<std::string>{}(value) % bits.size();
	}
	unsigned int insert(const std::string& value) {
		auto h = hash(value);
		bits[h] = 1; 
		return h;
	}  
	bool maybe_contains(const std::string& value) { 
		return bits.test(hash(value));
	}
	void build_filter(std::istream& is) {
		for (std::string line; std::getline(is, line); ) {
			insert(line);
		}
		if (!is.eof()) {
			throw std::runtime_error("Error reading from istream.");
		}
	}
	void write_file() {
		for (size_t k = 0; k < bits.size() / 8; ++k) {
			std::bitset<8> cur;
			for (size_t i = 0; i < cur.size(); ++i) {
				cur[i] = bits[(k * 8) + i];
			}
			uint8_t byte = static_cast<uint8_t>(cur.to_ulong());
			std::cout << byte;
		}
	}
	void fill_bits_from_string(std::string string) {
		int h = 0;
		for (std::size_t i = 0; i < string.size(); ++i)
		{
			//std::cout << std::bitset<8>(string.c_str()[i]) << std::endl;
			std::bitset<8> cur_bits(string.c_str()[i]);
			for (size_t j = 0; j < cur_bits.size(); ++j) {
				bits[h] = cur_bits[j];
				++h;
			}

		}
	}
	//file is stored in bytes
	void build_from_filter_file(std::istream& is) {
		/*std::string line;
		std::string lines;
		while (std::getline(is, line)) {
			std::cout << line << std::endl;
			lines += line;
		}
		fill_bits_from_string(lines);*/
		char c = 0;
		std::string str;
		while (is.get(c)) {
			str += c;
		}
		fill_bits_from_string(str);
	}
	//If file is stored in raw binary
	void build_from_filter_file_raw(std::istream& is) {
		std::string line; 
		std::getline(is, line);
		std::bitset<NUM_BITS> cur_bits(line.c_str());
		//bits.reset();
		bits = cur_bits;
		print();
		std::cout << std::endl;
	}
	void print() const {
		std::cout << bits;
	}
};


#endif
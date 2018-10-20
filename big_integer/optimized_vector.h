//
//Created by Halasas on 29.09.18
//
#ifndef OPTIMIZED_VECTOR_H
#define OPTIMIZED_VECTOR_H
#include "stdafx.h"
#include <vector>
#include <memory>
struct optimized_vector {
private:
	const size_t SMALL_SIZE = 256;
		std::shared_ptr<std::vector<uint32_t>> big;
		uint32_t small[256];
	size_t size_;
	bool is_small() const;
	void check_unique();
public:
	optimized_vector();
	optimized_vector(size_t size, uint32_t x);
	~optimized_vector();
	size_t size() const;
	bool empty() const;
	void clear();
	void resize(size_t new_size);
	uint32_t back() const;
	uint32_t &back();
	void push_back(uint32_t x);
	void pop_back();
	uint32_t operator[](size_t ind) const;
	uint32_t &operator[](size_t ind);
	optimized_vector &operator= (optimized_vector const &other);
	bool equal(optimized_vector const &b) const;
};
bool operator==(optimized_vector const &a, optimized_vector const &b);
#endif  OPTIMIZED_VECTOR_H 
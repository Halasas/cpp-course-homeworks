//
//Created by Halasas on 29.09.18
//

#ifndef OPTIMIZED_VECTOR_H
#define OPTIMIZED_VECTOR_H

#include "stdafx.h"
#include <vector>
#include <memory>

struct opt_vector {
private:
		std::shared_ptr<std::vector<uint32_t>> big;
		uint32_t small[5];
		size_t size_;
		bool is_small() const;
		void check_unique();
public:
	opt_vector();
	~opt_vector();

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

	opt_vector &operator= (opt_vector const &other);
	bool equal(opt_vector const &b) const ;
};

bool operator==(opt_vector const &a, opt_vector const &b);

#endif  OPTIMIZED_VECTOR_H
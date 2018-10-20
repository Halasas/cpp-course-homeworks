//
//Created by Halasas on 29.09.18
//
#include "optimized_vector.h"
bool optimized_vector::is_small() const {
	return size() <= SMALL_SIZE;
}
void optimized_vector::check_unique() {
	if (big.unique())
		return;
	big = std::make_shared<std::vector<uint32_t>>(*big);
}
optimized_vector::optimized_vector() {
	size_ = 0;
}
optimized_vector::optimized_vector(size_t size, uint32_t x) {
	if (size > SMALL_SIZE)
	{
		big = std::make_shared<std::vector<uint32_t>>();
		big->resize(size, x);
	}
	else
	{
		for (size_t i = 0; i < size; ++i)
			small[i] = x;
	}
	size_ = size;
}
optimized_vector::~optimized_vector() {
	big.reset();
}
size_t optimized_vector::size() const {
	return size_;
}
bool optimized_vector::empty() const {
	return size() == 0;
}
void optimized_vector::clear() {
	big.reset();
	size_ = 0;
}
void optimized_vector::resize(size_t new_size) {
	if (!is_small()) {
		if (new_size <= SMALL_SIZE) {
			for (size_t i = 0; i < new_size; ++i)
				small[i] = operator[](i);
			big.reset();
		}
		else {
			check_unique();
			big->resize(new_size);
		}
	}
	else if (new_size > SMALL_SIZE) {
		big = std::make_shared<std::vector<uint32_t>>();
		big->resize(new_size);
		for (size_t i = 0; i < SMALL_SIZE; ++i) {
			big->operator[](i) = small[i];
		}
	}
	else {
		for (int i = size_; i < new_size; i++) {
			small[i] = 0;
		}
	}
	if (new_size < 0)
		new_size = 0;
	size_ = new_size;
}
uint32_t optimized_vector::back() const {
	if (is_small())
		return small[size() - 1];
	return big->operator[](size() - 1);
}
uint32_t &optimized_vector::back() {
	if (is_small())
		return small[size() - 1];
	check_unique();
	return big->operator[](size() - 1);
}
void optimized_vector::push_back(uint32_t x) {
	resize(size_ + 1);
	this->operator[](size() - 1) = x;
}
void optimized_vector::pop_back() {
	resize(size_ - 1);
}
uint32_t optimized_vector::operator[](size_t ind) const {
	if (is_small())
		return small[ind];
	else
		return big->operator[](ind);
}
uint32_t & optimized_vector::operator[](size_t ind) {
	if (is_small())
		return small[ind];
	else {
		check_unique();
		return big->operator[](ind);
	}
}
optimized_vector & optimized_vector::operator=(optimized_vector const & other) {
	if (!is_small())
		big.reset();
	if (!other.is_small())
		big = std::shared_ptr<std::vector<uint32_t>>(other.big);
	else
		for (size_t i = 0; i < other.size(); ++i)
			small[i] = other.small[i];
	size_ = other.size();
	return *this;
}
bool optimized_vector::equal(optimized_vector const & b) const {
	if (is_small() ^ b.is_small() || size() != b.size()) {
		return false;
	}
	else if (is_small()) {
		for (size_t i = 0; i < size(); ++i) {
			if (small[i] != b.small[i])
				return false;
		}
		return true;
	}
	return *big == *b.big;
}
bool operator==(optimized_vector const & a, optimized_vector const & b) {
	return a.equal(b);
}

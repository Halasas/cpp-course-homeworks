#include "optimized_vector.h"

bool opt_vector::is_small() const
{
	return size() < 4;
}

void opt_vector::check_unique()
{
	if (big.unique())
		return;
	big.reset();
	big = std::make_shared<std::vector<uint32_t>>(*big);
}

opt_vector::opt_vector()
{
	big->resize(0);
	size_ = 0;
}

opt_vector::~opt_vector()
{
	big.reset();
}

size_t opt_vector::size() const
{
	return size_;
}

bool opt_vector::empty() const
{
	return size() == 0;
}

void opt_vector::clear()
{
	big.reset();
	for (size_t i = 0; i < 4; ++i)
		small[i] = 0;
	size_ = 0;
}

void opt_vector::resize(size_t new_size)
{
	if (is_small) {
		if (new_size > 4) {
			big = std::shared_ptr<std::vector<uint32_t>>();
			big->resize(new_size, 0);
			for (size_t i = 0; i < size(); ++i)
				big->operator[](i) = small[i];
		}
	}
	else {// is_big
		if (new_size < 4) {
			for (size_t i = 0; i < new_size; ++i)
				small[i] = big->operator[](i);
			big.reset();
		}
		else { //size >= 4
			check_unique();
			big->resize(new_size);
		}
	}
	size_ = new_size;
}

uint32_t opt_vector::back() const
{
	if (is_small())
		return small[size() - 1];
	return big->operator[](size() - 1);
}

uint32_t &opt_vector::back()
{
	if (is_small())
		return small[size() - 1];
	check_unique();
	return big->operator[](size() - 1);
}

void opt_vector::push_back(uint32_t x)
{
	resize(++size_);
	this->back = x;
}

void opt_vector::pop_back()
{
	resize(--size_);
}

uint32_t opt_vector::operator[](size_t ind) const
{
	if (is_small()) 
		return small[ind];
	else
		return big->operator[](ind);
}

uint32_t & opt_vector::operator[](size_t ind)
{
	if (is_small()) 
		return small[ind];
	else {
		check_unique();
		return big->operator[](ind);
	}
}

opt_vector & opt_vector::operator=(opt_vector const & other)
{
	if(!is_small())
		big.reset();
	if (other.is_small())
		big = std::shared_ptr<std::vector<uint32_t>>(other.big);
	else
		for (size_t i = 0; i < other.size(); ++i)
			small[i] = other[i];
	size_ = other.size();
	return *this;
}

bool opt_vector::equal(opt_vector const & b) const
{
	if (is_small() ^ b.is_small()) {
		return false;
	}
	else if (is_small()) {
		for (size_t i = 0; i < 4; ++i) {
			if (small[i] != b.small[i])
				return false;
		}
		return true;
	}
	return *big == *b.big;
}

bool operator==(opt_vector const & a, opt_vector const & b)
{
	return a.equal(b);
}

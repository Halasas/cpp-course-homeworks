#ifndef big_integer_h 
#define big_integer_h

#include <functional>
#include <vector>
#include "optimized_vector.h"

class big_integer {
	optimized_vector num;
public:
	bool sign; // "true" - "-"
	//secondary functions
	bool is_zero() const;
	void cut_zero();
	void set_zero();
	size_t size() const;
	friend void abs(big_integer & x);
	friend std::pair<big_integer, uint32_t> div_big_uint32(big_integer const &a, uint32_t const &b);
	friend big_integer mul_big_uint32(big_integer const &a, uint32_t const &b);
	uint32_t get_compliment_zero() const;
	void sh_digits(int rhs);

	big_integer();
	big_integer(big_integer const& x);
	big_integer(int x);
	big_integer(uint32_t x);
	explicit big_integer(const std::string & str);
	~big_integer() = default;

	friend bool operator==(big_integer const& a, big_integer const& b);
	friend bool operator!=(big_integer const& a, big_integer const& b);
	friend bool operator<(big_integer const& a, big_integer const& b);
	friend bool operator>(big_integer const& a, big_integer const& b);
	friend bool operator<=(big_integer const& a, big_integer const& b);
	friend bool operator>=(big_integer const& a, big_integer const& b);

	big_integer& operator=(big_integer const& other);
	big_integer& operator+=(big_integer const& rhs);
	big_integer& operator-=(big_integer const& rhs);
	big_integer& operator*=(big_integer const& rhs);
	
	big_integer& operator/=(big_integer const& rhs);
	big_integer& operator%=(big_integer const& rhs);

	big_integer operator+() const;
	big_integer operator-() const;
	big_integer operator~() const;

	big_integer& operator++();
	big_integer operator++(int);

	big_integer& operator--();
	big_integer operator--(int);

	big_integer& operator&=(big_integer const& rhs);
	big_integer& operator|=(big_integer const& rhs);
	big_integer& operator^=(big_integer const& rhs);

	big_integer& operator<<=(int rhs);
	big_integer& operator>>=(int rhs);
	friend optimized_vector complement(big_integer const& x);
	friend big_integer decomplement(optimized_vector const& x);
	friend big_integer apply_with_complement(big_integer const& x,
		big_integer const& y,
		std::function<uint32_t(uint32_t, uint32_t)> F);
};

optimized_vector complement(big_integer const& x);
big_integer decomplement(optimized_vector const& x);
optimized_vector apply_to_vectors(optimized_vector const& x,
	optimized_vector const& y,
	std::function<uint32_t(uint32_t, uint32_t)> F);

big_integer apply_with_complement(big_integer const& x,
	big_integer const& y,
	std::function<uint32_t(uint32_t, uint32_t)> F);
big_integer operator+(big_integer a, big_integer const& b);
big_integer operator-(big_integer a, big_integer const& b);
big_integer operator*(big_integer a, big_integer const& b);
big_integer operator/(big_integer a, big_integer const& b);
big_integer operator%(big_integer a, big_integer const& b);

big_integer operator&(big_integer a, big_integer const& b);
big_integer operator|(big_integer a, big_integer const& b);
big_integer operator^(big_integer a, big_integer const& b);

big_integer operator<<(big_integer a, int b);
big_integer operator>>(big_integer a, int b);

bool operator==(big_integer const& a, big_integer const& b);
bool operator!=(big_integer const& a, big_integer const& b);
bool operator<(big_integer const& a, big_integer const& b);
bool operator>(big_integer const& a, big_integer const& b);
bool operator<=(big_integer const& a, big_integer const& b);
bool operator>=(big_integer const& a, big_integer const& b);
void abs(big_integer & x);

std::string to_string(big_integer const& a);
std::ostream& operator<<(std::ostream& s, big_integer const& a);
std::pair<big_integer, uint32_t> div_big_uint32(big_integer const &a, uint32_t const &b);
big_integer mul_big_uint32(big_integer const &a, uint32_t const &b);

#endif big_integer_h
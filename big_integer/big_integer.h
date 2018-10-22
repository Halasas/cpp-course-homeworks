#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <sstream>
#include <string>
#include <algorithm>
#include "optimized_vector.h"

enum ops { AND, OR, XOR };

struct big_integer
{
private:
	std::vector<uint32_t> data;
	char sign;
	bool is_zero();
	void cut_zeros();
	big_integer abs(big_integer a);
	friend big_integer mul_uint32(big_integer num, uint32_t const &a);
	friend big_integer div_uint32(big_integer num, uint32_t const &a);

public:
	big_integer();
	big_integer(big_integer const& other);
	big_integer(int a);
	big_integer(uint32_t a);
	big_integer(uint64_t a);
	explicit big_integer(std::string const& str);
	~big_integer() = default;

	big_integer& operator=(big_integer const& other);

	big_integer& operator+=(big_integer const& rhs);
	big_integer& operator-=(big_integer const& rhs);
	big_integer& operator*=(big_integer const& rhs);
	big_integer& operator/=(big_integer const& rhs);
	big_integer& operator%=(big_integer const& rhs);

	big_integer& operator&=(big_integer const& rhs);
	big_integer& operator|=(big_integer const& rhs);
	big_integer& operator^=(big_integer const& rhs);

	big_integer& operator<<=(int rhs);
	big_integer& operator>>=(int rhs);

	big_integer operator+() const;
	big_integer operator-() const;
	big_integer operator~() const;

	big_integer& operator++();
	big_integer operator++(int);

	big_integer& operator--();
	big_integer operator--(int);

	friend bool operator==(big_integer const& a, big_integer const& b);
	friend bool operator!=(big_integer const& a, big_integer const& b);
	friend bool operator<(big_integer const& a, big_integer const& b);
	friend bool operator>(big_integer const& a, big_integer const& b);
	friend bool operator<=(big_integer const& a, big_integer const& b);
	friend bool operator>=(big_integer const& a, big_integer const& b);

	friend std::string to_string(big_integer const& a);
	friend big_integer to_compliment(big_integer a);
	friend big_integer from_compliment(big_integer a);
	friend big_integer apply(big_integer a, big_integer b, ops op);
};

big_integer operator+(big_integer a, big_integer const& b);
big_integer operator-(big_integer a, big_integer const& b);
big_integer operator*(big_integer a, big_integer const& b);
big_integer operator*(big_integer a, uint32_t const& b);
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

std::string to_string(big_integer const& a);
std::ostream &operator<<(std::ostream& s, big_integer const& a);
big_integer apply(big_integer a, big_integer b, ops op);
big_integer to_compliment(big_integer a);
big_integer from_compliment(big_integer a);
#endif // BIG_INTEGER_H
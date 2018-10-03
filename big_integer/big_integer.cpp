#include "big_integer.h"
#include <algorithm>
#include <iostream>
#include <sstream>


size_t big_integer::size() const {
	return num.size();
}
void big_integer::cut_zero() {
	while (size() > 1 && num.back() == 0) {
		num.pop_back();
	}
}
void big_integer::set_zero() {
	this ->operator=(big_integer());
}
void abs(big_integer &x) {
	x.sign = false;
}
std::pair<big_integer, uint32_t> div_big_uint32(big_integer const &a, uint32_t const &b) {
	big_integer res = a;
	uint64_t carry = 0, cur;
	for (size_t i = res.num.size(); i > 0; --i) {
		cur = (carry << 32) + res.num[i - 1];
		res.num[i - 1] = (uint32_t)(cur / b);
		carry = (uint32_t)(cur % b);
	}
	res.cut_zero();
	return { res, carry };
}
big_integer mul_big_uint32(big_integer const &a, uint32_t const &b) {
	big_integer res = a;
	bool sign = res.sign;
	abs(res);
	res.num.push_back(0);
	uint32_t carry = 0;
	for (size_t i = 0; i < res.num.size(); ++i) {
		uint64_t cur = (uint64_t)res.num[i] * b + carry;
		res.num[i] = (cur << 32) >> 32;
		carry = (uint32_t)(cur >> 32);
	}
	if (sign) {
		res = -res;
	}
	res.cut_zero();
	return res;
}
void big_integer::sh_digits(int rhs) {
	if (rhs > 0) {
		size_t oldLen = num.size();
		num.resize(oldLen + rhs);
		for (size_t i = oldLen; i > 0; --i) {
			num[i + rhs - 1] = num[i - 1];
		}
		for (auto i = (size_t)rhs; i > 0; --i) {
			num[i - 1] = 0;
		}
	}
	else {
		for (auto i = (size_t)-rhs; i < num.size(); ++i) {
			num[i + rhs] = num[i];
		}
		uint32_t emptyBlock = get_compliment_zero();
		for (size_t i = num.size(); i > num.size() + rhs; --i) {
			num[i - 1] = emptyBlock;
		}
		cut_zero();
	}
}
uint32_t big_integer::get_compliment_zero() const {
	if (this->sign) {
		return (uint32_t)((uint64_t)(1<<32) - 1);
	}
	return 0; 
}


big_integer::big_integer() : big_integer(0) {}
big_integer::big_integer(big_integer const &x) : num(x.num), sign(x.sign) {}
big_integer::big_integer(int x) : num(1, std::abs(x)), sign(x < 0) {}
big_integer::big_integer(uint32_t x) : num(1, x), sign(false) {}
big_integer::big_integer(std::string const &str) {
	size_t i = 0;
	bool s = false;
	if (str[i] == '-' || str[i] == '+') {
		if(str[i] == '-')
			s = true;
		i++;
	}

	for (; i < str.size(); i++) {
		if (this->num.size() == 8)
			int kek = 0;
		*this = mul_big_uint32(*this, 10);
		*this += (str[i] - '0');
	}
	if (!is_zero())
		sign = s;
}


big_integer & big_integer::operator=(big_integer const & other)
{
	this->num = other.num;
	this->sign = other.sign;
	return *this;
}
big_integer &big_integer::operator+=(big_integer const &rhs) {
	if (this->sign == rhs.sign) {
		uint32_t carry = 0;
		for (size_t i = 0; i < std::max(size(), rhs.size()) || carry; i++) {
			if (i == size()) {
				num.push_back(0);
			}
			uint64_t tmp = num[i];
			tmp += carry;
			tmp += (i < rhs.size() ? rhs.num[i] : 0);
			carry = (tmp >> 32);
			num[i] = tmp;
		}
	}
	else {
		bool s1 = sign;
		bool s2 = rhs.sign;

		big_integer trhs(rhs);

		sign = false;
		trhs.sign = false;

		if (*this >= trhs) {
			int carry = 0;
			for (size_t i = 0; i < trhs.size() || carry; i++) {
				int64_t tmp = num[i];
				tmp -= carry;
				tmp -= (i < trhs.size() ? trhs.num[i] : 0);
				carry = tmp < 0;
				if (carry) tmp += 1ll << 32;
				num[i] = tmp;
			}

			sign = s1;
		}
		else {
			int carry = 0;
			for (size_t i = 0; i < size() || carry; i++) {
				int64_t tmp = trhs.num[i];
				tmp -= carry;
				tmp -= (i < size() ? num[i] : 0);
				carry = tmp < 0;
				if (carry) tmp += ((int64_t)1ll << 32);
				trhs.num[i] = tmp;
			}

			sign = s2;
			std::swap(num, trhs.num);
		}
	}

	cut_zero();
	if (is_zero()) {
		sign = false;
	}
	return *this;
}
big_integer &big_integer::operator-=(const big_integer &rhs) {
	operator+=(-rhs);
	return *this;
}
big_integer &big_integer::operator*=(const big_integer &rhs) {
    if (rhs.is_zero() || is_zero()) {
        set_zero();
        return *this;
    }
	optimized_vector tmp(rhs.size() + size(), 0);
    for (int i = 0; i < size(); i++) {
        uint32_t carry = 0;
        for (int j = 0; j < rhs.size() || carry; j++) {
            uint64_t cur = tmp[i + j]
                           + num[i] * ((uint64_t) 1) * (j < rhs.size() ? rhs.num[j] : 0)
                           + carry;

            tmp[i + j] = (cur << 32) >> 32;
            carry = cur >> 32;
        }
    }
    num = tmp;
    sign ^= rhs.sign;
    cut_zero();
    return *this;
}

big_integer &big_integer::operator/=(big_integer const &rhs) {
	big_integer res, current;
	big_integer b = rhs;
	bool s0 = sign, s1 = rhs.sign;
	abs(*this);
	abs(b);
	if (*this < b)
		return *this = 0;
	if (b.num.size() == 1)
		res = div_big_uint32(*this, b.num[0]).first;
	else {
		for (int i = this->num.size() - 1; i >= 0; --i) {
			current <<= 32;
			current += this->num[i];
			uint32_t x = 0;
			uint64_t l = 0, r = (1ll << 32) - 1;
			while (l <= r)
			{
				uint64_t m = (l + r) >> 1;
				if (mul_big_uint32(b, m) <= current)
				{
					x = m;
					l = m + 1;
				}
				else
					r = m - 1;
			}
			res <<= 32;
			res += x;
			current = current - b * x;
		}
	}
	res.cut_zero();
	if (s0 ^ s1)
		res = -res;
	return *this = res;
}
big_integer &big_integer::operator%=(big_integer const &b) {
	return *this -= (*this / b) * b;
}

big_integer &big_integer::operator&=(big_integer const &rhs) {
	*this = apply_with_complement(*this, rhs, [](uint32_t x, uint32_t y) -> uint32_t { return x & y; });
	if (is_zero()) {
		sign = false;
	}
	return *this;
}

big_integer &big_integer::operator|=(big_integer const &rhs) {
	*this = apply_with_complement(*this, rhs, [](uint32_t x, uint32_t y) -> uint32_t { return x | y; });
	if (is_zero()) {
		sign = false;
	}

	return *this;
}

big_integer &big_integer::operator^=(big_integer const &rhs) {
	*this = apply_with_complement(*this, rhs, [](uint32_t x, uint32_t y) -> uint32_t { return x ^ y; });
	if (is_zero()) {
		sign = false;
	}

	return *this;
}

big_integer &big_integer::operator<<=(int rhs) {
	if (rhs < 0) {
		return *this >>= -rhs;
	}

	this->num = complement(*this);
	int auxShift = rhs / 32; 
	if (auxShift) {
		sh_digits(auxShift);
	}
	auxShift = rhs - auxShift * 32;
	if (auxShift) {
		num.push_back(get_compliment_zero());
		for (size_t i = num.size(); i > 0; --i) {
			if (i != num.size()) {
				num[i] += num[i - 1] >> (32 - auxShift);
			}
			num[i - 1] <<= auxShift;
		}
	}
	if (sign)
		*this = decomplement(this->num);
	cut_zero();
	return *this;
}

big_integer &big_integer::operator>>=(int rhs) {
	if (rhs < 0) {
		return *this <<= -rhs;
	}
	this->num = complement(*this);
	int auxShift = rhs / 32;
	if (auxShift) {
		sh_digits(-auxShift);
	}
	auxShift = rhs - auxShift * 32;
	if (auxShift) {
		uint32_t emptyBlock = get_compliment_zero();
		for (size_t i = 0; i < num.size(); ++i) {
			if (i != 0) {
				num[i - 1] += num[i] << (32 - auxShift);
			}
			num[i] >>= auxShift;
		}
		num.back() += emptyBlock << (32 - auxShift);
	}
	if (sign)
		*this = decomplement(this->num);
	cut_zero();
	return *this;
}
big_integer big_integer::operator+() const {
	return *this;
}

big_integer big_integer::operator-() const {
	if (is_zero()) {
		return *this;
	}

	big_integer tmp(*this);

	tmp.sign ^= 1;
	return tmp;
}

big_integer big_integer::operator~() const {
	return -(*this) - 1;
}

big_integer &big_integer::operator++() {
	return operator+=(1);
}

big_integer big_integer::operator++(int) {
	big_integer tmp(*this);
	operator+=(1);
	return tmp;
}

big_integer &big_integer::operator--() {
	return operator-=(1);
}

big_integer big_integer::operator--(int) {
	big_integer tmp(*this);
	operator-=(1);
	return tmp;
}

bool operator==(big_integer const &a, big_integer const &b) {
	return a.sign == b.sign && a.num == b.num;
}

bool operator!=(big_integer const &a, big_integer const &b) {
	return !(a == b);
}

bool operator<(big_integer const &a, big_integer const &b) {
	if (a.sign != b.sign) { return !a.sign; }
	if (a.size() != b.size()) { return a.size() < b.size(); }

	for (std::ptrdiff_t i = a.size() - 1; i >= 0; i--) {
		if (a.num[i] != b.num[i]) {
			return a.num[i] < b.num[i];
		}
	}
	return false;
}

bool operator>(big_integer const &a, big_integer const &b) {
	if (a.sign != b.sign) { return a.sign; }
	if (a.size() != b.size()) { return a.size() > b.size(); }

	for (std::ptrdiff_t i = a.size() - 1; i >= 0; i--) {
		if (a.num[i] != b.num[i]) {
			return a.num[i] > b.num[i];
		}
	}

	return false;
}

bool operator<=(big_integer const &a, big_integer const &b) {
	if (a.sign != b.sign) { return !a.sign; }
	if (a.size() != b.size()) { return a.size() < b.size(); }

	for (std::ptrdiff_t i = a.size() - 1; i >= 0; i--) {
		if (a.num[i] != b.num[i]) {
			return a.num[i] < b.num[i];
		}
	}

	return true;
}

bool operator>=(big_integer const &a, big_integer const &b) {
	if (a.sign != b.sign) { return a.sign; }
	if (a.size() != b.size()) { return a.size() > b.size(); }

	for (std::ptrdiff_t i = a.size() - 1; i >= 0; i--) {
		if (a.num[i] != b.num[i]) {
			return a.num[i] > b.num[i];
		}
	}
	return true;
}

std::string to_string(big_integer const &a) {
	if (a.is_zero()) {
		return "0";
	}

	std::string s;
	big_integer tmp(a);

	while (!tmp.is_zero()) {
		std::pair<big_integer, uint32_t> res = div_big_uint32(tmp ,10);
		s += (char)res.second + '0';
		tmp = res.first;
	}

	if (a.sign) {
		s += '-';
	}

	std::reverse(s.begin(), s.end());

	return s;
}

std::ostream &operator<<(std::ostream &s, big_integer const &a) {
	return s << to_string(a);
}

bool big_integer::is_zero() const {
	return size() == 1 && num[0] == 0;
}

big_integer operator+(big_integer a, big_integer const &b) {
	a += b;
	return a;
}

big_integer operator-(big_integer a, big_integer const &b) {
	a -= b;
	return a;
}

big_integer operator*(big_integer a, big_integer const &b) {
	a *= b;
	return a;
}

big_integer operator/(big_integer a, big_integer const &b) {
	a /= b;
	return a;
}

big_integer operator%(big_integer a, big_integer const &b) {
	a %= b;
	return a;
	return a;
}

big_integer operator&(big_integer a, big_integer const &b) {
	a &= b;
	return a;
}

big_integer operator|(big_integer a, big_integer const &b) {
	a |= b;
	return a;
}

big_integer operator^(big_integer a, big_integer const &b) {
	a ^= b;
	return a;
}

big_integer operator<<(big_integer a, int b) {
	return a <<= b;
}

big_integer operator>>(big_integer a, int b) {
	return a >>= b;
}


optimized_vector complement(big_integer const &x) {
	if (!x.sign)
		return x.num;

	big_integer tmp(x);
	tmp.sign = false;
	for (size_t i = 0; i < tmp.size(); i++) {
		tmp.num[i] = ~tmp.num[i];
	}

	tmp++;
	return tmp.num;
}

big_integer decomplement(optimized_vector const &x) {
	if (x.back() & (1 << 31)) {
		big_integer tmp;
		tmp.num = x;

		tmp--;
		for (size_t i = 0; i < tmp.size(); i++) {
			tmp.num[i] = ~tmp.num[i];
		}

		tmp.sign = true;
		return tmp;
	}
	else {
		big_integer tmp;
		tmp.num = x;
		return tmp;
	}
}

optimized_vector apply_to_vectors(optimized_vector const &x, optimized_vector const &y,
										std::function<uint32_t(uint32_t, uint32_t)> F) {
	optimized_vector tmp(x);
	if (tmp.size() < y.size()) {
		tmp.resize(y.size());
	}

	for (int i = 0; i < tmp.size(); i++) {
		tmp[i] = F(tmp[i], (i < y.size() ? y[i] : 0));
	}

	return tmp;
}

big_integer apply_with_complement(big_integer const &x,
	big_integer const &y,
	std::function<uint32_t(uint32_t, uint32_t)> F) {
	return decomplement(apply_to_vectors(complement(x),	complement(y),	std::move(F)));
}
#include "big_integer.h"

bool big_integer::is_zero() {
	cut_zeros();
	if (data.size() == 1 && data[0] == 0)
		return true;
	return false;
}

void big_integer::cut_zeros() {
	while (data.size() > 1 && data.back() == 0)
		data.pop_back();
}

big_integer big_integer::abs(big_integer a) {
	a.sign = '+';
	return a;
}


big_integer::big_integer() : data(1, 0), sign('+') {}
big_integer::big_integer(big_integer const & other) : data(other.data), sign(other.sign) {}
big_integer::big_integer(int a) : data(1, (uint32_t)std::abs((int64_t)a)), sign(a < 0 ? '-' : '+') {}
big_integer::big_integer(uint32_t a) : data(1, a), sign('+') {}
big_integer::big_integer(uint64_t a) {
	data.resize(2);
	data[0] = ((a << 32) >> 32);
	data[1] = (a >> 32);
	sign = '+';
}
big_integer::big_integer(std::string const & str) {
	data = std::vector<uint32_t>(1, 0);
	char s = '+';
	sign = '+';
	size_t i = 0;
	if ((str[i] == '-') || (str[i] == '+'))
		s = str[i++];

	for (; i < str.size(); ++i) {
		*this *= 10;
		*this += (str[i] - '0');
	}
	if (is_zero())
		s = '+';
	this->sign = s;
}

big_integer & big_integer::operator=(big_integer const & other)
{
	data = other.data;
	sign = other.sign;
	return *this;
}

big_integer & big_integer::operator+=(big_integer const & rhs)
{
	if (sign != rhs.sign)
	{
		big_integer b = rhs;
		char s = sign;
		b.sign = '+';
		sign = '+';
		*this = (s == '-') ? b -= *this : *this -= b;
		return *this;
	}
	uint32_t carry = 0;
	ptrdiff_t i = 0;
	while (true) {
		if (carry == 0 && i >= rhs.data.size())
			break;
		if (i == data.size())
			data.push_back(0);
		uint64_t tmp = (uint64_t)data[i] + (uint64_t)carry;
		if (i < rhs.data.size())
			tmp += (uint64_t)rhs.data[i];
		carry = tmp >> 32;
		data[i] = (tmp << 32) >> 32;
		++i;
	}
	cut_zeros();
	return *this;
}

big_integer & big_integer::operator-=(big_integer const & rhs)
{
	if (sign != rhs.sign) {
		big_integer b = rhs;
		b.sign = (b.sign == '-' ? '+' : '-');
		return *this += rhs;
		return *this += rhs;
	}
	char s = this->sign;
	big_integer a = abs(*this);
	big_integer b = abs(rhs);
	if (a < b)
		std::swap(a, b), s = (s == '-' ? '+' : '-');
	bool carry = 0;
	for (ptrdiff_t i = 0; i < a.data.size(); ++i) {
		int64_t tmp = a.data[i];
		if (carry)
			--tmp, carry = 0;
		if (i < b.data.size())
			tmp -= b.data[i];
		if (tmp < 0)
			carry = 1, tmp += 1ll << 32;
		a.data[i] = (uint32_t)tmp;
	}
	*this = a;
	if (!is_zero())
		sign = s;
	cut_zeros();

	return *this;
}

big_integer mul_uint32(big_integer num, uint32_t const & a) {
	num.data.push_back(0);
	uint64_t carry = 0;
	for (int i = 0; i < num.data.size(); i++) {
		uint64_t tmp = (uint64_t)num.data[i] * a + carry;
		carry = tmp >> 32;
		num.data[i] = (tmp << 32) >> 32;
	}
	num.cut_zeros();
	return num;
}

big_integer operator*(big_integer a, uint32_t const & b)
{
	return a *= b;
}

big_integer operator/(big_integer a, big_integer const & b)
{
	return a /= b;
}

big_integer operator%(big_integer a, big_integer const & b)
{
	return a -= (a / b * b);
}

big_integer operator&(big_integer a, big_integer const & b)
{
	return apply(a, b, AND);
}

big_integer operator|(big_integer a, big_integer const & b)
{
	return apply(a, b, OR);
}

big_integer operator^(big_integer a, big_integer const & b)
{
	return apply(a, b, XOR);
}

big_integer operator<<(big_integer a, int b)
{
	return a <<= b;
}

big_integer operator>>(big_integer a, int b)
{
	return a >>= b;
}

big_integer & big_integer::operator*=(big_integer const & rhs) {
	big_integer tmp;
	char s = '+';
	if (sign ^ rhs.sign)
		s = '-';
	this->sign = '+';
	big_integer rhs2 = abs(rhs);
	for (int i = 0; i < rhs2.data.size(); ++i) {
		tmp += (mul_uint32(*this, rhs2.data[i])) << (32 * i);
	}
	tmp.sign = s;
	*this = tmp;
	return *this;
}

big_integer div_uint32(big_integer num, uint32_t const &a)
{
	if (a == 1)
		return num;
	if (num == a)
		return num = big_integer(1);
	char s = num.sign;
	num.sign = '+';
	if (num < a)
		return num = big_integer();
	big_integer res;
	res.data.resize(num.data.size());
	uint32_t carry = 0;
	for (ptrdiff_t i = num.data.size() - 1; i >= 0; --i) {
		uint64_t tmp = ((uint64_t)carry << 32);
		tmp += num.data[i];
		uint32_t cur = (uint32_t)(tmp / a);
		carry = tmp % a;
		res.data[i] = cur;
	}
	res.sign = s;
	num = res;
	num.cut_zeros();
	return num;
}

big_integer & big_integer::operator/=(big_integer const & rhs) {
	big_integer b = rhs;
	char s = (sign != b.sign ? '-' : '+');
	this->sign = '+';
	b.sign = '+';
	if (*this < b) {
		*this = big_integer();
		return *this;
	}
	if (*this == b)
	{
		*this = big_integer(1);
		return *this;
	}
	if (rhs.data.size() == 1)
	{
		sign = s;
		return *this = div_uint32(*this, rhs.data.back());
	}
	uint32_t f = (uint32_t)(((uint64_t)1 << 32) / (rhs.data.back() + 1));
	big_integer res;
	res.data.resize(data.size() - rhs.data.size() + 1);
	*this *= f;
	b *= f;
	big_integer buf;
	buf.data.resize(b.data.size());
	ptrdiff_t j = data.size() - 1;
	for (ptrdiff_t i = buf.data.size() - 1; i >= 0; --j, --i)
		buf.data[i] = data[j];
	for (ptrdiff_t i = data.size() - b.data.size() - 1; i >= 0; --i) {
		buf <<= 32;
		buf += data[i];
		uint64_t m = (uint64_t)buf.data.back();
		if (buf.data.size() > b.data.size())
			m <<= 32, m += buf.data[buf.data.size() - 2];
		m /= b.data.back();
		uint32_t ml = ((m << 32) >> 32);
		uint32_t mh = (m >> 32);
		if (mh != 0) 
			res.data[i + 1] += mh;
		big_integer bm = b * big_integer(m);
		while (bm > buf) m--, ml--, bm -= b;
		res.data[i] = ml;
		buf -= bm;
	} 
	res.cut_zeros();
	*this = res;
	sign = s;
	return *this;
}

big_integer & big_integer::operator%=(big_integer const & rhs)
{
	return *this = *this % rhs;
}

big_integer & big_integer::operator&=(big_integer const & rhs)
{
	return *this = *this & rhs;
}

big_integer & big_integer::operator|=(big_integer const & rhs)
{
	return *this = *this | rhs;
}

big_integer & big_integer::operator^=(big_integer const & rhs)
{
	return *this = *this ^ rhs;
}

big_integer & big_integer::operator<<=(int rhs) {
	int num_of_blocks = rhs / 32;
	if (num_of_blocks > 0) {
		this->data.resize(data.size() + num_of_blocks);
		for (ptrdiff_t i = data.size() - 1; i >= num_of_blocks; --i)
			data[i] = data[i - num_of_blocks];
		for (int i = num_of_blocks - 1; i >= 0; --i)
			data[i] = 0;
	}
	rhs -= num_of_blocks * 32;
	data.push_back(0);
	for (ptrdiff_t i = data.size() - 2; i >= 0; --i) {
		uint64_t tmp = data[i];
		tmp <<= rhs;
		data[i + 1] |= (uint32_t)(tmp >> 32);
		data[i] = (uint32_t)((tmp << 32) >> 32);
	}
	cut_zeros();
	return *this;
}

big_integer & big_integer::operator>>=(int rhs) {
	*this = to_compliment(*this);
	int num_of_blocks = rhs / 32;
	if (num_of_blocks > 0) {
		for (int i = 0; i < data.size() - num_of_blocks - 1; ++i)
			data[i] = data[i + num_of_blocks], data[i + num_of_blocks] = 0;
	}
	rhs -= num_of_blocks * 32;
	for (int i = 0; i < data.size() - num_of_blocks; ++i) {
		uint64_t tmp = data[i];
		tmp <<= 32;
		tmp >>= rhs;
		data[i] = (uint32_t)(tmp >> 32);
		if (i >= 1)
			data[i - 1] |= (uint32_t)((tmp << 32) >> 32);
	}
	if (sign == '-')
		data[data.size() - num_of_blocks - 1] |= (((1ll << 32) - 1) << (32 - rhs));
	*this = from_compliment(*this);
	cut_zeros();
	return *this;
}

big_integer big_integer::operator+() const
{
	big_integer tmp = *this;
	return tmp;
}

big_integer big_integer::operator-() const
{
	big_integer tmp = *this;
	if (!tmp.is_zero())
		tmp.sign = (tmp.sign == '-' ? '+' : '-');
	return tmp;
}

big_integer big_integer::operator~() const
{
	return -*this - 1;
}

big_integer & big_integer::operator++()
{
	return (*this += 1);
}

big_integer big_integer::operator++(int)
{
	big_integer tmp = *this;
	++*this;
	return tmp;
}

big_integer & big_integer::operator--()
{
	return (*this -= 1);
}

big_integer big_integer::operator--(int)
{
	big_integer tmp = *this;
	--*this;
	return tmp;
}

bool operator==(big_integer const & a, big_integer const & b)
{	
	if (a.sign ^ b.sign)
		return false;
	if (a.data == b.data)
		return true;
	return false;
}

bool operator!=(big_integer const & a, big_integer const & b)
{
	return !(a == b);
}

bool operator<(big_integer const & a, big_integer const & b)
{
	if (a.sign ^ b.sign) {
		return a.sign == '-' ? true : false;
	}
	if (a.data.size() != b.data.size()) {
		if (a.sign == '+')
			return a.data.size() < b.data.size() ? true : false;
		else
			return a.data.size() > b.data.size() ? true : false;
	}
	for (ptrdiff_t i = a.data.size() - 1; i >= 0; --i) {
		if (a.data[i] != b.data[i]) {
			if (a.sign == '+')
				return a.data[i] < b.data[i] ? true : false;
			else
				return a.data[i] > b.data[i] ? true : false;
		}
	}
	return false;
}

bool operator>(big_integer const & a, big_integer const & b)
{
	return !(a < b) && (a != b);
}

bool operator<=(big_integer const & a, big_integer const & b)
{
	return !(a > b);
}

bool operator>=(big_integer const & a, big_integer const & b)
{
	return !(a < b);
}

std::string to_string(big_integer const & a)
{

	if (a == 0)
		return "0";
	std::string s = "";
	big_integer tmp = a;
	while (tmp != 0) {
		big_integer tmp2 = tmp % 10;
		char c = tmp2.data.back() + '0';
		tmp /= 10;
		s.push_back(c);
	}
	if (a.sign == '-')
		s.push_back('-');
	reverse(s.begin(), s.end());
	return s;
}

std::ostream & operator<<(std::ostream & s, big_integer const & a)
{
	return s << to_string(a);
}

big_integer apply(big_integer a, big_integer b, ops op) {
	while (a.data.size() != b.data.size()) {
		if (a.data.size() < b.data.size())
			a.data.push_back(0);
		else
			b.data.push_back(0);
	}
	a = to_compliment(a);
	b = to_compliment(b);
	for (int i = 0; i < a.data.size(); ++i) {
		switch (op)
		{
		case AND:
			a.data[i] = a.data[i] & b.data[i];
			break;
		case OR:
			a.data[i] = a.data[i] | b.data[i];
			break;
		case XOR:
			a.data[i] = a.data[i] ^ b.data[i];
			break;
		}
	}
	a = from_compliment(a);
	a.cut_zeros();
	return a;
}

big_integer to_compliment(big_integer a)
{
	if (a.sign == '+')
		return a;
	for (int i = 0; i < a.data.size(); ++i)
		a.data[i] = ~a.data[i];
	a.sign = '+';
	++a;
	a.sign = '-';
	return a;
}

big_integer from_compliment(big_integer a) {
	bool sign_bit = (uint64_t)a.data.back() & ((uint64_t)(1 << 31));
	if (sign_bit == 0)
		return a;
	a.sign = '+';
	--a;
	a.sign = '-';
	for (int i = 0; i < a.data.size(); ++i)
		a.data[i] = ~a.data[i];
	a.sign = '-';
	return a;
}


big_integer operator+(big_integer a, big_integer const & b)
{
	return a += b;
}

big_integer operator-(big_integer a, big_integer const & b)
{
	return a -= b;
}

big_integer operator*(big_integer a, big_integer const & b)
{
	return a *= b;
}

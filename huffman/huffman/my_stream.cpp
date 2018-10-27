#include "my_stream.h"

my_stream::my_stream(string input, string output)
{
	out.open(output, ios::binary);
	in.open(input, ios::binary);

	if (!out.is_open()) {
		this->close();
		throw FileNotOpenedException(output);
	}
	if (!in.is_open()) {
		this->close();
		throw FileNotOpenedException(input);
	}
	this->input = input;
	this->output = output;
}
my_stream::~my_stream() {
	if (in.is_open())
		in.close(); 
	if (out.is_open()) 
		out.close(); 
}

int my_stream::read_bytes(string & str, size_t count_bit)
{
	str.clear();
	char* buf = new char[(count_bit + 7) / 8];
	in.read(buf, (count_bit + 7) / 8);
	uint32_t cnt = (uint32_t)in.gcount();
	for (uint32_t i = 0; i < cnt; ++i) {
		for (int j = 0; j < 8; j++) 
		{
			int tmp = 1;
			tmp <<= (7 - j);
			if (count_bit > i * 8 + j)
				tmp &= buf[i];
			else
				break;
			tmp >>= (7 - j);

			str.push_back(tmp + '0');
		}
	}
	return cnt;
}

int my_stream::read_string(string & str, size_t size)
{
	str.clear();
	char* buf = new char[size];
	in.read(buf, size);
	uint32_t cnt = (uint32_t)in.gcount();
	for (uint32_t i = 0; i < cnt; ++i) {
		str.push_back(buf[i]); 
	}
	return cnt;
}
bool my_stream::read_uint32_t(uint32_t & x)
{
	x = 0;
	char* buf = new char[sizeof(uint32_t)];
	in.read(buf, sizeof(uint32_t));
	uint32_t cnt = (uint32_t)in.gcount();
	for (uint32_t i = 0; i < cnt; ++i) {
		x <<= sizeof(char) * 8;
		x += (unsigned char)buf[i];
	}
	return true;
}

void my_stream::write_bytes(string const & str, size_t count_bit)
{
	size_t size = (count_bit + 7) / 8;
	char* buf = new char[size];
	for (size_t i = 0; i < size; ++i) {
		buf[i] = 0;
		for (int j = 0; j < 8; ++j)
		{
			int tmp = 0;
			if (count_bit > i * 8 + j)
				tmp = (str[i * 8 + j] == '1') ? 1 : 0;
			tmp <<= (7 - j);
			buf[i] |= tmp;
		}
	}
	out.write(buf, size);
}
void my_stream::write_string(string const & str, size_t size)
{
	char* buf = new char[size];
	for (size_t i = 0; i < size; ++i) {
		buf[i] = str[i];
	}
	out.write(buf, size);
}
void my_stream::write_uint32_t(uint32_t const & x)
{
	uint32_t tmp = x;
	uint32_t cnt = sizeof(uint32_t);
	char* buf = new char[cnt];
	int ind = 0;
	for (int i = (int)(cnt - sizeof(char)) ; i >= 0; i -= (sizeof(char)))
	{
		char c = (tmp >> (i * 8));
		buf[ind++] = c;
	}
	out.write(buf, sizeof(uint32_t));
}


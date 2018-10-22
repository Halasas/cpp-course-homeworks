#include "my_stream.h"

my_stream::my_stream(string input, string output)
{
	this->input = input;
	this->output = output;
	in.open(input, ios::binary);
	out.open(output, ios::binary);
	if (!in.is_open())
		throw FileNotOpenedException(input);
	if (!out.is_open())
		throw FileNotOpenedException(output);
}
my_stream::~my_stream() { if (in.is_open()) in.close(); if (out.is_open()) out.close(); }

int my_stream::read_bytes(string & str, int size)
{
	str.clear();
	char* buf = new char[size];
	in.read(buf, size);
	int cnt = in.gcount();
	for (int i = 0; i < cnt; ++i) {
		for (int j = 0; j < 8; j++) {
			str.push_back('0' + (uint8_t)buf[i] % 2);
			buf[i] = (uint8_t)buf[i] / 2;
		}
	}
	return cnt;
}
int my_stream::read_string(string & str, int size)
{
	str.clear();
	char* buf = new char[size];
	in.read(buf, size);
	int cnt = in.gcount();
	for (int i = 0; i < cnt; ++i) {
		str.push_back(buf[i]); 
	}
	return cnt;
}
bool my_stream::read_uint32_t(uint32_t & x)
{
	x = 0;
	char* buf = new char[sizeof(uint32_t)];
	in.read(buf, sizeof(uint32_t));
	int cnt = in.gcount();
	for (int i = 0; i < cnt; ++i) {
		x <<= sizeof(char) * 8;
		x += (unsigned char)buf[i];
	}
	return true;
}

void my_stream::write_bytes(string const & str, int size)
{
	char* buf = new char[size];
	for (int i = 0; i < size; ++i) {
		buf[i] = 0;
		for (int j = 0; j < 8; ++j)
		{
			buf[i] <<= 1; 	
			buf[i] += (str[i*8 + j] == '1') ? 1 : 0;
		}
	}
	out.write(buf, size);
}
void my_stream::write_string(string const & str, int size)
{
	char* buf = new char[size];
	for (int i = 0; i < size; ++i) {
		buf[i] = str[i];
	}
	out.write(buf, size);
}
void my_stream::write_uint32_t(uint32_t const & x)
{
	uint32_t tmp = x;
	int cnt = sizeof(uint32_t);
	char* buf = new char[cnt];
	int ind = 0;
	for (int i = cnt - sizeof(char) ; i >= 0; i -= (sizeof(char))) 
	{
		buf[ind++] = (tmp >> (i * 8));
	}
	out.write(buf, sizeof(uint32_t));
}


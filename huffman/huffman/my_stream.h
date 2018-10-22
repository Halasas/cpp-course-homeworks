#ifndef STREAM_H
#define STREAM_H

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

struct FileNotOpenedException{
private:
	string er;
public:
	FileNotOpenedException(string filename) : er(filename + "cant be opened!") {}
	const char* error() 
	{
		char* msg = new char(er.size());
		for (ptrdiff_t i = 0; i < er.size(); ++i)
			msg[i] = er[i];
		return msg;
	}
};

struct FileIsDamagedException {
private:
	string er;
public:
	FileIsDamagedException(string filename) : er(filename + "is damaged!") {}
	const char* error()
	{
		char* msg = new char(er.size());
		for (ptrdiff_t i = 0; i < er.size(); ++i)
			msg[i] = er[i];
		return msg;
	}
};

class my_stream
{
public:
	my_stream() {};
	my_stream(string input, string output);
	~my_stream();

	void close() { in.close(); out.close(); };
//read
	int read_bytes(string &str, int size);
	int read_string(string &str, int size);
	bool read_uint32_t(uint32_t &x);
//write
	void write_bytes(string const &str, int const size);
	void write_string(string const &str, int const size);
	void write_uint32_t(uint32_t const &x);
private:
	string input, output;
	ifstream in;
	ofstream out;
};

#endif // !STREAMER

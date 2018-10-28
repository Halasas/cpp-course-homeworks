#ifndef MY_STREAM_H
#define MY_STREAM_H

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
		char* msg = new char[er.size()];
		for (uint32_t i = 0; i < er.size(); ++i)
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
		char* msg = new char[er.size()];
		for (uint32_t i = 0; i < er.size(); ++i)
			msg[i] = er[i];
		return msg;
	}
};

class my_stream
{
private:
	string input, output;
	ifstream in;
	ofstream out;

public:
	my_stream() {};
	my_stream(string input, string output);
	~my_stream();

	bool in_eof() { return in.eof(); }
	void close() { in.close(); out.close(); };
	
	void seekg(uint32_t x) { 
		in.clear();
		in.seekg(0, std::ios_base::beg); 
	};
	
	int gcount() { return (int)in.gcount(); };
//read
	int read_bytes(string &str, size_t size);
	int read_string(string &str, size_t size);
	bool read_uint32_t(uint32_t &x);
//write
	void write_bytes(string const &str, size_t const size);
	void write_string(string const &str, size_t const size);
	void write_uint32_t(uint32_t const &x);

};

#endif // !MY_STREAM

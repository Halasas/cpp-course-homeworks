#ifndef HUFFMAN_INTERFACE
#define HUFFMAN_INTERFACE

#include "huff_lib/decoder.h"
#include "huff_lib/encoder.h"


namespace huf_interface {
	void decode(std::string const &file1, std::string const &file2);
	void encode(std::string const &file1, std::string const &file2);
}

#endif // !HUFFMAN_INTERFACE

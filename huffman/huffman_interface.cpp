#include "huffman_interface.h"

void huf_interface::decode(std::string const & file1, std::string const & file2)
{
	decoder d;
	d.decode(file1, file2);
}

void huf_interface::encode(std::string const & file1, std::string const & file2)
{
	encoder e;
	e.encode(file1, file2);
}

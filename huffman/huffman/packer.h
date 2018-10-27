#ifndef PACKER_H
#define PACKER_H

#include <iostream>
#include <string>
#include "huffman_tree.h"
#include "my_stream.h"

namespace packer
{
	void pack(string in, string out);
	void unpack(string in, string out);
}

#endif //PACKER_H
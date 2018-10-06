#include "huffman_lib.h"

node::node() : left_son(NULL), right_son(NULL), parent(NULL) {}
node::node(uint32_t weight, char value) : value(value), weight(weight), left_son(NULL), right_son(NULL) {}
node::node(node *parent, uint32_t weight, char c) : parent(parent), value(c), weight(weight),
											    left_son(NULL), right_son(NULL) {}
node::node(node * parent) : parent(parent), left_son(NULL), right_son(NULL) {}
node::node(uint32_t weight, node *left_son, node *right_son) : weight(weight), left_son(left_son), right_son(right_son) {}
node::~node(){if (left_son != NULL) delete left_son; if (right_son != NULL) delete right_son;}

bool chs_ops::val_of_bit(char const *array, size_t const ind) {
	return static_cast<bool>(
		(array[ind / (sizeof(char) * 8)] >> ((sizeof(char) * 8 - 1) - ind % (sizeof(char) * 8)))
		& 1);
}
void chs_ops::set_0(char *array, size_t ind) {
	array[ind / (sizeof(char) * 8)] &= ~(1 << (sizeof(char) * 8 - 1 - (ind % (sizeof(char) * 8))));
}

void chs_ops::set_1(char *array, size_t ind) {
	array[ind / (sizeof(char) * 8)] |= 1 << (sizeof(char) * 8 - 1 - (ind % (sizeof(char) * 8)));
}


OpenFileException::OpenFileException() = default;
DamagedFileException::DamagedFileException() = default;


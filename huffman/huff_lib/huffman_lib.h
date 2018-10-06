#ifndef HUFFMAN_LIB_H
#define HUFFMAN_LIB_H

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <queue>  

struct node {
	node();
	node(uint32_t w, char c);
	node(node *parent, uint32_t w, char c);
	node(node *parent);
	node(uint32_t w, node *l, node *r);
	~node();
	uint32_t weight;
	char value;
	node* left_son;
	node* right_son;
	node* parent;
};

namespace chs_ops {
	bool val_of_bit(char const *array, size_t const ind);
	void set_0(char* array, size_t ind);
	void set_1(char* array, size_t ind);
}


uint32_t const BUF_SIZE = 1 << 16;
uint32_t const ALP_SIZE = 1 << 8;

struct OpenFileException {OpenFileException();};
struct DamagedFileException {DamagedFileException();};

#endif //HUFFMAN_LIB_H
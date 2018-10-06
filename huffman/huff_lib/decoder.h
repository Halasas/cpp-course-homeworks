//
// Created by Halasas 03.10.18
//
#ifndef DECODER_H
#define DECODER_H

#include "huffman_lib.h"

class decoder {
public:
	void decode(std::string input, std::string output);

private:
	std::ifstream in;
	std::ofstream out;
	int64_t count_bits;
	uint64_t count_chrs;

	node* tree;
	template <typename T>
	void read_to(T &type);
	template <typename T>
	T to_T(char const *array, size_t const size);

	void FileIsDamaged();
	void make_tree(char const *desc_of_tree, size_t const count_bits);
	void dfs(node* cur);
};

#endif // !DECODER_H



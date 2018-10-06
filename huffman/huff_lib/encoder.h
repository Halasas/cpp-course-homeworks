#ifndef ENCODER_H
#define ENCODER_H

#include "huffman_lib.h"

class encoder {
public:
	void encode(std::string input, std::string output);
private:
	void to_count_chrs();
	uint64_t to_count_bits();
	template <typename T>
	char* to_char_array(T number);
	void make_tree();
	void dfs(node * cur, std::vector<bool> &code, uint16_t & ind);

	size_t write_to_buf(std::vector<bool> const &code, size_t code_start, char* buffer, size_t &ind, const size_t BIT_SIZE);

	uint64_t alp[ALP_SIZE];
	uint64_t count_chrs;
	uint64_t count_bits;

	node* tree;
	std::vector<char> desc_of_tree;
	std::vector<char> order_of_lists;
	std::vector<bool> codes[ALP_SIZE];

	std::ifstream in;
	std::ofstream out;
};

#endif // !ENCODER_H


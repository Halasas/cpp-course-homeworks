#include "decoder.h"

void decoder::decode(std::string input, std::string output) {
	in.open(input, std::ios::binary);
	out.open(output, std::ios::binary);
	if (!in.is_open()) {
		in.close();
		out.close();
		throw OpenFileException();
	}
	if (!out.is_open()) {
		in.close();
		out.close();
		throw OpenFileException();
	}
	read_to(count_chrs);
	if (count_chrs == 0)
	{
		in.close();
		out.close();
		return;
	}
	read_to(count_bits);
	if (count_chrs == 1)
		++count_chrs;
	size_t desc_size = (2 * count_chrs - 1 - 1 + 7) / 8;
	char* desc_of_tree = new char[desc_size]; //description of tree
	in.read(desc_of_tree, desc_size);
	if (in.gcount() < desc_size)
		FileIsDamaged();
	tree = new node();
	make_tree(desc_of_tree, 2 * count_chrs - 2);
	dfs(tree);
	size_t ind_buf = 0;
	char* buf_in = new char[1 << 16];
	char* buf_out = new char[1 << 16];


	node* last_node = NULL;
	while (count_bits > 0) {
		if (in.eof()) {
			delete[] buf_in;
			delete[] buf_out;
			delete tree;
			FileIsDamaged();
		}
		in.read(buf_in, BUF_SIZE);
		uint64_t count_bytes = in.gcount();
		size_t last_bit = std::min((uint64_t)count_bits, (uint64_t)count_bytes * 8);
		count_bits -= count_bytes * 8;

		size_t i = 0;
		while (i < last_bit) {
			node* curr_node;
			if (last_node != NULL) {
				curr_node = last_node;
				last_node = NULL;
			}
			else 
				curr_node = tree;
			while (curr_node->left_son != NULL) {
				if (i == last_bit) {
					last_node = curr_node;
					break;
				}
				bool val = chs_ops::val_of_bit(buf_in, i++);
				if (val)
					curr_node = curr_node->left_son;
				else
					curr_node = curr_node->right_son;
			}
			if (last_node != NULL) { break; }
			buf_out[ind_buf++] = curr_node->value;
			if (ind_buf == BUF_SIZE) {
				out.write(buf_out, BUF_SIZE);
				ind_buf = 0;
			}
		}
	}
	if (ind_buf > 0)
		out.write(buf_out, ind_buf);
	delete[] buf_in;
	delete[] buf_out;
	delete tree;

	in.close();
	out.close();
}


void decoder::FileIsDamaged() {
	in.close();
	out.close();
	throw DamagedFileException();
}

template<typename T> void decoder::read_to(T &type) {
	size_t type_ch_size = (sizeof(type) + sizeof(char) - 1)
		/ (sizeof(char)); //count number of chars need to read type to char*
	char* chars = new char[type_ch_size];
	in.read(chars, sizeof(type));
	if (type_ch_size > in.gcount()) {
		delete[] chars;
		FileIsDamaged();
	}
	type = to_T<T>(chars, type_ch_size);
	delete[] chars;
}
template<typename T>
T decoder::to_T(char const * array, size_t const size)
{
	T number = 0;
	size_t step = (sizeof(T) - sizeof(char)) * 8;
	for (size_t i = 0; i < size; ++i) {
		number |= (T)(unsigned char)array[i] << step;
		step -= sizeof(char) * 8;
	}
	return number;
	return T();
}


void decoder::make_tree(char const *desc_of_tree, size_t const count_bits) {
	node *cur = tree;
	for (size_t i = 0; i < count_bits; ++i) {
		if (chs_ops::val_of_bit(desc_of_tree, i)) {
			node *left_son = new node(cur);
			cur->left_son = left_son;
			cur = left_son;
		}
		else {
			do {
				cur = cur->parent;
			} while (cur->parent != NULL && cur->right_son != NULL);
			node *right_son = new node(cur);
			cur->right_son = right_son;
			cur = right_son;
		}
	}
}

void decoder::dfs(node * cur) {
	if ((cur->left_son != NULL) ^ (cur->right_son != NULL)) {
		in.close();
		out.close();
		throw DamagedFileException();
	}
	if (cur->left_son == NULL) {
		char c;
		in.read(&c, 1);
		cur->value = (unsigned char)c;
		return;
	}
	dfs(cur->left_son);
	dfs(cur->right_son);
}


#include "encoder.h"

void encoder::encode(std::string input, std::string output) {
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
	to_count_chrs();
	char* chr_count_chrs = to_char_array(count_chrs);
	out.write(chr_count_chrs, sizeof(count_chrs));
	if (!count_chrs) { 
		in.close();
		out.close();
		return;
	}
	if (count_chrs == 1)
		++count_chrs;
	
	desc_of_tree.resize((count_chrs * 2 - 2 + 7) / 8);
	make_tree();
	
	std::vector<bool> tmp_code;
	uint16_t ind = 0;
	dfs(tree, tmp_code, ind); 
	
	count_bits = to_count_bits();
	char* chr_count_bits =  to_char_array(count_bits);
	out.write(chr_count_bits,sizeof(count_bits));
	
	out.write(&desc_of_tree[0], desc_of_tree.size());
	out.write(&order_of_lists[0], order_of_lists.size());

	in.clear();
	in.seekg(0, std::ios_base::beg);

	char *buf_in = new char[BUF_SIZE]; 
	char *buf_out = new char[BUF_SIZE]; 
	size_t ind_in_buf = 0;
	while (!in.eof()) {
		in.read(buf_in, BUF_SIZE);
		uint32_t last_byt = (uint32_t)in.gcount();

		for (size_t i = 0; i < last_byt; ++i) {
			size_t remainder = write_to_buf(codes[(unsigned char)buf_in[i]], 0, buf_out, ind_in_buf, BUF_SIZE * sizeof(char) * 8);
			if (ind_in_buf == BUF_SIZE * sizeof(char) * 8) {
				out.write(buf_out, BUF_SIZE * sizeof(char));
				ind_in_buf = 0;
				write_to_buf(codes[(unsigned char)buf_in[i]], codes[(unsigned char)buf_in[i]].size() - remainder, buf_out, ind_in_buf, remainder);
			}
		}
	}
	if (ind_in_buf > 0)
		out.write(buf_out, sizeof(char) * ((ind_in_buf + 7) / 8));
	delete[] buf_in;
	delete[] buf_out;
	delete tree;
	in.close();
	out.close();
}

void encoder::to_count_chrs() {
	count_chrs = 0;
	char *buf = new char[BUF_SIZE];
	while (!in.eof()) {
		in.read(buf, BUF_SIZE);
		auto last_bit = in.gcount();
		for (size_t i = 0; i < last_bit; ++i) {
			if (alp[(unsigned char)buf[i]] == 0) 
				++count_chrs;
			++alp[(unsigned char)buf[i]];
		}
	}
	delete[] buf; 
}

template <typename T>
char* encoder::to_char_array(T number) {
	size_t size = (sizeof(T) + (sizeof(char) - 1)) / sizeof(char);
	size_t step = (sizeof(T) - sizeof(char)) * 8;
	char* chrs = new char[size];
	for (size_t i = 0; i < size; ++i) {
		chrs[i] = 0;
		chrs[i] |= number >> step;
		step -= sizeof(char) * 8;
	}
	return chrs;
}

size_t encoder::write_to_buf(std::vector<bool> const &code, size_t ind_code, char *buf_out, size_t &ind, const size_t BIT_SIZE) {
	size_t reminder = 0;
	for (size_t i = ind_code; i < code.size(); ++i) {
		if (ind == BIT_SIZE) {
			reminder = code.size() - i;
			break;
		}
		if (code[i])
			chs_ops::set_1(buf_out, ind);
		else
			chs_ops::set_0(buf_out, ind);
		++ind;
	}
	return reminder;
}

void encoder::make_tree()
{
	std::priority_queue<std::pair<int, node*>> tmp;
	for (size_t i = 0; i < ALP_SIZE; ++i)
		if (alp[i] != 0)
			tmp.push({ -(int64_t)alp[i], new node(alp[i], (char)i) });
	if (tmp.size() == 1) {
		tree = new node();
		node *fake = new node('\0', UINT32_MAX);
		tree->left_son = tmp.top().second; tmp.pop();
		tree->right_son = fake;
		return;
	}
	while (tmp.size() >= 2) {
		node* l = tmp.top().second; tmp.pop();
		node* r = tmp.top().second; tmp.pop();
		node* p = new node((uint32_t)(l->weight + r->weight), l, r);
		tmp.push({ -(int)p->weight, p });
	}
	tree = tmp.top().second;
	tmp.pop();
}

void encoder::dfs(node* cur, std::vector<bool> &code, uint16_t &ind) {
	if (cur->left_son != NULL) {
		chs_ops::set_1(&desc_of_tree[0], ind++);
		code.push_back(true);
		dfs(cur->left_son, code, ind);

		chs_ops::set_0(&desc_of_tree[0], ind++);
		code.push_back(false);
		dfs(cur->right_son, code, ind);
		if(!code.empty())
			code.pop_back();
		return;
	}
	order_of_lists.push_back(cur->value);
	codes[(unsigned char)cur->value] = code;
	code.pop_back();
}

uint64_t encoder::to_count_bits() { 
	uint64_t count = 0;
	for (size_t i = 0; i < ALP_SIZE; ++i)
		count += alp[i] * codes[i].size();
	return count;
}


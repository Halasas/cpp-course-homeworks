#include "packer.h"

using namespace std;

void packer::pack(string in, string out)
{
	try {
	my_stream stream(in, out);
	
	bool flag = false;
	vector<int> alp(256, 0);
	while (!stream.in_eof()) 
	{
		string buf;
		stream.read_string(buf, 4096);
		for (char c : buf) 
		{
			alp[(unsigned char)c]++;
			flag = true;
		}
	}
	if (!flag)
		return;
	huffman_tree huff(alp);

	vector<string> codes = huff.get_codes();
	
	uint32_t bit_counter = 0;
	uint32_t symb_counter = 0;

	for (size_t i = 0; i < 256; i++)
		bit_counter += (uint32_t)codes[i].size() * alp[i];
	
	string desc = huff.get_desc();
	string order = huff.get_order_symbols();

	stream.write_uint32_t((uint32_t)order.size());
	stream.write_uint32_t(bit_counter);
	
	stream.write_bytes(desc, desc.size());
	stream.write_string(order, order.size());

	string buf_out;
	stream.seekg(0);
	while (!stream.in_eof()) 
	{
		string buf_in;
		stream.read_string(buf_in, 4096);
		
		uint32_t buf_index = 0;
		uint32_t code_index = 0;
		while(true) 
		{
			string const * code = &codes[(unsigned char)buf_in[buf_index]];
			
			if (code_index >= code->size()) 
			{
				code_index = 0;
				++buf_index;
				
				if (buf_index == buf_in.size())
					break;

				code = &codes[(unsigned char)buf_in[buf_index]];
			}
			buf_out.push_back(code->operator[](code_index++));

			if (buf_out.size() == 8 * 4096)
				stream.write_bytes(buf_out, buf_out.size()), buf_out.resize(0);
		}
		if (buf_out.size() % 8 == 0)
			stream.write_bytes(buf_out, buf_out.size()), buf_out.resize(0);
	}
	stream.write_bytes(buf_out, buf_out.size()), buf_out.resize(0);
	stream.close();
	}
	catch (FileNotOpenedException e) {
		cout << e.error();
		return;
	}
}


void packer::unpack(string in, string out) {
	try {
		my_stream stream(in, out);

	uint32_t num_of_bit;
	uint32_t num_of_symbols;

	stream.read_uint32_t(num_of_symbols);
	stream.read_uint32_t(num_of_bit);

	string desc;
	string order;

	if (num_of_symbols == 1)
		++num_of_symbols;

	stream.read_bytes(desc, (num_of_symbols - 1) * 2);
	stream.read_string(order, num_of_symbols);


	huffman_tree huf(desc, order);
	Node* root = huf.get_tree();
	Node* cur = root;

	string buf_out;
	uint32_t counter = 0;
	while (num_of_bit > counter) {
		string in_buf;
		stream.read_bytes(in_buf, 4096);

		if (in_buf.size() == 0)
		{
			throw FileIsDamagedException(in);
		}

		for (char c : in_buf) {
			if (num_of_bit <= counter)
				break;
			++counter;
			if (c == '0')
				cur = cur->left_son;
			else if (c == '1')
				cur = cur->right_son;
			else
				throw FileIsDamagedException(in);
			if (cur->is_leaf) {
				buf_out.push_back(cur->symbol);
				cur = root;
				if (buf_out.size() == 4096)
					stream.write_string(buf_out, buf_out.size()), buf_out.resize(0);
			}
		}
		if (cur == root)
			stream.write_string(buf_out, buf_out.size()), buf_out.resize(0);
	}
	stream.write_string(buf_out, buf_out.size()), buf_out.resize(0);
	stream.close();

	}
	catch (FileNotOpenedException e) {
		cout << e.error();
		return;
	}
	catch (FileIsDamagedException e) {
		cout << e.error();
		return;
	}
}
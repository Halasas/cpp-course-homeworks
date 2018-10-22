#include "huffman_tree.h"

huffman_tree::huffman_tree() : tree(nullptr) { codes.resize(256); };

huffman_tree::huffman_tree(vector<int>& alp)
{
	codes.resize(256);
	priority_queue<pair<int, Node*>> pq;
	for (ptrdiff_t i = 0; i < 256; i++)
		if (alp[i] != 0)
			pq.push({ -alp[i], new Node(char(i),alp[i]) });
	if (pq.size() == 1) {
		Node* l = pq.top().second; 	pq.pop();
		Node* r = new Node();
		tree = new Node(l, r);
	}
	else {
		while (pq.size() >= 2) {
			Node* l = pq.top().second; 	pq.pop();
			Node* r = pq.top().second;	pq.pop();
			Node* tmp = new Node(l, r);
			pq.push({ -(int32_t)tmp->weight, tmp });
		}
		tree = pq.top().second; pq.pop();
	}
	string code;
	dfs_for_codes(tree, code);
}

huffman_tree::huffman_tree(string & desc, string & order)
{
	Node* cur = new Node();
	tree = cur;
	for (char c : desc) {
		if (c == '0') {
			cur->left_son = new Node(cur);
			cur = cur->left_son;
		}
		else {
			cur = cur->parent;
			while (cur->right_son != NULL)
				cur = cur->parent;
			cur->right_son = new Node(cur);
			cur = cur->right_son;
		}
	}
	int ind = 0;
	dfs_for_leafs(tree, order, ind);
}

huffman_tree::~huffman_tree()
{
	delete this->tree;
}

void huffman_tree::dfs_for_codes(Node* cur, string &code)
{
	if (!cur->is_leaf) {
		desc.push_back('0');
		code.push_back('0');
		dfs_for_codes(cur->left_son, code);
		code.pop_back();

		desc.push_back('1');
		code.push_back('1');
		dfs_for_codes(cur->right_son, code);
		code.pop_back();
	}
	else {
		order_symbols.push_back(cur->symbol);
		codes[(unsigned char)cur->symbol] = code;
	}
}

void huffman_tree::dfs_for_leafs(Node* cur, string &order, int &ind)
{
	if (cur->is_leaf) {
		cur->symbol = order[ind++]; 
		return;
	}
	dfs_for_leafs(cur->left_son, order, ind);
	dfs_for_leafs(cur->right_son, order, ind);
}

string get_desc(huffman_tree & huf_tree) {	return huf_tree.desc;	}
string get_order_symbols(huffman_tree & huf_tree)
{	return huf_tree.order_symbols;	}
vector<string> get_codes(huffman_tree & huf_tree) {	return huf_tree.codes;	}
Node * get_tree(huffman_tree & huf_tree) {	return huf_tree.tree;	}

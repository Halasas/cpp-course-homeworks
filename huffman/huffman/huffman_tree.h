#ifndef HUFFMAN_TREE
#define HUFFMAN_TREE

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

struct Node
{
	Node* left_son;
	Node* right_son;
	Node* parent;
	char symbol;
	uint32_t weight;
	bool is_leaf;
	Node() :left_son(nullptr), right_son(nullptr), parent(nullptr), symbol(0), weight(0), is_leaf(true) {};
	Node(char symbol, uint32_t weight) :left_son(nullptr), right_son(nullptr), parent(nullptr), symbol(symbol), weight(weight), is_leaf(true) {};
	Node(Node* &parent) : left_son(nullptr), right_son(nullptr), parent(parent), symbol(0), weight(0), is_leaf(true) { parent->is_leaf = false; };
	Node(Node* &left_son, Node* &right_son) : left_son(left_son), right_son(right_son), parent(nullptr), symbol(0),
		weight(left_son->weight + right_son->weight), is_leaf(false) {	left_son->parent = this; right_son->parent = this;	};
};

class huffman_tree
{
public:
	huffman_tree();
	huffman_tree(vector<int> &alp);
	huffman_tree(string &desc, string &order);
	~huffman_tree();
	
	friend string get_desc(huffman_tree &tree);
	friend string get_order_symbols(huffman_tree &tree);
	friend vector<string> get_codes(huffman_tree &huf_tree);
	friend Node* get_tree(huffman_tree &huf_tree);

private:
	void dfs_for_codes(Node* cur, string &code);
	void dfs_for_leafs(Node* cur, string &order, int &ind);
	Node* tree;
	string desc;
	string order_symbols;
	vector<string> codes;
};
string get_desc(huffman_tree &huf_tree);
string get_order_symbols(huffman_tree &huf_tree);
vector<string> get_codes(huffman_tree &huf_tree);
Node* get_tree(huffman_tree &huf_tree);
#endif // !HUFFMAN_TREE

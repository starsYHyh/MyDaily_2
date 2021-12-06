#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>



using namespace std;

/*
存放字符的初始编码信息
*/
struct Info
{
	char ch;
	string label;
	Info(char ch, string label)
	{
		this->ch = ch;
		this->label = label;
	}

};

/*
存放节点信息
*/
struct Node {

	int weight;	//权重
	int num;	//编号
	char ch;	//字符
	Node* lch;	//左孩子
	Node* rch;	//右孩子
	Node* nParent;	//父节点

	Node(Node* p_left, Node* p_right, Node* p_parent)
	{
		ch = NULL;
		weight = 1;
		num = 0;
		this->lch = p_left;
		this->rch = p_right;
		this->nParent = p_parent;
	}
};

/*
二叉树信息
*/
class BinaryTree
{
private:
	Node* bRoot;
public:

	enum Brother { LeftChild, RightChild };
	BinaryTree(int num = 0, int weight = 0);
	~BinaryTree();
	bool swapNode(Node* nodeA, Node* nodeB);
	bool addNode(Node* parent, Node* child, Brother brotherState);
	Node* findNode(Node* p);
	Node* findNode(string chain);
	void deleteNode(Node* node);
	//获得父节点信息
	Node* getRoot() { return bRoot; }
	//设置节点编号
	bool setNodeNum(Node* node, int num);
	//判断是左节点还是右节点
	Brother getBrotherState(Node* node);
	//判断后面一个参数是否为前面一个参数的祖先
	bool isAncestor(Node* nodeChild, Node* nodeAncestor);


};


/*
哈夫曼树信息
*/
class HuffmanTree
{
public:

	HuffmanTree();
	~HuffmanTree();
	bool ReadFile(string filename);
	int Height(Node* root);

	//编码
	bool encode(char* out_filename);
	//解码
	bool decode(string code,string out_filename);
	//vector<char> getMap() { return map; }

private:
	//增加权重
	void weightAdd(Node* p_node);

	//总权值
	static int sum;
	BinaryTree tree;

	//存储已存在字符的哈夫曼编码的结构
	struct charMap {
		char ch;	//字符
		string wayCode;	//路径编码
		Node* p;
	};
	std::string getHuffmanCode(Node* p);
	Node* findLarge(Node*);

	//一个存储哪些字符已经存在于树中的缓冲
	vector<charMap> buffers;

	//存储已经解码的字符
	vector<char> map;
	//输入输出流
	ifstream is;
	ofstream os;
};
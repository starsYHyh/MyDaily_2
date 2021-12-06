#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>



using namespace std;

/*
����ַ��ĳ�ʼ������Ϣ
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
��Žڵ���Ϣ
*/
struct Node {

	int weight;	//Ȩ��
	int num;	//���
	char ch;	//�ַ�
	Node* lch;	//����
	Node* rch;	//�Һ���
	Node* nParent;	//���ڵ�

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
��������Ϣ
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
	//��ø��ڵ���Ϣ
	Node* getRoot() { return bRoot; }
	//���ýڵ���
	bool setNodeNum(Node* node, int num);
	//�ж�����ڵ㻹���ҽڵ�
	Brother getBrotherState(Node* node);
	//�жϺ���һ�������Ƿ�Ϊǰ��һ������������
	bool isAncestor(Node* nodeChild, Node* nodeAncestor);


};


/*
����������Ϣ
*/
class HuffmanTree
{
public:

	HuffmanTree();
	~HuffmanTree();
	bool ReadFile(string filename);
	int Height(Node* root);

	//����
	bool encode(char* out_filename);
	//����
	bool decode(string code,string out_filename);
	//vector<char> getMap() { return map; }

private:
	//����Ȩ��
	void weightAdd(Node* p_node);

	//��Ȩֵ
	static int sum;
	BinaryTree tree;

	//�洢�Ѵ����ַ��Ĺ���������Ľṹ
	struct charMap {
		char ch;	//�ַ�
		string wayCode;	//·������
		Node* p;
	};
	std::string getHuffmanCode(Node* p);
	Node* findLarge(Node*);

	//һ���洢��Щ�ַ��Ѿ����������еĻ���
	vector<charMap> buffers;

	//�洢�Ѿ�������ַ�
	vector<char> map;
	//���������
	ifstream is;
	ofstream os;
};
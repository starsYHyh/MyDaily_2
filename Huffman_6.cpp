#include "HFM.h"
#include <cstring>

//allInfo存放所有字符的初始编码信息。
vector<Info> allInfo;
//sum为总权重
int HuffmanTree::sum = 1;
//allRoot为树的根节点
Node* allRoot;
//labelLen为初始编码的长度。
int labelLen;
//preIndex前一段识别的编码段的类型
//1为新节点的编码
//2为new节点的编码
//3为重复节点的编码
//初始时默认为0，对应着第一个编码必为新建节点
unsigned int preIndex = 0;

//获取所有字符的信息，包括字符及其对应的初始编码
void getLabls(string fileName, string labelsName)
{
	ifstream f1(fileName);
	ifstream f2(labelsName);
	ofstream o1("ouput1.txt");
	if (f1 && f2)
	{
		string tchstr;
		char tch;
		string tlabel;
		while (getline(f1, tchstr) && getline(f2, tlabel))
		{
			tch = tchstr[0];
			Info info = Info(tch, tlabel);
			allInfo.push_back(info);
			o1 << tch + tlabel << endl;
		}

	}

}

BinaryTree::BinaryTree(int num, int weight)
{
	bRoot = new Node(nullptr, nullptr, nullptr);
	bRoot->num = num;
	bRoot->weight = weight;
}

BinaryTree::~BinaryTree()
{
	deleteNode(bRoot);
}

//交换节点
bool BinaryTree::swapNode(Node* nodeA, Node* nodeB)
{

	//如果A节点或B节点为空或两个节点相同，则直接返回false。
	if (nodeA == nullptr || nodeB == nullptr || nodeA == nodeB)
	{
		return false;
	}

	Node* pTemp;
	//这里有四种情况，

	if (getBrotherState(nodeA) == LeftChild) {
		if (getBrotherState(nodeB) == LeftChild) {
			//当A为左节点，B也为左节点时：
			pTemp = nodeA->nParent->lch;
			nodeA->nParent->lch = nodeB->nParent->lch;
			nodeB->nParent->lch = pTemp;
		}
		else {
			//当A为左节点，B为右节点时：
			pTemp = nodeA->nParent->lch;
			nodeA->nParent->lch = nodeB->nParent->rch;
			nodeB->nParent->rch = pTemp;
		}
	}
	else {
		if (getBrotherState(nodeB) == LeftChild) {
			//当A为右节点，B为左节点时：
			pTemp = nodeA->nParent->rch;
			nodeA->nParent->rch = nodeB->nParent->lch;
			nodeB->nParent->lch = pTemp;
		}
		else {
			//当A为右节点，B也为右节点时：
			pTemp = nodeA->nParent->rch;
			nodeA->nParent->rch = nodeB->nParent->rch;
			nodeB->nParent->rch = pTemp;

		}
	}
	pTemp = nodeA->nParent;
	nodeA->nParent = nodeB->nParent;
	nodeB->nParent = pTemp;
	//交换完毕返回true。
	return true;

}

//增加节点
bool BinaryTree::addNode(Node* p_parent, Node* p_child, Brother brotherState)
{
	//如果孩子或父节点有一个为空指针，则直接返回false：
	if (p_parent == nullptr || p_child == nullptr)
	{
		return false;
	}
	if (brotherState == LeftChild) {
		//当要添加的节点为左节点时：
		//如果将要添加到的父节点的左孩子已经存在，则不能进行添加，直接返回false
		if (p_parent->lch != nullptr) {
			cout << "左孩子不存在！" << endl;
			return false;
		}
		p_parent->lch = p_child;
	}
	else if (brotherState == RightChild) {
		//同理
		if (p_parent->rch != nullptr) {
			cout << "右孩子不存在！" << endl;
			return false;
		}
		p_parent->rch = p_child;
	}
	else {
		cout << "节点位置有误！" << endl;
		return false;
	}
	p_child->nParent = p_parent;
	//添加完成之后返回true：
	return true;
}

//直接根据指针找节点，用于编码的过程中：
Node* BinaryTree::findNode(Node* p)
{
	//用队列来寻找指定节点
	//深度优先
	Node* p_node = allRoot;
	queue<Node*> queue;
	queue.push(p_node);
	while (!queue.empty()) {
		p_node = queue.front();
		if (p_node == p) {
			//如果当前的节点为所需的节点，则返回
			return p_node;
		}
		queue.pop();
		if (p_node->lch != nullptr) {
			queue.push(p_node->lch);
		}
		if (p_node->rch != nullptr) {
			queue.push(p_node->rch);
		}
	}
	//如果没找到，则直接返回Nullptr
	return nullptr;
}

//根据路径信息来寻找节点，用于解码的过程中：
Node* BinaryTree::findNode(string way)
{
	string tempStr = way;
	Node* p = allRoot;
	for (int i = 0; i < tempStr.length(); i++)
	{
		//当当前的路径字符信息为0时，表示的是找其左节点
		if (tempStr[i] == '0')
		{
			//如果p不为空指针，则当前节点变成其左节点
			if (p != nullptr)
			{
				p = p->lch;
			}
		}
		//同理
		else if (tempStr[i] == '1')
		{
			if (p != nullptr)
			{
				p = p->rch;
			}
		}
	}
	return p;
}

//设置节点编号
bool BinaryTree::setNodeNum(Node* p_node, int num)
{
	if (p_node == nullptr)
		return false;
	else {
		p_node->num = num;
		return true;
	}
}

//判断后面一个参数是否为前面一个参数的祖先
bool BinaryTree::isAncestor(Node* p_nodeChild, Node* p_nodeAncestor)
{

	while (p_nodeChild != bRoot) {
		//只要当前节点不为根节点
		if (p_nodeChild == p_nodeAncestor) {
			//如果当前节点为需要判断的祖先节点，返回true，表示后面一个参数是为前面一个参数的祖先
			return true;
		}
		else {
			p_nodeChild = p_nodeChild->nParent;
		}
	}
	return false;
}

//删除节点
void BinaryTree::deleteNode(Node* p_node)
{

	if (p_node->lch != nullptr) {
		deleteNode(p_node->lch);
	}
	if (p_node->rch != nullptr) {
		deleteNode(p_node->rch);
	}
	delete p_node;
}

//判断是左节点还是右节点
BinaryTree::Brother BinaryTree::getBrotherState(Node* p_node)
{
	if (p_node->nParent->lch == p_node) {
		return LeftChild;
	}
	else {
		return RightChild;
	}
}


HuffmanTree::HuffmanTree() :tree(0, 0)
{
}

HuffmanTree::~HuffmanTree()
{
	os.close();
	is.close();
}

//读取文件
bool HuffmanTree::ReadFile(string filename)
{
	is.open(filename, ios_base::in);
	if (!is.is_open()) {
		cout << "error: " << filename << " is not exist!" << endl;
		return false;
	}
	return true;
}

//获取节点的哈夫曼编码
string HuffmanTree::getHuffmanCode(Node* p_n)
{
	string huffmanCode = "";
	stack<Node*> stack;
	deque<char> code;

	//逆向后推，当为左孩子的时候则置0，当为右孩子的时候则置1。
	while (p_n != tree.getRoot()) {
		//只要当前节点不是根节点
		if (tree.getBrotherState(p_n) == tree.LeftChild) {
			code.push_back('0');
		}
		else {
			code.push_back('1');
		}
		p_n = p_n->nParent;
	}

	while (!code.empty()) {
		huffmanCode += code.back();
		code.pop_back();
	}
	return huffmanCode;
}

//找到所在块中最大节点编号的节点
Node* HuffmanTree::findLarge(Node* p_node)
{
	stack<Node*> stack;
	Node* p = tree.getRoot();
	Node* large = p;
	while (p || !stack.empty()) {
		if (p != nullptr) {
			stack.push(p);
			if (p->weight == p_node->weight) {
				//如果large不在同权重下，则置large为p
				if (large->weight != p->weight) {
					large = p;
				}
				//同权重下的large比p小，则置large为p
				else if (large->num > p->num) {
					large = p;
				}
			}
			p = p->lch;
		}
		else {
			p = stack.top();
			stack.pop();
			p = p->rch;
		}
	}
	//large不可能是根节点，当large为根节点时返回原节点
	if (large == tree.getRoot()) {
		return p_node;
	}
	return large;
}

//增加权重，并检验是否满足兄弟性质
void HuffmanTree::weightAdd(Node* p_node)
{
	while (p_node != nullptr) {
		Node* largeNode = findLarge(p_node);
		if (largeNode != p_node && !tree.isAncestor(p_node, largeNode)) {
			cout << "即将为节点" << p_node->num << "加一,但是同块最大节点字符为：" << largeNode->ch << "，编号为：" << largeNode->num << ", 权重值为：" << p_node->weight << endl;
			tree.swapNode(largeNode, p_node);
			int temp;
			temp = largeNode->num;
			largeNode->num = p_node->num;
			p_node->num = temp;
			for (auto iterator = buffers.begin(); iterator != buffers.end(); iterator++) {
				iterator->wayCode = getHuffmanCode(iterator->p);
			}
		}
		p_node->weight++;
		cout << "节点" << p_node->num << "权重值加1，" << "为：" << p_node->weight << endl;
		p_node = p_node->nParent;
	}
}


//编码函数
bool HuffmanTree::encode(char* out_filename)
{
	//确认文件存在
	if (!is.is_open()) {
		cout << "无法打开文件！" << endl;
		return false;
	}
	os.open(out_filename, ios_base::out);
	if (!os.is_open()) {
		cout << "无法打开写入的文件！" << endl;
	}


	//读取字符，设置nyt节点为根节点
	char charRead;
	Node* newNode = tree.getRoot();
	//因为第一个节点都是新建New节点，所以先向文件中写入'0'
	os << '0';
	while (!is.eof()) {
		charRead = is.get();	//获得单个字符

		//isIn判断是否读取的字符是否在编码的字符中
		bool isIn = false;
		for (int i = 0; i < allInfo.size(); i++)
		{
			if (charRead == allInfo[i].ch)
			{
				isIn = true;
				break;
			}
		}
		if (isIn == false)
		{
			//如果isIn=false，即读到的字符不在规定的解码列表里面，则直接返回false。
			return false;
		}

		//exist判断读取的字符是否已经在树中，即，判断是否已经读取过和当前字符相同的字符。
		bool isExisted = false;
		string code;
		auto existedNode = buffers.begin();	//buffers的一个迭代器，当charRead存在于树中的时候，existNode指向该节点
		for (existedNode; existedNode != buffers.end(); existedNode++) {
			if (existedNode->ch == charRead) {
				//如果charRead已经存在于编码过的字符串中，则获得其路径编码
				code = existedNode->wayCode;
				for (int i = 0; '\0' != code[i]; i++) {
					os << code[i];
				}
				isExisted = true;
				cout << "字符 \'" << charRead << "\' 在树中存在，路径编码为： " << existedNode->wayCode << endl;
				break;
			}
		}
		if (isExisted) {
			Node* tempRoot = existedNode->p;
			weightAdd(tempRoot);
		}
		else {
			//当字符不存在树中时，则新建子树，并替代原newCode节点
			//右节点
			Node* newChar = new Node(nullptr, nullptr, newNode);
			newChar->num = sum++;
			newChar->weight = 1;

			//左节点
			Node* blankNode = new Node(nullptr, nullptr, newNode);
			blankNode->num = sum++;
			blankNode->weight = 0;

			//给树增加左右节点
			tree.addNode(newNode, blankNode, BinaryTree::LeftChild);
			tree.addNode(newNode, newChar, BinaryTree::RightChild);
			newNode->weight = 1;

			//获得新节点的路径编码
			code = getHuffmanCode(newNode);
			for (int i = 0; '\0' != code[i]; i++) {
				//向文件中写入
				os << code[i];
			}
			for (int i = 0; i < allInfo.size(); i++)
			{
				if (charRead == allInfo[i].ch)
				{
					//如果读取的字符在规定的解码列表中
					string label = allInfo[i].label;
					os << label;
					cout << charRead << "首次出现，设定编码为：" << code << label << endl;
					string temp = code + label;
					break;
				}
			}

			//将新的字符放进buffers中
			charMap* new_cm = new charMap();
			new_cm->ch = charRead;
			new_cm->p = newNode->rch;
			new_cm->wayCode = getHuffmanCode(newNode->rch);
			buffers.push_back(*new_cm);

			//依次增加权重
			Node* root = newNode->nParent;
			weightAdd(root);

			//设置新的newCode节点为原newCode节点的左孩子
			newNode = newNode->lch;
		}

	}
	return true;
}

bool HuffmanTree::decode(string code, string out_filename)
{
	//将labelLen设置为初始编码的长度
	labelLen = allInfo[0].label.length();


	string codeHandle = code;
	//nowLoc
	//因为第一段编码都代表新建节点，所以将nowLoc设置为1；
	int nowLoc = 1;
	ofstream otest(out_filename);

	//当前的字符
	char charGet;
	//根节点

	Node* nyt = tree.getRoot();
	//获得全局根节点
	allRoot = tree.getRoot();
	for (nowLoc = 1; nowLoc < codeHandle.length();)
	{
		if (codeHandle[nowLoc] != NULL)
		{
			//如果PI=0，则代表上一段的编码为新建节点，则下一段识别的代码必为新节点的初始编码。
			if (preIndex == 0)
			{
				//从nOWloc截取长度为初始编码的一段字符串
				string tempCode = codeHandle.substr(nowLoc, labelLen);
				for (int i = 0; i < allInfo.size(); i++)
				{
					//如果满足，则为新字符，将其编码，并添加到树中。
					if (allInfo[i].label == tempCode)
					{

						preIndex = 2;	//将preIndex设为2，表示上一段代码为新的字符节点


						//将该字符放入到map中
						map.push_back(allInfo[i].ch);
						otest << allInfo[i].ch;

						//将解码出的的字符赋值给charGet
						charGet = allInfo[i].ch;
						cout <<"\n解码出新字符：" << charGet << endl;

						//此处和编码一样，都需要进行添加节点的操作
						//右孩子
						Node* newChar = new Node(nullptr, nullptr, nyt);
						newChar->num = sum++;
						newChar->weight = 1;	//权值为1
						newChar->ch = charGet;

						//左孩子
						Node* blankNode = new Node(nullptr, nullptr, nyt);
						blankNode->num = sum++;
						blankNode->weight = 0;

						tree.addNode(nyt, blankNode, BinaryTree::LeftChild);
						tree.addNode(nyt, newChar, BinaryTree::RightChild);
						nyt->weight = 1;

						charMap* new_cm = new charMap();
						new_cm->ch = charGet;
						new_cm->p = nyt->rch;
						buffers.push_back(*new_cm);

						//依次增加权重
						Node* root = nyt->nParent;
						weightAdd(root);

						//设置新的nyt节点为原nyt节点的左孩子
						nyt = nyt->lch;

						nowLoc += labelLen;
						break;
					}
				}
			}
			else
			{
				//如果preIndex不为0，则说明上一段编码不是新建节点的操作
				//所以当前的编码为路径编码。
				//获得树的高度，因为路径树的最长路径总是比树的高度少1，所以有减1的操作。
				int tempHgt = Height(allRoot) - 1;

				for (int j = nowLoc; j < nowLoc + tempHgt; j++)
				{
					//以nowLoc为字符串的开始，结尾的索引递增，截取的字符串的最大长度为树的最长路径长度。
					string tempCode = codeHandle.substr(nowLoc, j - nowLoc + 1);
					//获得路径编码之后，通过找到对应的Node。
					Node* nodeFind = tree.findNode(tempCode);

					//如果找到的节点不为空指针
					if (nodeFind != nullptr)
					{

						//如果满足找到节点的字符>0且权重>0，找到的节点为重复节点。
						//和编码一样，都需要进行哈夫曼树的调整。
						if (nodeFind->ch != NULL && nodeFind->weight > 0)
						{
							charGet = nodeFind->ch;
							otest << charGet;
							cout << "\n解码出重复字符：" << charGet << endl;
							auto existNode = buffers.begin();
							for (existNode; existNode != buffers.end(); existNode++)
							{
								if (existNode->ch == charGet)
								{
									break;
								}
							}
							Node* root = existNode->p;
							weightAdd(root);

							preIndex = 1;
							nowLoc += (j - nowLoc + 1);
							break;
						}
						//如果找到的结点的字符<=0且权重为0，则找到的节点为新节点
						else if (nodeFind->ch == NULL && nodeFind->weight == 0)
						{
							//此时将preindex置为0
							preIndex = 0;
							nowLoc += (j - nowLoc + 1);
							break;
						}
					}
				}
			}

		}
	}
	return false;

}


//获得树的高度
int HuffmanTree::Height(Node* root)
{

	int m, n;
	if (root == NULL)
	{
		return 0;
	}
	else
	{
		m = HuffmanTree::Height(root->lch);
		n = HuffmanTree::Height(root->rch);
		return (m > n) ? m + 1 : n + 1;
	}

}


void main()
{
	HuffmanTree huff;
	string f1 = "chs.txt";
	string f2 = "labels.txt";
	getLabls(f1, f2);
	//待解码的字符串
	string code = "0101000010000001001100100110001111101111110111000000111100011100011000001010110011000110110000011010100010000111100011001001111000000111111000000110111111101000010110010101001111000101100011111000100111001001001011011101";	//待解码的字符串
	//otest.txt为用于保存输出的文件
	huff.decode(code, "otest.txt");



}
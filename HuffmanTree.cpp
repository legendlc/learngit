#include <iostream>
#include "HuffmanTree.h"
using std::cout;

TreeNode::TreeNode() {
	value = 0;
	power = 0;
	isLeftChild = true;
	stringCode = "";
	lchild = 0;
	rchild = 0;
}
TreeNode::TreeNode(TreeNode& node) {
	value = node.value;
	power = node.power;
	isLeftChild = node.isLeftChild;
	stringCode = node.stringCode;
	lchild = node.lchild;
	rchild = node.rchild;
}
TreeNode::TreeNode(char ch, double p) {
	value = ch;
	power = p;
	isLeftChild = true;
	stringCode = "";
	lchild = 0;
	rchild = 0;
}

HuffmanTree::HuffmanTree() {
	nNodes = 0;
	Nodes = 0;
	head = new TreeNode;
}
HuffmanTree::~HuffmanTree() {
	delete Nodes;
	recursiveDelete(head);
}
TreeHead HuffmanTree::create(double* p, int num) {
	nNodes = num;
	//Nodes = new TreeNode[num];
	TreeNode* sortSpace = new TreeNode[num];
	for (int i = 0; i < nNodes; i++) {
		//Nodes[i] = TreeNode(char(i), p[i]);
		sortSpace[i] = TreeNode(char(i), p[i]);
	}
	TreeHead treeHead = 0;
	while (num > 1) {
		sort(sortSpace, num);
		TreeNode* leftChild = new TreeNode(sortSpace[num - 2]);
		leftChild->isLeftChild = true;
		TreeNode* rightChild = new TreeNode(sortSpace[num - 1]);
		rightChild->isLeftChild = false;
		treeHead = new TreeNode(-1, leftChild->power + rightChild->power);
		treeHead->lchild = leftChild;
		treeHead->rchild = rightChild;
		sortSpace[num - 2] = *treeHead;
		num--;
	}
	//delete sortSpace;
	this->head = treeHead;
	return treeHead;
}
void HuffmanTree::display() {
	recursiveDisplay(head);
}
void HuffmanTree::sort(TreeNode* p, int n) {
	bool flag_no_swap;
	for (int i = 0; i < n - 1; i++) {
		flag_no_swap = true;
		for (int j = 0; j < n - i - 1; j++) {
			if (p[j].power < p[j + 1].power) {
				swap(p[j], p[j + 1]);
				flag_no_swap = false;
			}
		}
		if (flag_no_swap)
			break;
	}
}
void HuffmanTree::swap(TreeNode& a, TreeNode& b) {
	TreeNode temp = a;
	a = b;
	b = temp;
}
void HuffmanTree::recursiveDelete(TreeNode* p) {
	if (p->value == -1) {
		recursiveDelete(p->lchild);
		recursiveDelete(p->rchild);
	}
	delete p;
}
void HuffmanTree::recursiveDisplay(TreeNode* p) {
	if (p->value == -1) {
		recursiveDisplay(p->lchild);
		recursiveDisplay(p->rchild);
	}
	cout << p->value << ' ';
}



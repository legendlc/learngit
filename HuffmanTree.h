#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <iostream>
#define RELEASE

class TreeNode {
public:
	TreeNode();
	TreeNode(TreeNode& node);
	TreeNode(char ch, double p);
	char value;                   //value is more than -1 for leaf nodes and -1 for those which are not leaf nodes
	double power;                 //the power for a node
	bool isLeftChild;         
	std::string stringCode;       //store the code for the value
	TreeNode* lchild;             //pointer to left child
	TreeNode* rchild;             //pointer to left child
};

typedef TreeNode* TreeHead;

class HuffmanTree {
public:
	HuffmanTree();
	~HuffmanTree();
	TreeHead create(double* p, int num);        //create a Huffman tree with a power array
	void display();                             //test function, nonsense
	TreeHead getHead() { return head; }         //return the root node of the tree
private:
	TreeHead head;
	int nNodes;
	TreeNode* Nodes;
	void sort(TreeNode* p, int n);
	void swap(TreeNode& a, TreeNode& b);
	void recursiveDelete(TreeNode* p);        //recuisively delete the nodes 
	void recursiveDisplay(TreeNode* p);       //test function, nonsense
};

#endif
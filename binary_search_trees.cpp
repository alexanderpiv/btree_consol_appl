
#include <iostream>
using namespace std;
#include "binary_search_trees.h"


binTree* binTree::getRight() {
	return right;
}

void binTree::setRight(binTree* node) {
	right = node;
}

void inorderTreeWalk(binTree* root) {
	if (root != nullptr) {
		inorderTreeWalk(root->getLeft());
		printf("%d ", root->getKey()); 
		inorderTreeWalk(root->getRight());
	}
}

//following book, we add a node rather than just key.. weird..
void treeInsert(binTree* root, binTree* node) {
	if (root != nullptr && node != nullptr) {
		if (node->getKey() > root->getKey()) {
			if (root->getRight() != nullptr) {
				treeInsert(root->getRight(), node);
			}
			else {
				root->setRight(node);
			}
		}
		else {
			if (root->getLeft() != nullptr) {
				treeInsert(root->getLeft(), node);
			}
			else {
				root->setLeft(node);
			}
		}
	}
	else {
		printf("Error: either root or node are null!!\n");
	}
}

void treeInsert2(binTree* root, binTree* node) {
	while (root != nullptr && node != nullptr) {
		if (node->getKey() > root->getKey()) {
			if (root->getRight() != nullptr) {
				root = root->getRight();
			}
			else {
				root->setRight(node);
				break;
			}
		}
		else {
			if (root->getLeft() != nullptr) {
				root = root->getLeft();
			}
			else {
				root->setLeft(node);
				break;
			}
		}
	}
}

//following book - maintain parent pointer and set only in the end
void treeInsert3(binTree* root, binTree* node) {
	binTree* parent = nullptr;
	while (root != nullptr && node != nullptr) {
		parent = root;
		if (node->getKey() > root->getKey()) {
			root = root->getRight();
		}
		else {
			root = root->getLeft();
		}
	}
	if (parent != nullptr) {
		if (node->getKey() > parent->getKey()) {
			parent->setRight(node);
		}
		else {
			parent->setLeft(node);
		}
	}
}

//in book again deleting node rathen than deleting node based on key.. 
void treeDelete(binTree* root, binTree* node) {
	//if node has no children, just remove node
	//if node has only right or left child, replace node with right or left child
	//if node has both children, either go right and then keep going left or go left and then keep going right until hit leaf and remove leaf and set as new root 
	binTree* parent = nullptr;
	while (node->getKey() != root->getKey()) {
		parent = root;
		if (node->getKey() > root->getKey()) {
			root = root->getRight();
		}
		else {
			root = root->getLeft();
		}
	}
	//case 1
	if (root->getRight() == nullptr && root->getLeft() == nullptr) {
		if (parent != nullptr) {
			if (node->getKey() > parent->getKey()) {
				delete parent->getRight();
				parent->setRight(nullptr);
			}
			else {
				delete parent->getLeft();
				parent->setLeft(nullptr);
			}
		}
	}
	//case 2a
	else if (root->getRight() == nullptr && root->getLeft() != nullptr) {
		if (parent != nullptr) {
			if (node->getKey() > parent->getKey())
				parent->setRight(root->getLeft());
			else
				parent->setLeft(root->getLeft());
			delete root;
			root = nullptr;
		}
	}
	//case 2b
	else if (root->getRight() != nullptr && root->getLeft() == nullptr) {
		if (parent != nullptr) {
			if (node->getKey() > parent->getKey())
				parent->setRight(root->getRight());
			else
				parent->setLeft(root->getRight());
			delete root;
			root = nullptr;
		}
	}
	//case 3 - book does it differently - i mean the cases are same but implemenation varies, epecially for the last one.
	else if (root->getRight() != nullptr && root->getLeft() != nullptr) {
		binTree* right = root->getRight();
		//go left until null and keep track of parent
		binTree* parent_lefty = nullptr;
		while (right->getLeft() != nullptr) {
			parent_lefty = right;
			right = right->getLeft();
		}
		if (parent_lefty == nullptr) {
			right->setLeft(root->getLeft());
			if (node->getKey() > parent->getKey())
				parent->setRight(right);
			else
				parent->setLeft(right);
			delete root;
			root = nullptr;
		}
		else {
			right->setLeft(root->getLeft());
			right->setRight(root->getRight());
			if (node->getKey() > parent->getKey())
				parent->setRight(right);
			else
				parent->setLeft(right);
			delete root;
			root = nullptr;
			//delete parent_lefty->getLeft(); //we don;t want to delete since it will delete the atual nde that is still valid. We just want to null parent_lefty's left child reference since we moved it up.
			parent_lefty->setLeft(nullptr);
		}
	}
}


void rotate_left(binTree* root, binTree* node) {
	//find node in the tree
	binTree* parent = nullptr;
	while (node->getKey() != root->getKey()) {
		parent = root;
		if (node->getKey() > root->getKey()) {
			root = root->getRight();
		}
		else {
			root = root->getLeft();
		}
	}
	//store x's (root's) right child
	binTree* x_right = root->getRight();
	root->setRight((root->getRight())->getLeft());
	//if (x_right->getLeft() != nullptr) {
		x_right->setLeft(root);
	//}
	//swap parent's child (x) with child of child (y)
	if (node->getKey() > parent->getKey())
		parent->setRight(x_right);
	else
		parent->setLeft(x_right);
}

//changed x_right to y_left and then changed all get/setLeft to get/setRight and vice versa
void rotate_right(binTree* root, binTree* node) {
	//find node in the tree
	binTree* parent = nullptr;
	while (node->getKey() != root->getKey()) {
		parent = root;
		if (node->getKey() > root->getKey()) {
			root = root->getRight();
		}
		else {
			root = root->getLeft();
		}
	}
	//store x's (root's) right child
	binTree* y_left = root->getLeft();
	root->setLeft((root->getLeft())->getRight());
	//if (x_right->getLeft() != nullptr) {
	y_left->setRight(root);
	//}
	//swap parent's child (x) with child of child (y)
	if (node->getKey() > parent->getKey())
		parent->setRight(y_left);
	else
		parent->setLeft(y_left);
}

//not finishing impelremtnaiton of Red Black trees which involves adding an updated Tree insert and an updated Tree delete functinos.
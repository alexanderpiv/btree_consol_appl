#pragma once

class binTree {
public:
/*	binTree() {
		left = nullptr;
		right = nullptr;
		key = -1;
		data = -1;
	}
*/
	binTree(int keyIn) {
		left = nullptr;
		right = nullptr;
		key = keyIn;
		data = -1;
	}
	~binTree() {}

	//declaring and implementing inside
	binTree* getLeft() {
		return left;
	}
	void setLeft(binTree* node) {
		left = node;
	}

	//declaring in class and implementing outside
	binTree* getRight();
	void setRight(binTree*);

	int getData() {
		return data;
	}

	int getKey() {
		return key;
	}
	void setKey(int keyIn) {
		key = keyIn;
	}
private:
	binTree* left;
	binTree* right;
	int key;
	int data; //can be any data type / struct/ class etc
};


void inorderTreeWalk(binTree*);
void treeInsert(binTree*, binTree*);
void treeInsert2(binTree*, binTree*);
void treeInsert3(binTree*, binTree*);
void treeDelete(binTree*, binTree*);
void rotate_left(binTree*, binTree*);
void rotate_right(binTree*, binTree*);




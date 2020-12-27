#include <iostream>
#include <assert.h>

using namespace std;

class btree {
public:
    btree(int order_loc, int data_record_num_loc) {
        printf("Received order size %d, i.e. %d keys; max data records inputted=%d\n", order_loc, order_loc - 1, data_record_num_loc);
        //maybe dont need below two if will use them only in this constructor
        order = order_loc + 1; //affects ptr and key; +1 for easier management of splitting
        data_record_num = data_record_num_loc + 1; //affects leaf data; +1 for easier management of splitting
        keys = new int[order - 1];
        for (int i = 0; i < order - 1; i++) {
            keys[i] = -1;
        }
        //tree_ptr = new btree(order_loc); //this should be a n infinte loop - let's run and check
        tree_ptr = new btree * [order];
        for (int i = 0; i < order; i++) {
            tree_ptr[i] = nullptr;
        }
        data_record_cur = 0;
        curr_fill = 0;
    }
    btree() {
        printf("This is the default constructor, B Tree properties not specified, not proceeding further\n");
    }

    //for now make all poublic; later can add accessor functions and hide below. ACtually made add_in_order external functions, so yes, i this case cannot access provate var.
    //private:
    int order; //#keys = order - 1
    int data_record_num; //max num of data records
    int* keys; //array of int keys, will intiitalize with order-1
    btree** tree_ptr; //array of pointers with array size = order
    int data_record_cur; //current fill level of leaf
    int curr_fill;
};



void print_tree(btree* tree_root, int level = 0, int side = 0) {
    int i = 0;
    while (tree_root->keys[i] != -1) {
        printf("Level %d and key %d and side %d\n", level, tree_root->keys[i], side);
        if (tree_root->tree_ptr[i] != nullptr) {
            print_tree(tree_root->tree_ptr[i], level + 1, 1);
        }
        if (tree_root->tree_ptr[i + 1] != nullptr) {
            print_tree(tree_root->tree_ptr[i + 1], level + 1, 2);
        }
        i++;
    }
}

struct addElem_returnVal {
    btree* rightBtree;
    bool added;
    int midKey;
};

//TODO!!!!!!!!!!!!!!!!!!!!!!!!:
//1) Add code to work with even order number; currently works only with odd
//2) Remove pass by reference added and midKey and return them instead together with btree* - I guess create a struct.
//3) I assume eed to delete "new" objects.
//4) Add remove element funcitonality

addElem_returnVal add_elem(int key, btree*& tree_root, btree*& tree_root_parent) { //second btree is parent root
    int i = 0;
    int extraKey = tree_root->order - 2;

    addElem_returnVal treeMore;

    treeMore.rightBtree = nullptr;
    treeMore.added = false;
    treeMore.midKey = -5;

    while (tree_root->keys[i] != -1 && key >= tree_root->keys[i]) {
        i++;
    }
    if (tree_root->keys[i] == -1) {
        if (tree_root->tree_ptr[i] != nullptr) { //going right subtree
            treeMore = add_elem(key, tree_root->tree_ptr[i], tree_root);
        }
    }
    else if (tree_root->tree_ptr[i] != nullptr) { //going left subtree
        treeMore = add_elem(key, tree_root->tree_ptr[i], tree_root);
    }

    //if tree has space for more keys, add them - when tree is empty, will do this and fill first level this way
    //NEW: Here adding new element assuming there is space - if it's a leaf, rightBtree=null which is ok and if it's a node with children, 
    //      will add its right tree to the right; left is untouched and all left nodes are untouched and to the right of node all were moved.
    //Newer: if have rightBtree==nullptr, then already added element (to leaves)
    if (tree_root->keys[extraKey] == -1 && !treeMore.added) { //the latter two tree_ptr check if we are at the leaf
        for (int j = tree_root->curr_fill; j > i; j--) {
            tree_root->keys[j] = tree_root->keys[j - 1];
            tree_root->tree_ptr[j + 1] = tree_root->tree_ptr[j];
        }
        tree_root->keys[i] = key;
        tree_root->tree_ptr[i + 1] = treeMore.rightBtree;
        tree_root->curr_fill++;
        treeMore.added = true;
    }
    if (tree_root->keys[extraKey] == -1 && treeMore.rightBtree != nullptr) { //the latter two tree_ptr check if we are at the leaf
        for (int j = tree_root->curr_fill; j > i; j--) {
            tree_root->keys[j] = tree_root->keys[j - 1];
            tree_root->tree_ptr[j + 1] = tree_root->tree_ptr[j];
        }
        tree_root->keys[i] = treeMore.midKey;
        tree_root->tree_ptr[i + 1] = treeMore.rightBtree;
        tree_root->curr_fill++;
        treeMore.added = true;
    }
    //if just filled extra key space, need to split
    //tree_root will be updated to be the left subtree and newRightBtree will be created and populated to be the right subtree
    if (tree_root->keys[extraKey] != -1) {
        //extract mid element to serve as new root
        treeMore.midKey = tree_root->keys[(extraKey + 1) / 2]; //extraKey + 1 = num of leaf elements

        //create right new tree
        btree* newRightBtree = new btree(tree_root->order - 1, 3);

        for (int i = 0; i < tree_root->order / 2 - 1; i++) { //-1 for odd order (or even, depends if you take my +1 to order into account).
            //populate new right tree
            newRightBtree->keys[i] = tree_root->keys[tree_root->order / 2 + i];
            newRightBtree->curr_fill++;
            newRightBtree->tree_ptr[i + 1] = tree_root->tree_ptr[tree_root->order / 2 + i + 1];
            //update/remove element from original left tree
            tree_root->keys[tree_root->order / 2 + i] = -1;
            tree_root->tree_ptr[tree_root->order / 2 + i + 1] = nullptr;
            tree_root->curr_fill--;
        }
        newRightBtree->tree_ptr[0] = tree_root->tree_ptr[tree_root->order / 2];

        //remove mid element
        tree_root->keys[tree_root->curr_fill - 1] = -1;
        tree_root->tree_ptr[tree_root->curr_fill] = nullptr;
        tree_root->curr_fill--;

        if (tree_root_parent == tree_root) {
            btree* newRoot = new btree(tree_root->order - 1, 3);
            newRoot->tree_ptr[0] = tree_root;
            newRoot->tree_ptr[1] = newRightBtree;
            newRoot->keys[0] = treeMore.midKey;
            newRoot->curr_fill++;
            tree_root = newRoot;
            treeMore.rightBtree = nullptr;
            return treeMore;
        }
        else {
            treeMore.rightBtree = newRightBtree;
            return treeMore;
        }
    }
    else {
        treeMore.rightBtree = nullptr;
        return treeMore;
    }

}

int main()
{
    cout << "Hello World\n";
    btree btree_inst;
    btree btree_inst_actual(5, 3); //order 3 means when reaching 3 elemnts in a tree, need to split, i.e. should have max of 2 elements per tree.
    btree* btree_inst2 = &btree_inst_actual; //need this since otherwise passing &btree_inst_actual instead won't work with add_elem func arg of btree*& val, i.e. cannot pass by reference the adress of the class..
    add_elem(2, btree_inst2, btree_inst2);
    print_tree(btree_inst2);
    printf("DONE Iter 1\n");
    add_elem(3, btree_inst2, btree_inst2);
    print_tree(btree_inst2);
    printf("DONE Iter 1\n");
    add_elem(5, btree_inst2, btree_inst2);
    print_tree(btree_inst2);
    printf("DONE Iter 2\n");
    add_elem(7, btree_inst2, btree_inst2);
    print_tree(btree_inst2);
    printf("DONE Iter 3\n");
    add_elem(11, btree_inst2, btree_inst2);
    print_tree(btree_inst2);
    printf("DONE Iter 4\n");
    add_elem(6, btree_inst2, btree_inst2);
    print_tree(btree_inst2);
    printf("DONE Iter 5\n");
    add_elem(15, btree_inst2, btree_inst2);
    print_tree(btree_inst2);
    printf("DONE Iter 6\n");
    add_elem(16, btree_inst2, btree_inst2);
    print_tree(btree_inst2);
    printf("DONE Iter 7\n");
    add_elem(19, btree_inst2, btree_inst2);
    print_tree(btree_inst2);
    printf("DONE Iter 8\n");
    add_elem(20, btree_inst2, btree_inst2);
    print_tree(btree_inst2);
    printf("DONE Iter 9\n");
    add_elem(23, btree_inst2, btree_inst2);
    print_tree(btree_inst2);
    printf("DONE Iter 10\n");
    add_elem(22, btree_inst2, btree_inst2);
    print_tree(btree_inst2);
    printf("DONE Iter 11\n");
    add_elem(25, btree_inst2, btree_inst2);
    print_tree(btree_inst2);
    printf("DONE Iter 12\n");
    add_elem(30, btree_inst2, btree_inst2);
    print_tree(btree_inst2);
    printf("DONE Iter 13\n");
    add_elem(31, btree_inst2, btree_inst2);
    print_tree(btree_inst2);
    printf("DONE Iter 14\n");
    add_elem(32, btree_inst2, btree_inst2);
    print_tree(btree_inst2);
    printf("DONE Iter 15\n");
    add_elem(33, btree_inst2, btree_inst2);
    print_tree(btree_inst2);
    printf("DONE Iter 16\n");
    add_elem(35, btree_inst2, btree_inst2);
    print_tree(btree_inst2);
    printf("DONE Iter 17\n");
    add_elem(37, btree_inst2, btree_inst2);
    print_tree(btree_inst2);
    printf("DONE Iter 18\n");
    add_elem(13, btree_inst2, btree_inst2);
    print_tree(btree_inst2);
    printf("DONE Iter 19\n");
    return 0;
}


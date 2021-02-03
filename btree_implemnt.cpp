#include <iostream>
#include <assert.h>

#include "sorts.h"
#include "heap.cpp"
#include "binary_search_trees.h"
#include "LCS.h"
#include "graph.h"
#include "various.h"

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
//3) I assume eed to delete "new" objects.
//5) IMpelment preemptive insertion
//6) Add corner cases like removing non existent element..

addElem_returnVal add_elem(int key, btree*& tree_root, btree*& tree_root_parent, bool addAlways) { //second btree is parent root
    int i = 0;
    int extraKey = tree_root->order - 2;

    addElem_returnVal treeMore;

    treeMore.rightBtree = nullptr;
    treeMore.added = false;
    treeMore.midKey = -5;

    if (addAlways) {
        //USERSET (sort of): assuming key==parent, if want key in left substree, have: key > tree_root->keys[i], else use: key >= tree_root->keys[i]
        while (tree_root->keys[i] != -1 && key > tree_root->keys[i]) {
            i++;
        }
    }
    else {
        while (tree_root->keys[i] != -1 && key > tree_root->keys[i]) {
            i++;
        }
        //if key exists already, no need to add it again
        if (key == tree_root->keys[i]) {
            treeMore.added = true;
            printf("Key %d already exists so not adding it again\n", key);
            return treeMore;
        }
    }

    if (tree_root->keys[i] == -1) {
        if (tree_root->tree_ptr[i] != nullptr) { //going right subtree
            treeMore = add_elem(key, tree_root->tree_ptr[i], tree_root, addAlways);
        }
    }
    else if (tree_root->tree_ptr[i] != nullptr) { //going left subtree
        treeMore = add_elem(key, tree_root->tree_ptr[i], tree_root, addAlways);
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

int next_smallest(btree* tree_root) {
    int i = 0;
    while (tree_root->keys[i] != -1) {
        i++; 
    }
    if (tree_root->tree_ptr[i] != nullptr) {
        return next_smallest(tree_root->tree_ptr[i]);
    }
    else {
        return tree_root->keys[i - 1];
    }
}

bool remove_elem(int key, btree*& tree_root, btree*& tree_root_parent) { //second btree is parent root
    int i = 0;
    int extraKey = tree_root->order - 2;

    addElem_returnVal treeMore;

    treeMore.rightBtree = nullptr;
    treeMore.added = false;
    treeMore.midKey = -5;

    while (tree_root->keys[i] != -1 && key > tree_root->keys[i]) {
        i++;
    }

    bool removed = false;
    //if found key and key is not leaf node, find a replacement for this key from the next smaller entry (at the leaf will be) 
    //OPTimization: by the way, can check just one sub tree rather than both since both should always exist...
    if (key == tree_root->keys[i] && tree_root->tree_ptr[i + 1] != nullptr && tree_root->tree_ptr[i] != nullptr) {
        int newKey = next_smallest(tree_root->tree_ptr[i]);
        tree_root->keys[i] = newKey;
        removed = remove_elem(newKey, tree_root->tree_ptr[i], tree_root);
    }
    else if (tree_root->keys[i] == -1) {
        if (tree_root->tree_ptr[i] != nullptr) { //going right subtree
            removed = remove_elem(key, tree_root->tree_ptr[i], tree_root);
        }
    }
    else if (tree_root->tree_ptr[i] != nullptr) { //going left subtree
        removed = remove_elem(key, tree_root->tree_ptr[i], tree_root);
    }
    else if (key == tree_root->keys[i] && tree_root->tree_ptr[i + 1] == nullptr && tree_root->tree_ptr[i] == nullptr) {
        //shift left onto space that want to remove the lements to the right of the one that want to remove
        for (int j = i; j < tree_root->curr_fill - 1; j++) {
            tree_root->keys[j] = tree_root->keys[j + 1];
        }
        tree_root->curr_fill--;
        tree_root->keys[tree_root->curr_fill] = -1;
        return true;
    }

    if (removed) { 
        //if removing from left subtree and it becomes <2 and right subtree is LESS than half full, then merge left subtree with parent node 
        //(and remove parent node and shift left the rmaininig pointers and keys) and with right subtree
        //A1
        if (tree_root->tree_ptr[i]->curr_fill == tree_root->order / 2 - 2 && i == 0 && tree_root->tree_ptr[i + 1]->curr_fill < tree_root->order / 2) {
            tree_root->tree_ptr[i]->keys[tree_root->tree_ptr[i]->curr_fill] = tree_root->keys[i];
            tree_root->tree_ptr[i]->curr_fill++;
            //on leaf level copy right subtree into left subtree
            for (int j = 0; j < tree_root->tree_ptr[i + 1]->curr_fill; j++) {
                tree_root->tree_ptr[i]->keys[j + tree_root->tree_ptr[i]->curr_fill] = tree_root->tree_ptr[i + 1]->keys[j];
                //below ptr copy is releavnt only for if node's level below is not leaf - but won't hurt to do if it is.
                tree_root->tree_ptr[i]->tree_ptr[tree_root->tree_ptr[i]->curr_fill + j] = tree_root->tree_ptr[i + 1]->tree_ptr[j];
            }
            tree_root->tree_ptr[i]->curr_fill += tree_root->tree_ptr[i + 1]->curr_fill;
            //below releavnt for if node's level beliw is not leaft - copy rightmost pointer from right subtree as well to left subtree
            tree_root->tree_ptr[i]->tree_ptr[tree_root->tree_ptr[i]->curr_fill] = tree_root->tree_ptr[i + 1]->tree_ptr[tree_root->tree_ptr[i + 1]->curr_fill];
            delete tree_root->tree_ptr[i + 1];
            //on parent level shift left onto space that want to remove the lements to the right of the one that want to remove
            for (int j = i; j < tree_root->curr_fill - 1; j++) {
                tree_root->keys[j] = tree_root->keys[j + 1];
                //keep j=i ptr, i.e. left one, as is
                tree_root->tree_ptr[j + 1] = tree_root->tree_ptr[j + 2];
            }
            tree_root->curr_fill--;
            tree_root->tree_ptr[tree_root->curr_fill + 1] = nullptr;
            tree_root->keys[tree_root->curr_fill] = -1;
            return true;
        }
        //if removing from left subtree and it becomes <2 and right subtree is MORE than half full, then steal from right subtree leftmost element and make new root
        //and move previous root to rightmost elemtn in left subtree
        else if (tree_root->tree_ptr[i]->curr_fill == tree_root->order / 2 - 2 && i == 0 && tree_root->tree_ptr[i + 1]->curr_fill >= tree_root->order / 2) {
            tree_root->tree_ptr[i]->keys[tree_root->tree_ptr[i]->curr_fill] = tree_root->keys[i];
            tree_root->tree_ptr[i]->curr_fill++;

            tree_root->keys[i] = tree_root->tree_ptr[i + 1]->keys[0];
            //shift left by one right subtree due to keys[0] going up to parent
            for (int j = 0; j < tree_root->tree_ptr[i + 1]->curr_fill - 1; j++) {
                tree_root->tree_ptr[i + 1]->keys[j] = tree_root->tree_ptr[i + 1]->keys[j + 1];
            }
            tree_root->tree_ptr[i + 1]->curr_fill--;
            tree_root->tree_ptr[i + 1]->keys[tree_root->tree_ptr[i + 1]->curr_fill] = -1;
        }
        //if removing from left subtree and it DOES NOT become <2, then do nothing.
        else if (tree_root->tree_ptr[i]->curr_fill > tree_root->order / 2 - 2) {
            return true;
        }
        //if left subtree is <2 but it's not the leftmost subtree, then merge with lefter one, assuming it is not more than half full
        else if (tree_root->tree_ptr[i]->curr_fill == tree_root->order / 2 - 2 && i > 0 && tree_root->tree_ptr[i - 1]->curr_fill < tree_root->order / 2) {
            //adding root to lefter subtree
            tree_root->tree_ptr[i-1]->keys[tree_root->tree_ptr[i-1]->curr_fill] = tree_root->keys[i-1];
            tree_root->tree_ptr[i-1]->curr_fill++;
            //on leaf level copy right (left really) subtree into left (lefter really) subtree
            for (int j = 0; j < tree_root->tree_ptr[i]->curr_fill; j++) {
                tree_root->tree_ptr[i-1]->keys[j + tree_root->tree_ptr[i-1]->curr_fill] = tree_root->tree_ptr[i]->keys[j];
                //below ptr copy is releavnt only for if node's level below is not leaf - but won't hurt to do if it is.
                tree_root->tree_ptr[i-1]->tree_ptr[tree_root->tree_ptr[i-1]->curr_fill + j] = tree_root->tree_ptr[i]->tree_ptr[j];
            }
            tree_root->tree_ptr[i-1]->curr_fill += tree_root->tree_ptr[i]->curr_fill;
            //below releavnt for if node's level beliw is not leaft - copy rightmost pointer from right subtree as well to left subtree
            tree_root->tree_ptr[i-1]->tree_ptr[tree_root->tree_ptr[i-1]->curr_fill] = tree_root->tree_ptr[i]->tree_ptr[tree_root->tree_ptr[i]->curr_fill];
            delete tree_root->tree_ptr[i];
            //on parent level shift left onto space that want to remove the lements to the right of the one that want to remove
            for (int j = i-1; j < tree_root->curr_fill - 1; j++) {
                tree_root->keys[j] = tree_root->keys[j + 1];
                //keep j=i ptr, i.e. left one, as is
                tree_root->tree_ptr[j + 1] = tree_root->tree_ptr[j + 2];
            }
            tree_root->curr_fill--;
            tree_root->tree_ptr[tree_root->curr_fill + 1] = nullptr;
            tree_root->keys[tree_root->curr_fill] = -1;
            return true;
        }
        //if left subtree is <2 but it's not the leftmost subtree, then steal from lefter one, assuming it is more than half full
        else if (tree_root->tree_ptr[i]->curr_fill == tree_root->order / 2 - 2 && i > 0 && tree_root->tree_ptr[i - 1]->curr_fill >= tree_root->order / 2) {
            //shift right by one the left (right really) subtree and then put in place [0] the root key
            for (int j = tree_root->tree_ptr[i]->curr_fill; j > 0; j--) {
                tree_root->tree_ptr[i]->keys[j] = tree_root->tree_ptr[i]->keys[j - 1];
                tree_root->tree_ptr[i]->tree_ptr[j + 1] = tree_root->tree_ptr[i]->tree_ptr[j];
            }
            tree_root->tree_ptr[i]->keys[0] = tree_root->keys[i - 1];
            tree_root->tree_ptr[i]->tree_ptr[1] = tree_root->tree_ptr[i]->tree_ptr[0];
            tree_root->tree_ptr[i]->curr_fill++;
            //take lefter substree's rightmost subtree and assign to left (right really) [0] key's subtree
            tree_root->tree_ptr[i]->tree_ptr[0] = tree_root->tree_ptr[i - 1]->tree_ptr[tree_root->tree_ptr[i - 1]->curr_fill];
            //set as new root the rightmost key of lefter subtree
            tree_root->keys[i - 1] = tree_root->tree_ptr[i - 1]->keys[tree_root->tree_ptr[i - 1]->curr_fill - 1];
            //remove last key in lefter subtree now and set the rightmost subtree pntr to null since already assigned it to left (right) subtree above
            tree_root->tree_ptr[i - 1]->keys[tree_root->tree_ptr[i - 1]->curr_fill - 1] = -1;
            tree_root->tree_ptr[i - 1]->tree_ptr[tree_root->tree_ptr[i - 1]->curr_fill] = nullptr;
            tree_root->tree_ptr[i - 1]->curr_fill--;
        }

    }
 
}



int main()
{
    //Current implementation and settings are as follows:
    //1) Adding elements even if exist (bool addEvenIfExists = true;) - verified correctness of converse scenario by eye check
    //2) Generating non unique elements (bool unique = false;) - verified correctness of converse by having program hang when had elem_range<elem_num and then couldn't generate more unique values.
    //3) If new element == parent, adding it in left substree if there is space there (given above settings and given we have at least 2 levels already populated) (key > tree_root->keys[i])

    cout << "Hello World\n";
    bool runBtreeCode = false;
    if (runBtreeCode) {
        btree btree_inst;
        btree btree_inst_actual(5, 3); //USERSET //order 3 means when reaching 3 elemnts in a tree, need to split, i.e. should have max of 2 elements per tree.
        btree* btree_inst2 = &btree_inst_actual; //need this since otherwise passing &btree_inst_actual instead won't work with add_elem func arg of btree*& val, i.e. cannot pass by reference the adress of the class..
        time_t seed = time(NULL);
        seed = 1234;
        srand(seed);
        printf("Starting with seed = %lld\n", seed);
        bool addEvenIfExists = true; //USERSET
        int elem_num = 40; //USERSET
        elem_num = 26;
        int* elem_arr = new int[elem_num];
        int elem_range = 50; //USERSET
        bool unique = false; //USERSET
        unique = true;
        bool sequential = false; //USERSET
        sequential = true;
        for (int iter = 0; iter < elem_num; iter++) {
            int elem = rand() % elem_range;
            if (sequential) {
                elem = iter + 1;
            }
            if (unique) {
                bool elem_unique = false;
                while (!elem_unique) {
                    int search;
                    for (search = 0; search < iter; search++) {
                        if (elem == elem_arr[search]) {
                            elem = rand() % elem_range;
                            break;
                        }
                    }
                    if (search == iter) {
                        elem_unique = true;
                    }
                }
            }
            printf("Adding element %d\n", elem);
            add_elem(elem, btree_inst2, btree_inst2, addEvenIfExists);
            print_tree(btree_inst2);
            printf("DONE Iter %d\n", iter);
            elem_arr[iter] = elem;
        }
        printf("Added the following list of elements:\n");
        for (int elm_i = 0; elm_i < elem_num; elm_i++) {
            printf("%d ", elem_arr[elm_i]);
        }
        printf("\nDone\n");

        int elem = 1;
        int iter = 0;
        printf("Removing element %d\n", elem);
        //bool goright = false;
        remove_elem(elem, btree_inst2, btree_inst2/*, goright*/);
        print_tree(btree_inst2);
        printf("DONE Iter %d\n", iter);

        elem = 24;
        iter = 1;
        printf("Removing element %d\n", elem);
        //bool goright = false;
        remove_elem(elem, btree_inst2, btree_inst2/*, goright*/);
        print_tree(btree_inst2);
        printf("DONE Iter %d\n", iter);

        elem = 12;
        iter = 2;
        printf("Removing element %d\n", elem);
        //bool goright = false;
        remove_elem(elem, btree_inst2, btree_inst2/*, goright*/);
        print_tree(btree_inst2);
        printf("DONE Iter %d\n", iter);

        add_elem(27, btree_inst2, btree_inst2, addEvenIfExists);
        add_elem(28, btree_inst2, btree_inst2, addEvenIfExists);
        add_elem(29, btree_inst2, btree_inst2, addEvenIfExists);

        elem = 6;
        iter = 3;
        printf("Removing element %d\n", elem);
        //bool goright = false;
        remove_elem(elem, btree_inst2, btree_inst2/*, goright*/);
        print_tree(btree_inst2);
        printf("DONE Iter %d\n", iter);

        elem = 5;
        iter = 4;
        printf("Removing element %d\n", elem);
        //bool goright = false;
        remove_elem(elem, btree_inst2, btree_inst2/*, goright*/);
        print_tree(btree_inst2);
        printf("DONE Iter %d\n", iter);

        elem = 4;
        iter = 5;
        printf("Removing element %d\n", elem);
        //bool goright = false;
        remove_elem(elem, btree_inst2, btree_inst2/*, goright*/);
        print_tree(btree_inst2);
        printf("DONE Iter %d\n", iter);

        elem = 23;
        iter = 6;
        printf("Removing element %d\n", elem);
        //bool goright = false;
        remove_elem(elem, btree_inst2, btree_inst2/*, goright*/);
        print_tree(btree_inst2);
        printf("DONE Iter %d\n", iter);

        elem = 22;
        iter = 7;
        printf("Removing element %d\n", elem);
        //bool goright = false;
        remove_elem(elem, btree_inst2, btree_inst2/*, goright*/);
        print_tree(btree_inst2);
        printf("DONE Iter %d\n", iter);

        elem = 21;
        iter = 8;
        printf("Removing element %d\n", elem);
        //bool goright = false;
        remove_elem(elem, btree_inst2, btree_inst2/*, goright*/);
        print_tree(btree_inst2);
        printf("DONE Iter %d\n", iter);

        elem = 27;
        iter = 9;
        printf("Removing element %d\n", elem);
        //bool goright = false;
        remove_elem(elem, btree_inst2, btree_inst2/*, goright*/);
        print_tree(btree_inst2);
        printf("DONE Iter %d\n", iter);

    }
   
    printf("Running insertion sort on:\n");
    int arr[] = { 4,6,2,3,1,5,7,9,15,12 };
    int sz = sizeof(arr) / sizeof(arr[0]);
    cout << "arr size = " << sz << endl;
    print_arr(arr, sz);
    insertion_sort(arr, sz);
    printf("Done running insertion sort and it is now:\n");
    print_arr(arr, sz); 

    printf("Running merge sort on:\n");
    int arr2[] = { 4,6,2,3,1,5,7,9,15,12,8 };
    int sz2 = sizeof(arr2) / sizeof(arr2[0]);
    sz = sizeof(arr2) / sizeof(arr2[0]);
    print_arr(arr2, sz2);
    merge_sort(arr2, 0, sz2-1);
    printf("Done running merge sort and it is now:\n");
    print_arr(arr2, sz2);

    printf("Running insertion sort variantion 2 on:\n");
    int arr3[] = { 4,6,2,3,1,5,7,9,15,12,8 };
    int sz3 = sizeof(arr3) / sizeof(arr3[0]);
    sz3 = sizeof(arr3) / sizeof(arr3[0]);
    print_arr(arr3, sz3);
    merge_sort(arr3, 0, sz3 - 1);
    printf("Done running insertion sort variantion 2 and it is now:\n");
    print_arr(arr3, sz3);

    printf("Running insertion sort variantion 3 on:\n");
    int arr4[] = { 4,6,2,3,1,5,7,9,15,12,8 };
    int sz4 = sizeof(arr4) / sizeof(arr4[0]);
    sz4 = sizeof(arr4) / sizeof(arr4[0]);
    print_arr(arr4, sz4);
    merge_sort(arr4, 0, sz4 - 1);
    printf("Done running insertion sort variantion 3 and it is now:\n");
    print_arr(arr4, sz4);

    printf("Running heap..no longer as in book since modified input to version 2\n");
    bin_heap bheap(20);
    printf("Before max_heapify\n");
    bheap.print_arr();
    printf("After max_heapify\n");
    bheap.max_heapify(2);
    bheap.print_arr();
    printf("Done running heap..\n");

    printf("Running heap2..no longer as in book since modified input to version 2\n");
    bin_heap bheap2(20);
    printf("Before max_heapify2\n");
    bheap2.print_arr();
    printf("After max_heapify2\n");
    bheap2.max_heapify2(2);
    bheap2.print_arr();
    printf("Done running heap2..\n");


    printf("Running heap3..changed input array to be unsorted completely more or less\n");
    bin_heap bheap3(20);
    printf("Before build_heap\n");
    bheap3.print_arr();
    printf("After build_heap\n");
    bheap3.build_heap();
    bheap3.print_arr();
    printf("Done running heap3..\n");


    printf("Running heap4..heapsort\n");
    bin_heap bheap4(20);
    printf("Before heapsort\n");
    bheap4.print_arr();
    bheap4.heapsort();
    printf("After heapsort\n");
    bheap4.print_arr();
    printf("Done running heap4..\n");

    printf("Running heap5\n");
    bin_heap bheap5(20);
    printf("Before build_heap\n");
    bheap5.print_arr();
    printf("After build_heap\n");
    bheap5.build_heap();
    bheap5.print_arr();
    printf("heap max = %d\n", bheap5.heap_maximum());
    printf("extract and return heap max = %d\n", bheap5.heap_extract_maximum());
    printf("print now resultalnnt heap\n");
    bheap5.print_arr();
    printf("Done running heap5..\n");

    printf("Running heap6..changed input array to be unsorted completely more or less\n");
    bin_heap bheap6(20);
    printf("Before build_heap\n");
    bheap6.print_arr();
    printf("After build_heap\n");
    bheap6.build_heap();
    bheap6.print_arr();
    bheap6.heap_increase_key2(4, 50);
    printf("print now resultalnnt heap after modifiying element 4 to value 50\n");
    bheap6.print_arr();
    bheap6.heap_insert(30);
    printf("print now resultalnnt heap after also inserting 30 as new elemtn\n");
    bheap6.print_arr();
    printf("Done running heap3..\n");

    printf("Running quicksort on:\n");
    int arr5[] = { 4,6,2,3,1,5,7,9,15,12,8 };
    int sz5 = sizeof(arr5) / sizeof(arr5[0]);
    sz5 = sizeof(arr5) / sizeof(arr5[0]);
    print_arr(arr5, sz5);
    quicksort(arr5, 0, sz5 - 1);
    printf("Done running quicksort it is now:\n");
    print_arr(arr5, sz5);

    printf("Running counting sort on:\n");
    int arr6[] = { 2,5,3,0,2,3,0,3 };
    int sz6 = sizeof(arr6) / sizeof(arr6[0]);
    sz6 = sizeof(arr6) / sizeof(arr6[0]);
    print_arr(arr6, sz6);
    counting_sort(arr6, sz6, 6);
    printf("Done running counting sort it is now:\n");
    print_arr(arr6, sz6);

    printf("Running merge sort \n");
    int arr9[] = { 4,6,2,3,1,5,7,9,15,12,8 };
    int sz9 = sizeof(arr9) / sizeof(arr9[0]);
    sz9 = sizeof(arr9) / sizeof(arr9[0]);
    print_arr(arr9, sz9);
    mergesort(arr9, 0, sz9 - 1);
    printf("Done running merge sort\n");
    print_arr(arr9, sz9);

    printf("\nStarting with binary trees...\n");
    binTree * binaryTree = new binTree(5);
    inorderTreeWalk(binaryTree);
    printf("\nDone printining 1st time - new tree with initial value 5\n");
    binTree* newNode = new binTree(10);
    treeInsert(binaryTree, newNode);
    inorderTreeWalk(binaryTree);
    printf("\nDone printining 2nd time - inserted 10\n");
    binTree* newNode2 = new binTree(3);
    treeInsert2(binaryTree, newNode2);
    inorderTreeWalk(binaryTree);
    printf("\nDone printining 3rd time - inserted 3\n");
    binTree* newNode3 = new binTree(13);
    treeInsert3(binaryTree, newNode3);
    inorderTreeWalk(binaryTree);
    printf("\nDone printining 4th time - inserted 13\n");
    treeDelete(binaryTree, newNode2);
    inorderTreeWalk(binaryTree);
    printf("\nDone printining 5th time - deleted 3 that has no children\n");
    treeDelete(binaryTree, newNode);
    inorderTreeWalk(binaryTree);
    printf("\nDone printining 6th time - deleted 10 that has one child\n");

    binTree* binaryTree2 = new binTree(30);
    binTree* newNode_1 = new binTree(20);
    treeInsert3(binaryTree2, newNode_1);
    binTree* newNode_2 = new binTree(41);
    treeInsert3(binaryTree2, newNode_2);
    binTree* newNode_3 = new binTree(15);
    treeInsert3(binaryTree2, newNode_3);
    binTree* newNode_4 = new binTree(25);
    treeInsert3(binaryTree2, newNode_4);
    binTree* newNode_5 = new binTree(12);
    treeInsert3(binaryTree2, newNode_5);
    binTree* newNode_6 = new binTree(34);
    treeInsert3(binaryTree2, newNode_6);
    binTree* newNode_7 = new binTree(45);
    treeInsert3(binaryTree2, newNode_7);
    binTree* newNode_8 = new binTree(37);
    treeInsert3(binaryTree2, newNode_8);

    //skip inorderTreeWalk(binaryTree2);
    //skip printf("\nInitial setup above\n");
    //skip treeDelete(binaryTree2, newNode_6);
    //skip treeDelete(binaryTree2, newNode_3);
    //skip inorderTreeWalk(binaryTree2);
    //skip printf("\nAfter deleting 35 and 15 setup above\n");
    binTree* newNode_9 = new binTree(33);
    treeInsert3(binaryTree2, newNode_9);
    binTree* newNode_10 = new binTree(31);
    treeInsert3(binaryTree2, newNode_10);
    binTree* newNode_11 = new binTree(32);
    treeInsert3(binaryTree2, newNode_11);
    binTree* newNode_12 = new binTree(36);
    treeInsert3(binaryTree2, newNode_12);
    binTree* newNode_13 = new binTree(40);
    treeInsert3(binaryTree2, newNode_13);
    binTree* newNode_14 = new binTree(35);
    treeInsert3(binaryTree2, newNode_14);
    binTree* newNode_15 = new binTree(47);
    treeInsert3(binaryTree2, newNode_15);
    inorderTreeWalk(binaryTree2);
    printf("\nInitial setup above\n");
    //works: treeDelete(binaryTree2, newNode_2); //delete 41
    //works: treeDelete(binaryTree2, newNode_7); //delete 45
    //works: treeDelete(binaryTree2, newNode_6); //delete 34
    //works2: rotate_left(binaryTree2, newNode_2);
    //works3: rotate_left(binaryTree2, newNode_7);
    //works4: rotate_left(binaryTree2, newNode_6);
    rotate_left(binaryTree2, newNode_8);
    rotate_right(binaryTree2, newNode_13);
    rotate_right(binaryTree2, newNode_6);

    inorderTreeWalk(binaryTree2);
    //works: printf("\nAfter deleting 41, 45, 34\n");
    //works2: printf("\nAfter rotating left 41\n");
    //works3: printf("\nAfter rotating left 45\n");
    //works4: printf("\nAfter rotating left 34\n");
    printf("\nAfter rotating left and right various\n");

    cAndb lcs_struct_inst;
    string s1 = "ABCBDAB";
    int s1_length = s1.length(); //preferably pass directly known one sinc this one is in bytes..
    string s2 = "BDCABA";
    int s2_length = s2.length();
    printf("\ns1 length = %d and s2 length = %d\n", s1_length, s2_length);
    lcs_struct_inst = lcs_length(s1, s1_length, s2, s2_length);
    printf("\nAbout to oprint LCS\n");
    print_lcs(lcs_struct_inst.b_in, s1, s1_length, s2_length);

    s1 = "ACCGGTCGAGTGCGCGGAAGCCGGCCGAA";
    s2 = "GTCGTTCGGAATGCCGTTGCTCTGTAAA";
    printf("\ns1 length = %d and s2 length = %d\n", s1.length(), s2.length());
    lcs_struct_inst = lcs_length(s1, s1.length(), s2, s2.length());
    printf("\nAbout to oprint LCS\n");
    print_lcs(lcs_struct_inst.b_in, s1, s1.length(), s2.length());


    graph graph_inst(6);

    //vertex* v0 = new vertex(0, 0); //nothing reachable from this one say
    //vertex* v1 = new vertex(1, 1); //undirected here assumed so from 1 reach 
    vertex* v11 = new vertex(2, 1); //2
    vertex* v12 = new vertex(5, 1); //and 5
    //vertex* v2 = new vertex(2, 2);
    vertex* v22 = new vertex(1, 2); //and vice versa, i.e. from 2 reach 1
    vertex* v23 = new vertex(5, 2);
    vertex* v24 = new vertex(3, 2);
    vertex* v25 = new vertex(4, 2);
    //vertex* v3 = new vertex(3, 3);
    vertex* v33 = new vertex(2, 3);
    vertex* v34 = new vertex(4, 3);
    //vertex* v4 = new vertex(4, 4);
    vertex* v44 = new vertex(2, 4);
    vertex* v45 = new vertex(5, 4);
    vertex* v46 = new vertex(3, 4);
    //vertex* v5 = new vertex(5, 5);
    vertex* v55 = new vertex(4, 5);
    vertex* v56 = new vertex(1, 5); //and from 5 reach 1
    vertex* v57 = new vertex(2, 5);


    //graph_inst.addVertex(v0);
    //graph_inst.addVertex(v1);
    graph_inst.addVertex(v11);
    graph_inst.addVertex(v12);
    //graph_inst.addVertex(v2);
    graph_inst.addVertex(v22);
    graph_inst.addVertex(v23);
    graph_inst.addVertex(v24);
    graph_inst.addVertex(v25);
    //graph_inst.addVertex(v3);
    graph_inst.addVertex(v33);
    graph_inst.addVertex(v34);
    //graph_inst.addVertex(v4);
    graph_inst.addVertex(v44);
    graph_inst.addVertex(v45);
    graph_inst.addVertex(v46);
    //graph_inst.addVertex(v5);
    graph_inst.addVertex(v55);
    graph_inst.addVertex(v56);
    graph_inst.addVertex(v57);

    printf("\n");
    //bfs(graph_inst, 1);
    //printf("\n");
    //print_adjList(graph_inst, 1);
    bfs2(graph_inst, v11);
    printf("\nFrom 2 to 2 based on vertex->getVal\n");
    print_shortest_path(graph_inst, v11, v44); 
    printf("\nFrom 2 to 5 based on vertex->getVal\n");
    print_shortest_path(graph_inst, v11, v23); 
    
    printf("\nbelow two maybe dont work since even though from same vertex logically, BFS was run from a different one.. let's see!\n");
    printf("\nfrom 2 to 2 based on vertex->getVal\n");
    print_shortest_path(graph_inst, v33, v11); 
    printf("\nFrom 1 to 4 based on vertex->getVal\n");
    print_shortest_path(graph_inst, v56, v34); //from 1 to 4 
    printf("\nabove two dont work - no path - ok\n");
    
    bfs2(graph_inst, v22);
    printf("\nFrom 1 to 4 based on vertex->getVal\n");
    print_shortest_path(graph_inst, v22, v55); //from 1 to 4 
    printf("\nFrom 1 to 4 based on vertex->getVal using another set of vertex dest\n");
    print_shortest_path(graph_inst, v22, v25); //from 1 to 4 

    printf("\nle me try based on arr val rather than getVal.\n");
    print_shortest_path(graph_inst, v22, v44); //from 1 to 4 

    bfs2(graph_inst, v56);
    printf("\n5,1 -> 3,4.\n");
    print_shortest_path(graph_inst, v56, v46); 

    //Printinting path should think of as for below: v=2 that is adjacent to 1 (2,1) to v=3 that is adjacent to 2(3,2)
    bfs2(graph_inst, v11);
    printf("\n2,1->3,2\n");
    print_shortest_path(graph_inst, v11, v24); //prints 1(0) 2(1)  3(2)

    //v=1 that is adacent to 2 (1,2) to 3 that is adjancent to 2 (3,2)
    //in general, print path goes from dest vector, traversing through its predecessor, so if pointed not in the shortest direction, will not get shortest,
    //as is seen in the below two casses.
    //book treats vertices it seems sometimes as just numbers i guess and not as (v,u), i.e. vertex and its pred. 
    //So except mathetmically, i don't see how print path can always give the shortest if we think of verties as just single points.
    bfs2(graph_inst, v22);
    printf("\n1,2->3,2\n");
    print_shortest_path(graph_inst, v22, v24); //prints: 1(0) 2(1)  3(2)
    printf("\n1,2->3,4\n");
    print_shortest_path(graph_inst, v22, v46); //prints: 1(0) 2(1)  4(2)  3(3)


    bfs2(graph_inst, v56);
    printf("\n1,5->3,2\n");
    print_shortest_path(graph_inst, v56, v24); //prints: 1(0) 2(1)  3(2)


    dfs(graph_inst);


    graph graph_inst_directed(7); //u = 1, v = 2, w = 3, x = 4, y = 5, z = 6 . u->x/v; v->y; x->v; y->x; w->y/z; z->z

    //vertex* v0 = new vertex(0, 0); //nothing reachable from this one say
    //vertex* v1 = new vertex(1, 1); //undirected here assumed so from 1 reach 
    vertex* v1 = new vertex(4, 1); 
    vertex* v2 = new vertex(2, 1); 

    vertex* v3 = new vertex(5, 2);

    vertex* v4 = new vertex(5, 3);
    vertex* v5 = new vertex(6, 3);

    vertex* v6 = new vertex(2, 4);

    vertex* v7 = new vertex(4, 5);

    vertex* v8 = new vertex(6, 6);


    graph_inst_directed.addVertex(v1);
    graph_inst_directed.addVertex(v2);
    graph_inst_directed.addVertex(v3);
    graph_inst_directed.addVertex(v4);
    graph_inst_directed.addVertex(v5);
    graph_inst_directed.addVertex(v6);
    graph_inst_directed.addVertex(v7);
    graph_inst_directed.addVertex(v8);

    printf("\nRunning dfs on directed graph and then printing all vertices..\n");
    printf("u = 1, v = 2, w = 3, x = 4, y = 5, z = 6 . u->x/v; v->y; x->v; y->x; w->y/z; z->z\n");
    dfs(graph_inst_directed);
    dfs_printAll(graph_inst_directed);


    exercise_vectors();
    exercise_map();

    cout << "THE MOST COMMOND WORD MINUS BANNED IS: "<<most_common_word("def") << endl;

    threesome();

    threeSum();
    twoSum();
    twoSum_hash();

    vector<int> numsv;
    numsv.push_back(4);
    numsv.push_back(5);
    numsv.push_back(8);
    numsv.push_back(2);
    KthLargest kInst(3, numsv); //once constructed object and pushed 4th element, 2, removed this 2 since it's the lowest and stayed at the top of the heap.
    cout << "Adding 3 and getting: " << kInst.add(3) << endl; //returns 4 - since had 4,5,8 and adding 3 will just remove 3 and output kth largest, i.e. 4
    cout << "Adding 3 and getting: " << kInst.add(5) << endl; //returns 5 - had 4,5,8 and adding 5 will push 4 out and remaining 5,5,8 the kth largest is 5.
    cout << "Adding 3 and getting: " << kInst.add(10) << endl; //returns 5 - had 5,5,8 and adding 10 will push 5 out and kth largest will be 5.
    cout << "Adding 3 and getting: " << kInst.add(9) << endl; //returns 8 - had 5,8,10 and adding 9 removes 5 and kth will be 8.
    cout << "Adding 3 and getting: " << kInst.add(4) << endl; //returns 8 - etc.


    ListNode* l1_1 = new ListNode(4);
    ListNode* l1_2 = new ListNode(2, l1_1);
    ListNode* l1_3 = new ListNode(1, l1_2);

    ListNode* travrs_l1 = l1_3;
    cout << "L1 traversal start" << endl;
    while (travrs_l1 != nullptr) {
        cout << travrs_l1->val << " ";
        travrs_l1 = travrs_l1->next;
    }
    cout << endl<<"L1 traversal end" << endl;

    ListNode* l2_1 = new ListNode(4);
    ListNode* l2_2 = new ListNode(3, l2_1);
    ListNode* l2_3 = new ListNode(1, l2_2);

    travrs_l1 = l2_3;
    cout << "L2 traversal start" << endl;
    while (travrs_l1 != nullptr) {
        cout << travrs_l1->val << " ";
        travrs_l1 = travrs_l1->next;
    }
    cout << endl<<"L2 traversal end" << endl;

    ListNode* merged = mergeTwoLists(l1_3, l2_3);

    travrs_l1 = merged;
    cout << "Merged traversal start" << endl;
    while (travrs_l1 != nullptr) {
        cout << travrs_l1->val << " ";
        travrs_l1 = travrs_l1->next;
    }
    cout <<endl<< "Merged traversal end" << endl;

    vector<vector<int>> intervals;
    vector<int> int1; 
    int1.push_back(7); 
    int1.push_back(9); 
    intervals.push_back(int1);
    vector<int> int2;
    int2.push_back(1);
    int2.push_back(4);
    intervals.push_back(int2);
    vector<int> int3;
    int3.push_back(2);
    int3.push_back(5);
    intervals.push_back(int3);

    if (canAttendMeetings(intervals)) cout << "Can attend meetings" << endl;
    else cout << "CAN NOT attend meetings" << endl;

    vector<int>v = { 1,2,3,3,4,5 };
    cout << "Lenght of longst subarray=" << lengthOfLongestSubarray(v) << endl;


    Graph_t g(6);
    g.addEdge(0, 1);
    g.addEdge(2, 0);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 3);
    g.addEdge(4, 5);

    if (g.isConnected())
        cout << "The Graph is Connected" << endl;
    else
        cout << "The Graph is not Connected" << endl;
    cout << "Following are strongly connected components in above graph \n";
    g.printSCCs();

    Graph_t g1(5);
    g1.addEdge(0, 1);
    g1.addEdge(1, 2);
    g1.addEdge(2, 3);
    g1.addEdge(3, 0);
    g1.addEdge(2, 4);
    g1.addEdge(4, 2);
    if (g1.isConnected())
        cout << "The Graph is Connected" << endl;
    else
        cout << "The Graph is not Connected" << endl;

    cout << "Following are strongly connected components in given graph \n";
    g1.printSCCs();


    //*****island problem - need to impekentn vertex and edge setting and thenjust call the algorithms (DFS, SCC, etc)
    //assuming problem is presented as a matrix filled with 1's (land) and 0's (water)
    int rows = 2;
    int cols = 3;
    if (rows < 1 || cols < 1) {
        cerr << "Invalid matrix size, quitting" << endl;
        return -1;
    }
    int** mtx = new int*[rows];
    for (int i = 0; i < rows; i++) 
        mtx[i] = new int[cols];
    cout << "fill matrix with random 0's and 1's" << endl;
    //uncomment below for nadomziation - and keep seed2=12345 commented
    time_t seed2 = time(NULL);
    //seed2 = 12345;
    srand(seed2);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            mtx[i][j] = rand() % 2; 

    cout << "print resultsnt matrix that will be used as input" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            cout << " " << mtx[i][j] << " ";
        cout << endl;
    }
    cout << "Now traverse matrix and add edges" << endl;
    Graph_t island(rows * cols);
    for (int i = 0; i < rows - 1; i++) {
        for (int j = 0; j < cols - 1; j++) {
            //seaerch right
            if (mtx[i][j] == 1 && mtx[i][j + 1] == 1) {
                island.addEdge(i * cols + j, i * cols + j + 1);
                cout << "Adding edge from " << i * cols + j << " to " << i * cols + j + 1 << endl;
            }
            //and search down
            if (mtx[i][j] == 1 && mtx[i + 1][j] == 1) {
                island.addEdge(i * cols + j, (i+1) * cols + j);
                cout << "Adding edge from " << i * cols + j << " to " << (i + 1) * cols + j << endl;
            }
        }
    }
    int i = rows - 1;
    //go throuw last row
    for (int j = 0; j < cols - 1; j++) {
        if (mtx[i][j] == 1 && mtx[i][j + 1] == 1) {
            island.addEdge(i * cols + j, i * cols + j + 1);
            cout << "Adding edge from " << i * cols + j << " to " << i * cols + j + 1 << endl;
        }
    }
    //go through last col
    int j = cols - 1;
    for (int i = 0; i < rows - 1; i++) {
        if (mtx[i][j] == 1 && mtx[i + 1][j] == 1) {
            island.addEdge(i * cols + j, (i + 1) * cols + j);
            cout << "Adding edge from " << i * cols + j << " to " << (i + 1) * cols + j << endl;
        }
    }

    cout << "Following are strongly connected components in THE ISLAND (graph is undirected)\n";
    island.printSCCs();
    /*
    cout << " And now, the single itemed strongly conencted compoenents" << endl;
    for (auto i : island.sccs) {
        if (i.size() == 1) {
            cout << i[0] << endl;
        }
    }
    */
    cout << "DONE" << endl;

    for (auto i : island.sccs) {
        if (i.size() == 1) {
            //check if mtx is 1 for this and if so, it's another island; otherwise it is not.
            //convert absolute number to row/col
            if (mtx[i[0] / cols][i[0] % cols] == 1) {
                //cout << "location: [" << i[0] / cols << "]" << "[" << i[0] % cols << "] is 1 !" << endl;
                island.islands++;
            }
            else {
                //cout << "location: [" << i[0] / cols << "]" << "[" << i[0] % cols << "] is 0 !" << endl;
            }
        }
    }


    cout << "We have " << island.islands << " islands !" << endl;

    return 0;
}

//S1: ACCGGTCGAGTGCGCGGAAGCCGGCCGAA
//S2: GTCGTTCGGAATGCCGTTGCTCTGTAAA
//S3: GTCGTCGGAAGCCGGCCGAA
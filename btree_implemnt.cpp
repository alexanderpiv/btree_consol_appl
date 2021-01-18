#include <iostream>
#include <assert.h>

#include "sorts.h"
#include "heap.cpp"

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

    return 0;
}


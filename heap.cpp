#include <iostream>
using namespace std;

class bin_heap {
public:
	bin_heap(int size) {
		heapArr = new int[size];
		heap_size = size;
		/*
		//temp way to isnert elements
		heapArr[1] = 16;
		heapArr[2] = 4;
		heapArr[3] = 10;
		heapArr[4] = 14;
		heapArr[5] = 7;
		heapArr[6] = 9;
		heapArr[7] = 3;
		heapArr[8] = 2;
		heapArr[9] = 8;
		heapArr[10] = 1;
		heap_fill = 10;
		*/
		heapArr[1] = 4;
		heapArr[2] = 1;
		heapArr[3] = 3;
		heapArr[4] = 2;
		heapArr[5] = 16;
		heapArr[6] = 9;
		heapArr[7] = 10;
		heapArr[8] = 14;
		heapArr[9] = 8;
		heapArr[10] = 7;
		heap_fill = 10;
	}
	int getSize() {
		return heap_fill;
	}
	int getFill() {
		return heap_fill;
	}
	~bin_heap() {
		delete[] heapArr;
	}

	//!!!below 3 funcs assume i starts at 1 while here it start with 0 due to array indices starting at 1, so later adjust code below.
	int parent(int i) {
		return i / 2;
	}
	int left(int i) {
		return 2 * i;
	}
	int right(int i) {
		return 2 * i + 1;
	}

	//below is actually incorrect although happened to produce correct result for given input.
	//It is not correct since i should be checked with BOTH left AND right and then pick the biggest of i,left,right.
	void max_heapify(int i) {
		if (left(i) < heap_fill + 1 && heapArr[i] < heapArr[left(i)]) {
			int tmp = heapArr[left(i)];
			heapArr[left(i)] = heapArr[i];
			heapArr[i] = tmp;
			max_heapify(left(i));
		}
		else if (right(i) < heap_fill + 1 && heapArr[i] < heapArr[right(i)]) {
			int tmp = heapArr[right(i)];
			heapArr[right(i)] = heapArr[i];
			heapArr[i] = tmp;
			max_heapify(right(i));
		}
	}
	//book version
	//O(lgn)
	void max_heapify2(int i) {
		int largest;
		if (left(i) < heap_fill + 1 && heapArr[i] < heapArr[left(i)]) {
			largest = left(i);
		}
		else {
			largest = i;
		}
		if (right(i) < heap_fill + 1 && heapArr[largest] < heapArr[right(i)]) {
			largest = right(i);
		}
		if (largest != i) {
			int tmp = heapArr[largest];
			heapArr[largest] = heapArr[i];
			heapArr[i] = tmp;
			max_heapify2(largest);
		}
	}
	//O(n)
	void build_heap() {
		for (int i = heap_fill / 2; i > 0; i--) {
			max_heapify2(i);
		}
	}

	//O(nlgn) since build_heap() takes O(n) and each of the n-1 calls to max_heapify take O(lgn)
	void heapsort() {
		build_heap();
		printf("inside heapsort after build_heap\n");
		print_arr();
		int heap_fill_rec = heap_fill;
		for (int i = heap_fill; i >= 2; i--) {
			int tmp = heapArr[i];
			heapArr[i] = heapArr[1];
			heapArr[1] = tmp;
			heap_fill--;
			max_heapify2(1);
		}
		heap_fill = heap_fill_rec;
	}

	//O(1)
	int heap_maximum() {
		return heapArr[1];
	}

	//O(lgn)
	int heap_extract_maximum() {
		if (heap_fill < 1) {
			printf("Error: Heap underflow\n");
		}
		else {
			int max = heapArr[1];
			heapArr[1] = heapArr[heap_fill];
			heap_fill--;
			max_heapify2(1);
			return max;
		}
	}

	//elemnt x , i.e. arry index x
	//below is my solution that works but will be O(n).
	void heap_increase_key(int x, int k) {
		heapArr[x] = k;
		build_heap();
	}

	//O(logn) since the path traced from the node update (heapArr[x]=k) to the root has length O(lgn).
	void heap_increase_key2(int x, int k) {
		if (k < heapArr[x]) {
			printf("Error: new key is smaller than existing key\n");
		}
		else {
			heapArr[x] = k; //needed in case modifying root as first run (since then while loop wont enter).
			while (parent(x) > 0 && heapArr[parent(x)] < heapArr[x]) { //or x>1 for first condition
				heapArr[x] = heapArr[parent(x)];
				heapArr[parent(x)] = k;
				//heap_increase_key(parent(x), k);
				//below is even better than above...
				x = parent(x);
			}
		}
	}

	//O(lgn)
	void heap_insert(int key) {
		heap_fill++;
		heapArr[heap_fill] = -1; //should be the minimal no valid value so that can run the next func on it safely..
		heap_increase_key2(heap_fill, key);
	}

	void print_arr() {
		for (int i = 1; i < heap_fill + 1; i++) {
			cout << "idx = " << i << " and elem = " << heapArr[i] << endl;
		}
		cout << endl;
	}
private:
	int* heapArr;
	int heap_fill;
	int heap_size;
};


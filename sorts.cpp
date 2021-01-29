#include <iostream>
using namespace std;

//we start with storing 2nd element and comparing first with it. Uf furst us bigger, we assign second with this bigger first.
//In general, we take nth (ith) item and if it's bigger than the intially stored key (that we are trying to sort), 
//	we move the bigger key to the right, making space on the left , at some point, for lower key to be inserted.
//At the end we put at the sopped position the key.
//COmplexity: 1+2+3+4+...+n = n/2(1+n) = O(n^2) (Gauss' formula: (n / 2)(first number + last number) = sum, where n is the number of integers.)
void insertion_sort(int* arr, int arr_sz) {
	for (int j = 1; j < arr_sz; j++) {
		int key = arr[j];
		int i = j - 1; 
		while (i >= 0 && arr[i] > key) {
			arr[i + 1] = arr[i];
			i = i - 1;
		}
		arr[i + 1] = key;
	}
}

void print_arr(int* arr, int arr_sz) {
	for (int i = 0; i < arr_sz; i++) {
		cout << "idx = " << i <<" and elem = "<< arr[i] << endl;
	}
}


//cmioplexity: O(nlgn) lg is log base 2 n
void merge_sort(int* arr, int left, int right) {
	if (left < right) {
		merge_sort(arr, left, left + (right-left) / 2);
		merge_sort(arr, left + (right-left) / 2 + 1, right);
		
		//Below is Merge implementation.
		int idxL = 0;
		int idxR = 0;
		int* arrLeft = new int[(right - left) / 2 + 1];
		int* arrRight = new int[(right - left) / 2 + 1];

		for (int i = left; i <= left + (right-left) / 2; i++) {
			arrLeft[idxL] = arr[i];
			idxL++;
		}
		for (int i = left + (right - left) / 2 + 1; i <= right; i++) {
			arrRight[idxR] = arr[i];
			idxR++;
		}

		int idxL_cntr = 0;
		int idxR_cntr = 0;

		for (int i = left; i <= right; i++) {
			if (idxL_cntr < idxL && idxR_cntr < idxR) {
				if (arrLeft[idxL_cntr] > arrRight[idxR_cntr]) {
					arr[i] = arrRight[idxR_cntr];
					idxR_cntr++;
				}
				else {
					arr[i] = arrLeft[idxL_cntr];
					idxL_cntr++;
				}
			}
			else if (idxL_cntr < idxL) {
				arr[i] = arrLeft[idxL_cntr];
				idxL_cntr++;
			}
			else {
				arr[i] = arrRight[idxR_cntr];
				idxR_cntr++;
			}
		}

		delete [] arrLeft;
		delete [] arrRight;
	}
}

//5,2,4,1 -> 2,5,4,1->2,4,5,1->2,4,1,5. 2,1,4,5. 1,2,4,5
void insertion_sort2(int* arr, int arr_sz) {
	for (int i = 0; i < arr_sz; i++) {
		int j = i;
		while (arr[j + 1] < arr[j] && j >= 0) {
			//swap and keep comparing
			int tmp = arr[j];
			arr[j] = arr[j + 1];
			arr[j + 1] = arr[j];
			j--;
		}
	}
}

//5,2,4,1 -> 2,5,4,1->2,4,5,1 . 2,4,5,5. 2,4,4,5. 2,2,4,5. 
//or take above 2 and merge with imrpeovment of book for doing only one assignment inside while loop..
//AT THE END, below is not a merge of my sort2 but rather a different way to do the same - where reducing by one asignment. It is a big deal..
void insertion_sort3(int* arr, int arr_sz) {
	for (int i = 0; i < arr_sz; i++) {
		int j = i;
		int key = arr[j + 1];
		while (key < arr[j] && j >= 0) {
			//swap and keep comparing
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j+1] = key;
	}
}

int partition(int* arr, int left, int right) {
	int pivot = arr[right]; //or can be a random one from arr like: int pivot = rand() % right - left; <-haven't verified but looks ok..
	int i = left - 1;
	//advance i and swap j with i+1 if current element(arr[j]) is less than pivot
	for (int j = left; j < right; j++) {
		if (arr[j] < pivot) {
			int tmp = arr[j];
			arr[j] = arr[i + 1];
			arr[i + 1] = tmp;
			i++;
		}
	}
	arr[right] = arr[i + 1];
	arr[i + 1] = pivot;
	return i + 1;
}

//O(nlgn) usually but worst is O(n^2) - to get the usual case need to ahve all values unique (ideally) and also should be random order inside (ideally)
void quicksort(int* arr, int left, int right) {
	if (left < right) {
		int q = partition(arr, left, right);
		quicksort(arr, left, q - 1);
		quicksort(arr, q + 1, right);
	}
}

//O(k)+O(n)+O(k)+O(n) = O(k+n)
void counting_sort(int* arr, int n, int k) {
	int* count_arr = new int[k];
	int* sorted_arr = new int[n];
	for (int i = 0; i < k; i++) {
		count_arr[i] = 0; //initialize count array
	}
	//go through input array and count how many times each value is seen
	for (int i = 0; i < n; i++) {
		count_arr[arr[i]] = count_arr[arr[i]] + 1;
	}
	//modify count_arr to hold prior value plus how many times lower values have been seen
	for (int i = 1; i < k; i++) {
		count_arr[i] = count_arr[i] + count_arr[i - 1];
	}
	//the key operation now
	for (int i = n-1; i >= 0; i--) {
		sorted_arr[count_arr[arr[i]] - 1] = arr[i]; //the -1 is to place sorted vals in location 0 to n-1 rather than 1 to n. 
													//count_arr may have 0 values but they won't be called since we call base on arr[i] and if
													//arr[i] doesnt have a ceratin value, we won't reference it.
		count_arr[arr[i]] = count_arr[arr[i]] - 1; //so that repeating numbers go into decreasing locations and not over the written one
	}

	//write back sorted array into original input arr
	for (int i = 0; i < n; i++) {
		arr[i] = sorted_arr[i];
	}
}

//another take at mergesort..trying to see if can do similar to book myself..
void merge(int* Arr, int p, int r) {
	int* arr_p = new int[(r - p)/2 + 1];
	int* arr_r = new int[(r - p)/2 + 1]; //will have one extra location here when p is one more than r but that's an insignificant memry misuage
	int p_ptr = 0;
	int r_ptr = 0;
	int Arr_ptr = p;

	for (int i = 0; i <= (r - p)/2; i++) {
		arr_p[i] = Arr[Arr_ptr];
		Arr_ptr++;
	}
	for (int i = 0; i < r - p - (r - p)/2; i++) {
		arr_r[i] = Arr[Arr_ptr];
		Arr_ptr++;
	}
	Arr_ptr = p;
	for (int i = p; i <= r; i++) {
		//OPTIMIZATION: add a last extra element in arr_p and arr_r and give it a max value that normal values cannot achieeve. 
		//Then can skips all the checks for empty piles and simply compare left with right from p to r.
		if (p_ptr <= (r - p) / 2 && r_ptr < r - p - (r - p) / 2) {
			if (arr_p[p_ptr] < arr_r[r_ptr]) {
				Arr[i] = arr_p[p_ptr];
				p_ptr++;
			}
			else {
				Arr[i] = arr_r[r_ptr];
				r_ptr++;
			}
		}
		else if (p_ptr <= (r - p) / 2) {
			Arr[i] = arr_p[p_ptr];
			p_ptr++;
		}
		else if (r_ptr < r - p - (r - p) / 2) { //can just have else without the further qualificationi since it's the only remainign option
			Arr[i] = arr_r[r_ptr];
			r_ptr++;
		}
	}
	delete [] arr_p;
	delete [] arr_r;
}

//optimized verison with sentinel, but need also to modify left and right sizes to be exact.
void merge_opt(int* Arr, int p, int r) {
	int* arr_p;
	int* arr_r;
	if ((r-p) % 2 == 1) {
		arr_p = new int[(r - p) / 2 + 1 + 1];
		arr_r = new int[(r - p) / 2 + 1 + 1]; //will have one extra location here when p is one more than r but that's an insignificant memry misuage
		arr_p[(r - p) / 2 + 1] = INT_MAX;
		arr_r[(r - p) / 2 + 1] = INT_MAX;
	}
	else {
		arr_p = new int[(r - p) / 2 + 1 + 1];
		arr_r = new int[(r - p) / 2 + 1]; //will have one extra location here when p is one more than r but that's an insignificant memry misuage
		arr_p[(r - p) / 2 + 1] = INT_MAX;
		arr_r[(r - p) / 2] = INT_MAX;
	}
	int p_ptr = 0;
	int r_ptr = 0;
	int Arr_ptr = p;

	for (int i = 0; i <= (r - p) / 2; i++) {
		arr_p[i] = Arr[Arr_ptr];
		Arr_ptr++;
	}

	for (int i = 0; i < r - p - (r - p) / 2; i++) {
		arr_r[i] = Arr[Arr_ptr];
		Arr_ptr++;
	}
	Arr_ptr = p;
	for (int i = p; i <= r; i++) {
		//OPTIMIZATION: add a last extra element in arr_p and arr_r and give it a max value that normal values cannot achieeve. 
		//Then can skips all the checks for empty piles and simply compare left with right from p to r.
		if (arr_p[p_ptr] < arr_r[r_ptr]) {
			Arr[i] = arr_p[p_ptr];
			p_ptr++;
		}
		else {
			Arr[i] = arr_r[r_ptr];
			r_ptr++;
		}
	}
	delete[] arr_p;
	delete[] arr_r;
}

void mergesort(int* Arr, int p, int r) {
	if (p < r) {
		mergesort(Arr, p, (r-p)/2 + p);
		mergesort(Arr, (r-p)/2 + p + 1, r);
		merge_opt(Arr, p, r);
	}
}

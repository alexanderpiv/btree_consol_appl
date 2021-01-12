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

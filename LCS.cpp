#include <string>
using namespace std;

struct cAndb {
	int** b_in;
	int** c_in;
};

//Longest common subsequence = LCS

cAndb lcs_length(string x, int x_length, string y, int y_length) {
	//int b[x_length][y_length];
	//int c[x_length + 1][y_length + 1];
	int** b = new int* [x_length]; //values: 1 means up+left, 2 means up, 3 means left
	int** c = new int* [x_length + 1];
	for (int i = 0; i < x_length; i++) {
		b[i] = new int[y_length];
	}
	for (int i = 0; i < x_length + 1; i++) {
		c[i] = new int[y_length + 1];
	}

	//c has extra row and column for storing 0's in there to allow for easy substraction of i=1 and all j and j=1 with all i
	for (int i = 0; i < x_length+1; i++) {
		c[i][0] = 0;
	}
	for (int i = 0; i < y_length+1; i++) {
		c[0][i] = 0;
	}

	//b below i subtract -1 since array goes from 0..
	for (int i = 1; i <= x_length; i++) {
		for (int j = 1; j <= y_length; j++) {
			if (x[i-1] == y[j-1]) { //string starts at index 0 as well
				c[i][j] = c[i - 1][j - 1] + 1;
				b[i-1][j-1] = 1;
			}
			else if (c[i - 1][j] >= c[i][j - 1]) {
				c[i][j] = c[i - 1][j];
				b[i-1][j-1] = 2;
			}
			else {
				c[i][j] = c[i][j - 1];
				b[i-1][j-1] = 3;
			}
		}
	}

	cAndb cAndb_inst;
	cAndb_inst.b_in = b;
	cAndb_inst.c_in = c;
	return cAndb_inst;
}

void print_lcs(int** b, string x, int x_length, int y_length) {
	if (x_length == 0 || y_length == 0) {
		return;
	}
	//b max val is [x_length-1][y_length-1] since starts at index 0..
	if (b[x_length-1][y_length-1] == 1) {
		print_lcs(b, x, x_length - 1, y_length - 1);
		printf("%c ", x[x_length-1]); //string also starts at index 0
	}
	else if (b[x_length-1][y_length-1] == 2) {
		print_lcs(b, x, x_length - 1, y_length);
	}
	else {
		print_lcs(b, x, x_length, y_length - 1);
	}
}
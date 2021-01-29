#pragma once
#include <string>
using namespace std;

struct cAndb {
	int** b_in;
	int** c_in;
};

cAndb lcs_length(string x, int x_length, string y, int y_length);
void print_lcs(int** b, string x, int x_length, int y_length);

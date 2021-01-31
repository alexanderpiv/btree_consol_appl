#pragma once
#include <string>
#include <vector>
#include <map>
#include <stdexcept> //for cerr and other eceptions - not used currently
#include <regex>
#include <algorithm> //for sort function
#include <functional> //for hash function
#include <numeric> //for accumulate
#include <unordered_set>

using namespace std;

//Sort based on last name, i.e. the first string, preceding the comma

void exercise_vectors() {
	string s1 = "wu, john";
	string s2 = "McNough, jack";
	string s3 = "Dynl, Bob";
	string s5 = "smith, bob";
	string s4 = "main, greg";
	vector<string> vec_str;

	vec_str.push_back(s1);
	vec_str.push_back(s2);
	cout << "vec_str.back()=" << vec_str.back() << endl;
	vec_str.at(1) = s5; //Returns a reference to the element at position n in the vector.
	cout << "vec_str.back()=" << vec_str.back() << endl;
	vec_str.back() = s3; //Returns a reference to the last element in the vector.
	cout << "vec_str.back()=" << vec_str.back() << endl;
	vector<string>::iterator vec_it = vec_str.end() - 1; //"end" Returns an iterator referring to the past-the-end element in the vector container
	*vec_it = s4;
	cout << "vec_str.back()=" << vec_str.back() << endl;
	for (size_t i = 0; i < vec_str.size(); i++) { //typedef unsigned int size_t - use size_t which is simply unsigned int since size() func returns size_t, since size is never negative.
		cout << "1vec " << i << " = " << vec_str[i] << endl;
		cout << "2vec " << i << " = " << vec_str.at(i) << endl; //better to use this vs [] since it will check bounds and throw exception if out of range (will need to catc exception but still..)
	}
	for (vector<string>::iterator it = vec_str.begin(); it != vec_str.end(); ++it) {
		cout << "3vec " << " = " << *it << endl;
	}

	//Because vectors use an array as their underlying storage, INSERTING ELEMENTS IN POSITIONS OTHER THAN THE VECTOR END causes the container 
	//to relocate all the elements that were after position to their new positions.This is generally an inefficient operation compared to the one 
	//performed for the same operation by other kinds of sequence containers(such as list or forward_list).
	vec_it = vec_str.begin();
	vec_it = vec_str.insert(vec_it, "abc"); //insert returns a pointer to the first element of the element(s) inserted
	for (size_t i = 0; i < vec_str.size(); i++) {
		cout << "4vec " << i << " = " << vec_str.at(i) << endl;
	}
	//insert can insert a range or do a fill too. Here is range ex:  myvector.insert (it+2,anothervector.begin(),anothervector.end());

	// constructors used in the same order as described above:
	std::vector<int> first;                                // empty vector of ints
	std::vector<int> second(4, 100);                       // four ints with value 100
	std::vector<int> third(second.begin(), second.end());  // iterating through second
	std::vector<int> fourth(third);                       // a copy of third

	// the iterator constructor can also be used to construct from arrays:
	int myints[] = { 16,2,77,29 };
	std::vector<int> fifth(myints, myints + sizeof(myints) / sizeof(int));

}

bool sort_strings_regx(string s1, string s2) {
	regex regx("(\\w+),\\s+(\\w+)");
	smatch mtch1;
	smatch mtch2;
	regex_search(s1, mtch1, regx);
	regex_search(s2, mtch2, regx);
	if (mtch1.size() == 3 && mtch2.size() == 3) {
		if (mtch1[2].compare(mtch2[2]) == 0) { //if equal , compare last names
			if (mtch1[1].compare(mtch2[1]) < 0) { //if equal , doesnt matter what to return
				return true;
			}
			else {
				return false;
			}
		}
		else if (mtch1[2].compare(mtch2[2]) < 0) {
			return true;
		} 
		else {
			return false;
		}
	}
	else {
		cerr << "Input is wrong format" << endl;
		return false;
	}
}

void exercise_map() {
	string s1 = "wu, john";
	string s2 = "McNough, jack";
	string s3 = "Jynl,  Bob";
	string s5 = "smith, bob";
	string s4 = "main,  greg";
	string s6 = "Dunior, Bob";
	map<string, string> map_LastFirst;

	string firstName = s1.substr(s1.find(","), s1.size()); // size/length give num of bytes..assuming each char is encoded with 1 byte..
	cout << "first name is: " << firstName << endl;
	
	//operator[] - Return match(public member function)
	//prefix - Return prefix(public member function)
	//suffix - Return suffix(public member function)
	//length - Return length of match(public member function)
	//position - Return position of match(public member function)
	//str - Return match as a string(public member function)

	regex regx("(\\w+),\\s+(\\w+)");
	smatch mtch;
	cout << "MATCHING START" << endl;
	regex_search(s1, mtch, regx);
	for (unsigned i = 0; i < mtch.size(); ++i) {
		std::cout << "match " << i << ": " << mtch[i];
		std::cout << " (with a length of " << mtch[i].length() << ")\n";
	}

	vector<string> vec_str;
	vec_str.push_back(s1);
	vec_str.push_back(s2);
	vec_str.push_back(s3);
	vec_str.push_back(s4);
	vec_str.push_back(s5);
	vec_str.push_back(s6);

	vector<string> vec_first;
	map<string, string> map_firstToRemain; 
	//sort on first name and then on last name (if first names the same). Handle same first names.
	hash<string> hash_str;
	size_t hash_rslt;

	for (auto vec_it = vec_str.begin(); vec_it != vec_str.end(); ++vec_it) {
		regex_search(*vec_it, mtch, regx);
		if (mtch.size() == 3) {
			map_LastFirst[mtch[1]] = mtch[2]; //key = last name = mtch[1] and value = first name = mtch[2]

			map_firstToRemain[mtch[2]] = mtch[0]; //key = first, value = the whole string
			vec_first.push_back(mtch[2]);

			//below doesnt halp since same key hashes to same value.
			hash_rslt = hash_str(mtch[2]);
			cout << "hash of:" << mtch[2] << " = " << hash_rslt << endl;
		}
	}
	cout << "printing map contents in some order" << endl;
	for (auto map_it = map_LastFirst.begin(); map_it != map_LastFirst.end(); ++map_it) {
		cout << map_it->first << "=" << map_it->second << endl;
	}

	cout << "before sorting first names" << endl;
	for (auto vec_it = vec_first.begin(); vec_it != vec_first.end(); ++vec_it) {
		cout << *vec_it << endl;
	}
	sort(vec_first.begin(), vec_first.end()); //Sorts the elements in the range [first,last) into ascending order. 
	//complexity: N log(N) if enough mem is avail. (N is num of elements) . The C++ standard does not specify which sorting algorithm to apply since this would be oftentimes and unnecessary implementation restriction.
	//The elements are compared using operator< for the first version, and comp for the second.
	//String's operator< use string::compare for the comparison.
	//string::compare returns 0 if equal, 
	//<0 if Either the value of the first character that does not match is lower in the compared string, or all compared characters match but the compared string is shorter.
	//>0 the opposite to above.
	//CAN PROVIDE FUNC or Class to SORT, ex: std::sort (myvector.begin()+4, myvector.end(), myfunction);
	//where: int myints[] = {32,71,12,45,26,80,53,33};
	//		std::vector<int> myvector(myints, myints + 8);
	//	bool myfunction (int i,int j) { return (i<j); }
	//The comparison function should return an equivalnt of bool, should not modify args and should accept two elements as args.

	cout << "after sorting first names" << endl;
	for (auto vec_it = vec_first.begin(); vec_it != vec_first.end(); ++vec_it) {
		cout << *vec_it << endl;
	}

	cout << "reersed order:" << endl;
	reverse(vec_first.begin(), vec_first.end());
	for (auto vec_it = vec_first.begin(); vec_it != vec_first.end(); ++vec_it) {
		cout << *vec_it << endl;
	}

	cout << "Now print original set of strings sorted on first name" << endl;
	reverse(vec_first.begin(), vec_first.end());
	for (auto vec_it = vec_first.begin(); vec_it != vec_first.end(); ++vec_it) {
		cout << map_firstToRemain[*vec_it] << endl;
	}
	

	cout << "Before Full string storting with extended comparison func" << endl;
	for (auto vec_it = vec_str.begin(); vec_it != vec_str.end(); ++vec_it) {
		cout << *vec_it << endl;
	}
	sort(vec_str.begin(), vec_str.end(), sort_strings_regx);
	cout << "After Full string storting with extended comparison func" << endl;
	for (auto vec_it = vec_str.begin(); vec_it != vec_str.end(); ++vec_it) {
		cout << *vec_it << endl;
	}

	/*for (auto x : mtch) {
		cout << x << endl;
	}*/
	/*while (regex_search(s1, mtch, regx)) {
		for (auto x : mtch) {
			cout << x << " ";
		}
		cout << endl;
		s1 = mtch.suffix().str();
	}*/
	cout << "MATCHING END" << endl;

	string s("this subject has a submarine as a subsequence");
	smatch m;
	regex e("\\b(sub)([^ ]*)");   // matches words beginning by "sub"

	cout << "Target sequence: " << s << endl;
	cout << "Regular expression: /\\b(sub)([^ ]*)/" << endl;
	cout << "The following matches and submatches were found:" << endl;

	while (regex_search(s, m, e)) {
		for (auto x : m) {
			cout << x << " ";
		}
		cout << std::endl;
		s = m.suffix().str();
	}

	//map_LastFirst[""]
}

//You are given an array of positive numbers from 1 to n, such that all numbers from 1 to n are present except one number x.You have to find x.The input array is not sorted.
//ex: n=8 ; 3,7,1,2,8,4,5 . Return 6, which is the missing number.
//complexiting should be n (for init) + n (for counting) + n (for checking count value) = 3n = O(n) , so linear in time (O(n) is better than O(nlgn)).
int find_missing(const vector<int>& input) {
	int* count_arr = new int[input.size() + 1];
	for (unsigned int i = 0; i <= input.size(); i++) {
		count_arr[i] = 0;
	}
	for (auto i = input.begin(); i != input.end(); ++i) {
		count_arr[*i] = 1;
	}
	for (unsigned int i = 1; i <= input.size(); i++) {
		if (count_arr[i] != 1) {
			return i;
		}
	}
}

//answer says compelity is O(n) which is similar to mine actually.. i mean in mine there are more constant factors but O is the same..
int find_missing_answr(const vector<int>& input) {
	// calculate sum of all elements 
	// in input vector
	auto sum_of_elements = std::accumulate(
		input.begin(), input.end(), 0);
	// There is exactly 1 number missing 
	int n = input.size() + 1;
	int actual_sum = (n * (n + 1)) / 2;
	return actual_sum - sum_of_elements;
}

void test(int n) {
	int missing_element = rand() % n + 1;
	vector<int> v;
	for (int i = 1; i <= n; ++i) {
		if (i == missing_element) {
			continue;
		}
		v.push_back(i);
	}
	int actual_missing = find_missing(v);
	cout << "Expected Missing = " << missing_element << " Actual Missing = " << actual_missing << endl;
}
/*
int main() {
	srand(time(NULL));
	for (int i = 1; i < 10; ++i)
		test(100000);

	return 0;
}
*/

//Given an array of integersand a value, determine if there are any two integers in the array whose sum is equal to the given value.Return true if the sum existsand return false if it does not.
//ex: 5,7,1,2,8,4,3 . Target sum = 10 so can use 7+3 or 2+8 so return true. If Target sum = 19 for instance, return false.
bool find_sum_of_two(vector<int>& A, int val) {
	map<int, bool> mapp;
	for (auto vec_it = A.begin(); vec_it != A.end(); ++vec_it) {
		mapp[*vec_it] = true;
		cout << "adding to map:" << *vec_it << endl;
	}
	for (auto vec_it = A.begin(); vec_it != A.end(); ++vec_it) {
		cout << "val=" << val << "; *vec_it=" << *vec_it << endl;
		//either use count which returns 1 if found or 0 otherwise
		if (mapp.count(val - *vec_it)) {
			cout << "and mapp has:" << val - *vec_it << endl;
			return true;
		}
		//OR use find which returns iterator to found location or end() otherwise.
		/*if (mapp.find(val - *vec_it) != mapp.end()) {
			cout<<"2and mapp has:"<<val - *vec_it<<endl;
			cout<<*vec_it<<endl;
			return true;
		}*/
	}
	return false;
}

//You are given a dictionary of words and a large input string. You have to find out whether the input string can be completely segmented into the words of a given dictionary. 
bool can_segment_string(string s, unordered_set<string>& dictionary) {
	for (auto set_iter = dictionary.begin(); set_iter != dictionary.end(); ++set_iter) {
		cout << *set_iter << endl;
	}
	cout << "search for: " << s << endl;

	//go thorugh each word in the dicstionsary and check if any matches a consecutive list of chars in s
	for (auto set_iter = dictionary.begin(); set_iter != dictionary.end(); ++set_iter) {
		//The position of the first character of the first match.
		//If no matches were found, the function returns string::npos.
		size_t found = s.find(*set_iter, 0);
		if (found != std::string::npos && found == 0) {
			cout << "first find=" << *set_iter << ". Next posiiotn = " << (*set_iter).length() << endl;
			for (auto set_iter2 = dictionary.begin(); set_iter2 != dictionary.end(); ++set_iter2) {
				size_t found2 = s.find(*set_iter2, (*set_iter).length());
				if (found2 != std::string::npos && found2 == (*set_iter).length()) {
					cout << "second find=" << *set_iter2 << endl;
					cout << *set_iter << " " << *set_iter2 << endl;
					return true;
				}
			}
		}
	}

	return false;
}

bool can_segment_string2(string s, unordered_set <string>& dictonary) {
	for (int i = 1; i <= s.length(); ++i) {
		string first = s.substr(0, i);
		if (dictonary.find(first) != dictonary.end()) {
			string second = s.substr(i);
			if (second.empty() || dictonary.find(second) != dictonary.end() || can_segment_string(second, dictonary)) {
				return true;
			}
		}
	}
	return false;
}
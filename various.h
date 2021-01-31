#pragma once
#include <string>
#include <vector>
#include <map>
#include <stdexcept> //for cerr and other eceptions - not used currently
#include <regex>
#include <algorithm> //for sort function
#include <functional> //for hash function

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

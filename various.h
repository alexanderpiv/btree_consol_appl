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
#include <cctype>  //for tolower
#include <set>
#include <unordered_map>

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
	//TODOOOOO: SHOULD EXCLUDE from val-*vec_it *vec_it since used it already !! unless that's a valid case.....
	map<int, bool> mapp;
	for (auto vec_it = A.begin(); vec_it != A.end(); ++vec_it) {
		mapp[*vec_it] = true;
		cout << "adding to map:" << *vec_it << endl;
	}
	for (auto vec_it = A.begin(); vec_it != A.end(); ++vec_it) {
		cout << "val=" << val << "; *vec_it=" << *vec_it << endl;
		//either use count which returns 1 if found or 0 otherwise
		if (mapp.count(val - *vec_it)) { //AND ADD: && val-*vec_it != *vec_it to avoid using same element.. unless it is repeatd in the input array..
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
			if (second.empty() || dictonary.find(second) != dictonary.end() || can_segment_string2(second, dictonary)) {
				return true;
			}
		}
	}
	return false;
}

//substr: The substring is the portion of the object that starts at character position pos and spans len characters (or until the end of the string, whichever comes first).

string most_common_word(string s) {

	/*ONE SOLUTION - assuming input list is a vector of words... eventually get to a map and later do same, i.e. get a set, etc*/

	//i guess if input is a vector, we won't have the punctation to process..
	vector<string>inp_vec;
	inp_vec.push_back("Bob");
	inp_vec.push_back("hit");
	inp_vec.push_back("a");
	inp_vec.push_back("ball");
	inp_vec.push_back("the");
	inp_vec.push_back("hit");
	inp_vec.push_back("BALL");
	inp_vec.push_back("flew");
	inp_vec.push_back("far");
	inp_vec.push_back("after");
	inp_vec.push_back("it");
	inp_vec.push_back("was");
	inp_vec.push_back("hit");
	cout << "vector list original version" << endl;
	for (auto it = inp_vec.begin(); it != inp_vec.end(); ++it) {
		cout << *it << endl;
	}
	for (auto it = inp_vec.begin(); it != inp_vec.end(); ++it) {
		string inpLower = *it;
		transform(inpLower.begin(), inpLower.end(), inpLower.begin(), [](unsigned char c) { return tolower(c); });
		*it = inpLower;
	}
	cout << "vector list tolower version" << endl;
	for (auto it = inp_vec.begin(); it != inp_vec.end(); ++it) {
		cout << *it << endl;
	}

	map<string, int> word_frq;
	for (auto it = inp_vec.begin(); it != inp_vec.end(); ++it) {
		if (word_frq.find(*it) != word_frq.end()) {
			word_frq[*it] += 1;
		}
		else {
			word_frq[*it] = 1;
		}
	}
	cout << "Start: Just shwoing map.." << endl;
	for (auto it = word_frq.begin(); it != word_frq.end(); ++it) {
		cout << it->first << "=" << it->second<<endl;
	}
	cout << "End: Just shwoing map.." << endl;

	/*ANOTHER SOLUTION - assuming input list is a string... so need to process it; eventually get to a map and later do same, i.e. get a set, etc*/

	string inp = "Bob hit a ball, the hit BALL flew far after it was hit."; //just ignore arg and set here for this exercise

	//traverse string until non word character - that's a word - store it with its count and then proceed to next word, etc
	map<string, int> words_freq;
	while (inp.length() > 0) {
		regex regx("([[:alpha:]]+)([^[:alpha:]]*)");
		smatch mtch;
		regex_search(inp, mtch, regx);
		if (mtch.size() == 3) {
			string mtchToLower = mtch[1];
			transform(mtchToLower.begin(), mtchToLower.end(), mtchToLower.begin(), [](unsigned char c) { return tolower(c); });
			if (words_freq.find(mtchToLower) != words_freq.end()) {
				words_freq[mtchToLower] += 1;
			}   
			else {
				words_freq[mtchToLower] = 1;
			}
			inp = inp.substr(mtch[1].length() + mtch[2].length());
		}
		else {
			break;
		}
	}

	set<int> vals;
	string banned_word = "hit";
	vector<string> banned_list;
	banned_list.push_back("a");
	banned_list.push_back("hit");
	banned_list.push_back("far");
	//vector is not a good option to store banned words since will need to search through all every time..
	//so let me convert to set for intsance.
	unordered_set<string> banned;
	banned.insert("a");
	banned.insert("hit");
	banned.insert("far");

	/*PART 2 OF SOLUTION - COMMON TO BOTH VERSIONS IF INPUT (string vs vector of strings) */

	//iterate through map and sort from highest to lowest and then show in descending order, excluding banned words.
	for (auto iter = words_freq.begin(); iter != words_freq.end(); ++iter) {
		cout << iter->first << "=" << iter->second << endl;
		vals.insert(iter->second);
	}
	cout << "In descending order, the freuqencies of words appearance" << endl;
	for (auto revrs_iter = vals.rbegin(); revrs_iter != vals.rend(); ++revrs_iter) {
		cout << *revrs_iter << endl;
		//iterate thorygh map again and shows words matching each of the values in vals.
		for (auto iter = words_freq.begin(); iter != words_freq.end(); ++iter) { 
			//ANSWER if banned is one word string:    UNCOMMENT: if (iter->second == *revrs_iter && iter->first != banned_word) { //string.compare is used for the string comparison
			//ANSWER if banned is a vector turnd set: UNCOMMENT: if (iter->second == *revrs_iter && banned.find(iter->first) == banned.end()) {
			if (iter->second == *revrs_iter) { //string.compare is used for the string comparison
				cout << iter->first << endl;
				//ANSWER: UNCOMMENT: return iter->first;
			}
		}
	}

	//cannot use find_first_of since cannot gurantee what chars will see other than words. Could use find_first_not_of but then need to list the alphabet, in lower and upper case..
	return "abc";
}


//Given an array n integers, are there elements a, b, c in array such that sum of (a,b,c) equals to a particular target X?
//Solve in O(n^2) at least ( O(n^3) should be easy )
void threesome(/*int inp_arr[], int inp_target*/) {
	//will just set input variables here:
	//int arr[] = { 4, 9, 1, 3, 5, 6, 7, 2 };
	int arr[] = { -1,0,1,2,3,-4 };
	//int target = 5; //6 works for instance
	int target = 0; //6 works for instance
	vector<set<int>> foundNums;
	//like the two sum, but here will be O(n^2) since will iterate through two..
	unordered_set<int> num_set; // NO DUPLICATES WILL BE RECORDED FROM INPUT if use set. USE MULTISET to allow for duplicates..
	for (auto i = 0; i < sizeof(arr) / sizeof(int); i++) {
		num_set.insert(arr[i]);
	}
	for (auto i = 0; i < sizeof(arr) / sizeof(int) - 1; i++) {
		//the j above i is to exclude number that already picked. We also dont need lower than i numbers in j since when i was 0,1,etc, it already covered the range.
		for (auto j = i+1; j < sizeof(arr) / sizeof(int); j++) {
			//the last two conditions are to ensure we don't pick the same number again. BUT, if same num exists multiple times in input array, need to modify below somehow.
			if (num_set.find(target - arr[j] - arr[i]) != num_set.end() && (target - arr[j] - arr[i]) != arr[i] && (target - arr[j] - arr[i]) != arr[j]) {
				cout << " Num 1=" << arr[j] << " Num 2=" << arr[i] << " Num 3=" << target - arr[j] - arr[i] << endl;
				//go through foundNums vectors of sets to see if the 3 numbers already were recorded.
				bool found = false;
				for (auto num = foundNums.begin(); num != foundNums.end(); ++num) {
					set<int> three = *num;
					//iterating through the 3 numbers of each vector entry
					if (three.find(arr[j]) != three.end() && three.find(arr[i]) != three.end() && three.find(target - arr[j] - arr[i]) != three.end()) {
						found = true;
						cout << "Already recorded this triplet" << " Num 1=" << arr[j] << " Num 2=" << arr[i] << " Num 3=" << target - arr[j] - arr[i] << endl;
						break;
					}
				}
				if (!found) {
					cout <<"Recording following triplet" << " Num 1=" << arr[j] << " Num 2=" << arr[i] << " Num 3=" << target - arr[j] - arr[i] << endl;
					set<int> threeNums;
					threeNums.insert(arr[j]);
					threeNums.insert(arr[i]);
					threeNums.insert(target - arr[j] - arr[i]);
					foundNums.push_back(threeNums);
				}
			}
		}
	}
	cout << "Found the following triplets"<<endl;
	for (auto i = foundNums.begin(); i != foundNums.end(); ++i) {
		set<int> three = *i;
		for (auto j = three.begin(); j != three.end(); ++j) {
			cout << *j << " ";
		}
		cout << endl;
	}
	cout << "Done finding the following triplets" << endl;

}


void threeSum() {

	int nums_arr[] = { -1,0,1,2,-1,-4 };

	vector<int> nums (nums_arr, nums_arr + sizeof(nums_arr) / sizeof(int));

	sort(nums.begin(), nums.end());

	vector<vector<int>> ans;

	for (int i = 0; i < nums.size(); i++)
	{
		if (i > 0 && nums[i] == nums[i - 1]) continue; // Now placing the 'i' pointer to next 'DIFFERENT' element

		int target_2 = 0 - nums[i];
		int left = i + 1;
		int right = nums.size() - 1;

		while (left < right)
		{
			int sum = nums[left] + nums[right];

			if (target_2 > sum) left++;

			else if (target_2 < sum) right--;

			else //target_2 == sum 
			{
				vector<int> triplet(3);
				triplet[0] = nums[i];
				triplet[1] = nums[left];
				triplet[2] = nums[right];

				ans.push_back(triplet);

				//Now placing the left pointer to the next different element 
				//(BASICALLY SKIPPING ALL SAME ELEMENTS THAT COMES IN THE RANGE)
				while (left < right && nums[left] == triplet[1]) left++;

				//Now placing the right pointer to the next different element 
				//(BASICALLY SKIPPING ALL SAME ELEMENTS THAT COMES IN THE RANGE)
				while (left < right && nums[right] == triplet[2]) right--;
			}
		}

	}

	cout << "printing trpiplets" << endl;
	for (auto i = ans.begin(); i != ans.end(); ++i) {
		vector<int> insd = *i;
		for (auto j = insd.begin(); j != insd.end(); ++j) {
			cout << *j << " ";
		}
		cout << endl;
	}
}

void twoSum() {

	int nums_arr[] = { -1,0,1,2,-1,-4,0 };

	vector<int> arr(nums_arr, nums_arr + sizeof(nums_arr) / sizeof(int));
	vector<vector<int>> ans;

	int k = -2; //target
	sort(arr.begin(), arr.end());
	int lhs = 0;
	int rhs = arr.size() - 1;
	bool found = false;
	while (lhs < rhs) {
		int sum = arr[lhs] + arr[rhs];
		if (sum == k) {
			cout << "found: " << arr[lhs] << " " << arr[rhs] << endl;
			found = true;
			break;
		}
		else if (sum < k) lhs++;
		else rhs--;
	}
	if (!found) cout << "Did not find" << endl;
}

void twoSum_hash() {

	int nums_arr[] = { -1,0,1,2,-1,-4,0 };

	vector<int> arr(nums_arr, nums_arr + sizeof(nums_arr) / sizeof(int));
	vector<vector<int>> ans;

	int k = 0; //target
	unordered_set<int> values;
	bool found = false;
	for (auto i = arr.begin(); i != arr.end(); ++i) {
		if (values.find(k - *i) != values.end()) {
			cout << "FOUDN: " << k - *i << " " << *i << endl;
			found = true;
			break;
		}
		values.insert(*i);
	}
	if (!found) cout << "Did not find" << endl;
}


//Design a class to find the kth largest element in a stream. Note that it is the kth largest element in the sorted order, not the kth distinct element.
//Implement KthLargest class :
//KthLargest(int k, int[] nums) Initializes the object with the integer kand the stream of integers nums.
//int add(int val) Returns the element representing the kth largest element in the stream.
/*EXAMPLE:
Input
["KthLargest", "add", "add", "add", "add", "add"]
[[3, [4, 5, 8, 2]], [3], [5], [10], [9], [4]]
Output
[null, 4, 5, 5, 8, 8]

Explanation
KthLargest kthLargest = new KthLargest(3, [4, 5, 8, 2]);
kthLargest.add(3);   // return 4
kthLargest.add(5);   // return 5
kthLargest.add(10);  // return 5
kthLargest.add(9);   // return 8
kthLargest.add(4);   // return 8
*/

//************************DESIGN AND QUESTION EXPLAINED
//min_heap below is of type priority_queue. By default, less<T> is the comparison func and dictates the order of elements in this structure.
//This means that calling pop() will remove the LARGEST element and calling top() will just show the largest element.
//BUT, here we use larger<T> so removing the LOWEST element and always maintaining the K largest elements.
//Now, what the QUESTION asks is to provide the Kth largest, i.e. not The Largest, but the say 3rd largest. 
//Using min_heap of size K always keeps at top ([0]) the kth largest. The largest is in location [k] but we can only pop/top [0] and that's what we need.

class KthLargest {
	unsigned int K_;
	/* For reference
	template<
	class T,
	class Container = std::vector<T>,
	class Compare = std::less<typename Container::value_type>
	> class priority_queue;
	*/
	//Priority queues are a type of container adaptors, specifically designed such that its first element is always the greatest of the elements it contains, according to some strict weak ordering criterion.
	std::priority_queue<int, std::vector<int>, std::greater<int>> min_heap;
public:

	/*
		Use min heap of size K to store and sort the top K elements and return the root
	*/
	KthLargest(int k, vector<int>& nums) {
		K_ = k;
		for (auto x : nums) {
			min_heap.push(x);
			if (min_heap.size() > K_) { min_heap.pop(); }
		}
	}

	int add(int val) {
		min_heap.push(val);
		if (min_heap.size() > K_) { min_heap.pop(); }
		return min_heap.top();
	}
};

//merge two sorted lists into one sorted list
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
struct ListNode {
	int val;
	ListNode* next;
	ListNode() : val(0), next(nullptr) {}
	ListNode(int x) : val(x), next(nullptr) {}
	ListNode(int x, ListNode* next) : val(x), next(next) {}
};

//class Solution {
//public:
ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
	ListNode* merged = new ListNode();
	ListNode* merged_head = merged;
	//if could modify l1 and l2, would add a sentitnel to avoid checking bounds..
	while (l1 != nullptr || l2 != nullptr) {
		if (l1 == nullptr && l2 != nullptr) {
			merged->next = l2;
			merged = merged->next;
			l2 = l2->next;
		}
		else if (l1 != nullptr && l2 == nullptr) {
			merged->next = l1;
			merged = merged->next;
			l1 = l1->next;
		}
		else {
			if (l1->val < l2->val) {
				merged->next = l1;
				merged = merged->next;
				l1 = l1->next;
			}
			else {
				merged->next = l2;
				merged = merged->next;
				l2 = l2->next;
			}
		}
	}
	if (merged_head->next == nullptr) return nullptr;
	else return merged_head->next;
}
//};

//Given an array of intervals representing N meetings, find out if a person can attend all the meetings. 
//Input: [[6,7],[2,4],[8,12]] Output: true Explanation: None of the meetings overlap with each other. 
//Input: [[1,4],[2,5],[7,9]] Output: false Explanation: Meetings [1,4] and [2,5] overlap with each other. 

	//The time complexity is O(nlogn) and space complexity is O(n)
//Solution is based on merge intervals idea - we merge all intervals and if there are ovelraps, the number of merged intervals will be less than the original one.
//	class Solution {
//	public:
bool canAttendMeetings(vector<vector<int>>& intervals) {
	int i, j;

	vector<vector<int>>v;
	if (intervals.size() == 0)
	{
		return true;
	}
	sort(intervals.begin(), intervals.end()); //sortig on first column i believe if no compare funciton provided. Could provide comapre func to sort on 2nd, 3rd, etc column. or just sort first row, etc.
	vector<int>l;
	l.push_back(intervals[0][0]);
	l.push_back(intervals[0][1]);
	v.push_back(l);

	for (i = 1; i < intervals.size(); i++)
	{
		vector<int> prev = v.back();
		//time to merge
		if (intervals[i][0] < prev[1])
		{
			l.clear();
			v.pop_back();
			l.push_back(prev[0]);
			l.push_back(max(prev[1], intervals[i][1]));
			v.push_back(l);
		}
		else
		{
			v.push_back(intervals[i]);
		}
	}
	if (intervals.size() == v.size())
	{
		cout << "No meeting conflicts in the meetings provided:" << endl;
		return true;
	}
	return false;

}
//	};



//Given an array, find the length of the longest subarray which has no repeating numbers. 
//Input: A = {1,2,3,3,4,5} Output: 3 Explanation: Longest subarray without any repeating elements are {1,2,3} & {3,4,5}.

int lengthOfLongestSubarray(vector<int>v)
{
	if (v.size() == 0)
	{
		return 0;
	}
	map<int, int> mapy;
	int left = 0, right = 0;
	int max_window = -1;

	for (right = 0; right < v.size(); right++)
	{
		int num = v[right];
		mapy[num] = mapy[num] + 1;

		while (left < right && mapy[num] > 1) {
			mapy[v[left]] = mapy[v[left]] - 1;
			left = left + 1;
		}
		//calculating max_length of window
		max_window = max(max_window, right - left + 1);
	}
	return max_window;
}


//i'd say code it so that find top K freq words - could be one task. ANd then find which are in the keord list and output those - second task.
//maybe not.. or maybe yes..
//Ok, so if i take keyword=services and search in each review, that's ok but then searching same reviews for other keywords - not good.
//So, go through each review and store the freq of each word and then match taht word to keyword. and make lowercase, etc..and sort equal freq.

/*bool compare_map_vals(pair<int, string> p1, pair<int, string> p2) {
	if (p1.first > p2.first) return true;
	else if (p1.first == p2.first) return p1.second > p2.second;
	else return false;
}*/

class compare_map_vals
{
public:
	bool operator()(const pair<int, string>& lhs, const pair<int, string>& rhs)
	{
		//when returning true it means the item goes to the bottom of the queue and calling top() shows the opposite value
		if (lhs.first < rhs.first) return true;
		else if (lhs.first == rhs.first) {
			//sorting alphapbetically means Higher letter at the bottom
			if (lhs.second > rhs.second) {
				return true;
			}
			else {
				return false;
			}
		}
		else return false;
	}
};

void top_K_freq_keywords() {
	int k = 2;
	//practice list.. could use vector. Lists cannot access random elements diretly since need to traverse; otherwise, no issues. This is also not an issue, depedning on suage of course.
	list<string> keywords;
	keywords.push_back("services");
	keywords.push_back("algorithms");
	keywords.push_back("interview");

	list<string> reviews;
	reviews.push_back("algorithms and Me provides the best services for the interview preparations");
	reviews.push_back("Also, their mock interview services awesome services");
	reviews.push_back("Best services provided by Algorithms and me, everyone should use their services");

	map<string, int> words_freq;
	for (auto review : reviews) {
		string review_st = review;
		map<string, int> words_freq_local; //to be used to count just one iteration of repeating words
		//cout << "Review as is=" << review_st << endl;
		//transform(review_st.begin(), review_st.end(), review_st.begin(), [](unsigned char c) { return tolower(c); });
		//cout << "Review lowercase=" << review_st << endl;
		//split review into words and insert each word into words_freq - but for same review, don't increment count for multpiel appernc word
		while (review_st.length() > 0) {
			regex regx("([[:alpha:]]+)([^[:alpha:]]*)");
			smatch mtch;
			regex_search(review_st, mtch, regx);
			if (mtch.size() == 3) {
				string mtchToLower = mtch[1];
				transform(mtchToLower.begin(), mtchToLower.end(), mtchToLower.begin(), [](unsigned char c) { return tolower(c); });
				if (words_freq_local.find(mtchToLower) != words_freq_local.end()) {
					words_freq_local[mtchToLower] += 1;
				}
				else {
					words_freq_local[mtchToLower] = 1;
				}
				review_st = review_st.substr(mtch[1].length() + mtch[2].length());
			}
			else {
				break;
			}
		}
		//traverse through local map and add to global
		for (auto i = words_freq_local.begin(); i != words_freq_local.end(); ++i) {
			if (words_freq.find(i->first) != words_freq.end()) {
				words_freq[i->first] += 1;
			}
			else {
				words_freq[i->first] = 1;
			}
		}
	}

	cout << "Print resulting freq map" << endl;
	for (auto i = words_freq.begin(); i != words_freq.end(); ++i) {
		cout << i->first << "="<<i->second << endl;
	}
	cout << "Done printing resulting freq map" << endl;

	//show me freq count of keywords
	for (auto keyword : keywords) {
		cout << "Keyword " << keyword << " appeared in all reviewes " << words_freq[keyword] << " times" << endl;
	}

	multimap<int, string> words_freq_rvrs;
	for (auto keyword : keywords) {
		words_freq_rvrs.insert(pair<int,string> (words_freq[keyword], keyword));
	}
	for (auto i = words_freq_rvrs.begin(); i != words_freq_rvrs.end(); ++i) {
		cout << "first=" << i->first << " and second=" << i->second << endl;
	}
	//sort by freq in descending order and then sort alphabetically by ascending order
	priority_queue<pair<int,string>, vector<pair<int, string>>, compare_map_vals> pq;
	for (auto word_f : words_freq_rvrs) {
		pq.push(word_f);
	}
	//display top K elements
	int cnt = 0;
	while (!pq.empty() && cnt < k) {
		cout << "key val: "<<pq.top().first << " " << pq.top().second<<endl;
		pq.pop();
	}

}



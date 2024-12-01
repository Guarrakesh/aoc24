#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <map>
using namespace std;

int main(int argc, char** argv) {
	ifstream file(argv[1]);

	string str;
	vector<int> v1;
	vector<int> v2;
	while (getline(file, str)) {
		int loc1 = stoi(str.substr(0, 5));
		int loc2 = stoi(str.substr(8,5));
		v1.push_back(loc1);
		v2.push_back(loc2);
	}

	map<int, int> v2freq;
	for (auto num: v2) {
	   auto it = v2freq.find(num);
	   if (it == v2freq.end()) {
	       v2freq[num] = 1;
	   } else {
		   it->second = it->second + 1;
	   } 
	
	}
//	sort(v1.begin(), v1.end());
//	sort(v2.begin(), v2.end());

	long long similarity = 0;
	for (auto num: v1) {
 	  	auto it = v2freq.find(num);
		if (it != v2freq.end()) {
			similarity += num * it->second;
		}
		
	}

	cout << "similarity is " << similarity << endl;
}

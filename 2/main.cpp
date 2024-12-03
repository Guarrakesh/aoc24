#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <map>
#include <algorithm>
using namespace std;

bool isIncreasing(vector<int>& vec) {
	int prevNumber = vec[0];
	for (int i=1; i<vec.size(); i++) {
		auto num = vec[i];
		if (num <= prevNumber) {
			return false;
		} else {
			int diff = abs(num - prevNumber);
			if (diff >= 1 && diff <= 3) {
				prevNumber = num;
				continue;
			}
			return false;
		}
	}

	return true;
}
int main(int argc, char** argv) {
	ifstream file(argv[1]);
	
	string line;
	int safe = 0;
	
	while (getline(file, line)) {
		vector<int> nums ;
		string word = "";
		for (char c: line) {
			if (c != ' ') {
				word += c;
				continue;
			}
			cout << word << " ";
			int num = stoi(word);
			nums.push_back(num);
			word = "";			
		}
		if (!word.empty()) {
			nums.push_back(stoi(word));
			cout << word;
		} 
	
		
		// let's go brute force here. 
		// foreach report, I try to remove a number until it is valid.	
		bool isSafe = false;
		int numTries = nums.size()+1;
		while (!isSafe && numTries > 0) {
			vector<int> copied(nums);
			if (numTries <= copied.size()) {
				//skip first iteration,	
				copied.erase(copied.begin() + numTries-1); // then start from removing last element
			}

			
			if (copied.front() == copied.back()) {
				numTries--;
			}


			if (copied.front() > copied.back()) {
				reverse(copied.begin(), copied.end());
			}
			
			if (isIncreasing(copied)) {
				cout << " --> Safe " ;
				isSafe = true;
			} else {
				numTries--;
			}
			
		}
		if (isSafe) {
			cout << " --> OK (removing " << numTries << " element";
			safe++;

		} 
		cout << endl;
			
	}
	
	
	cout << "there are " << safe << " reports!" << endl;	
}

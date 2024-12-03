#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <map>
using namespace std;

int checkMonotonic(const vector<int>& nums, bool increasing) {
	int prevNumber = increasing ? nums.front()-1 : nums.front()+1;
	bool levelRemoved = false;
	for (int i=0; i<nums.size(); i++) {
		bool isIncreasing = nums[i] > prevNumber;
		int diff = abs(nums[i] - prevNumber);
		if (isIncreasing) {
			if (increasing && diff >= 1 && diff <= 3) {
				prevNumber = nums[i];
				continue;
			 } else {
				if(levelRemoved) return false;
				else {
					levelRemoved = true;
					continue;
				}			
			}
			
		} else {
			// not increaisng
			if (!increasing && diff >= 1 && diff <= 3) {
				prevNumber = nums[i];
				continue;
			} else {
				if (levelRemoved) return false;
				else {
				 	levelRemoved = true;
					continue;
				}
			}

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
		if (nums.front() == nums.back()) continue;
		
		bool result;
		if (nums.front() > nums.back()) {
			// expect decreasing
			cout << " --> Decreasing?";
			result = checkMonotonic(nums, false);
					
		} else {
			// expect increasing
			cout << " --> Increasing?";
			result = checkMonotonic(nums, true);
		}

		if (result) {
			safe++;	
			cout << " --> YES!";
		} else {
			cout << " -> NO";
		}
		
		cout << endl;
		
	}
	
	
	cout << "there are " << safe << " reports!" << endl;	
}

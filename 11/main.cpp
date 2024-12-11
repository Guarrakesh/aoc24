#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <format>
#include <unordered_map>

using namespace std;

int main(int argc, char** argv) {
	fstream file(argv[1]);
	string line;
	
	getline(file, line);
	int blinks = stoi(argv[2]);
	int pos;
	vector<long long> numbers;
	while ((pos = line.find(' ')) != string::npos) {
		numbers.push_back(stoll(line.substr(0,pos)));
		line.erase(0,pos+1);
	}

	unordered_map<unsigned long long, unsigned long long> stoneCount;
	numbers.push_back(stoll(line));
	for (auto num: numbers) stoneCount[num] = 1;
	unsigned long long sum = 0; 
	
	for (int i=0; i<blinks; i++) {
		unordered_map<unsigned long long, unsigned long long> newStones;
		for (auto &it: stoneCount) {
			auto num = it.first;
			if (num == 0) {
				newStones[1] = newStones[1] + it.second;
				//cout << newStones[1] << endl;
			} else if (to_string(num).size() % 2 == 0) {
				auto str = to_string(num);
				int half = str.size() / 2;
				auto num1 = stoll(str.substr(0, half));
				auto num2 = stoll(str.substr(half));
				
				newStones[num1] = newStones[num1] + it.second;
				
				newStones[num2] = newStones[num2] + it.second;
			} else {
				auto newNum = num * 2024;
				newStones[newNum] = newStones[newNum] + it.second;
			}

		}
		
		stoneCount = newStones;	
	}
	
	for (auto &it: stoneCount) {
		sum += it.second;	
	}
	cout << "Sum is " << sum << endl ;
	
}

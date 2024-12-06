#include <fstream>
#include <string>
#include <vector>
#include <iostream> 
#include <format>
#include "graph.h"
#include <algorithm>
#include <random>
using namespace std;

/**
* Here we do a bubblesort-like approach. 
* Everytime the list of updates doesn't follow the rule
* Swap the two elements (from & to) that are not connected and repeat
* until all the list is sorted.
* For the lists already sorted, we don't need to count them,
* so we keep track of a "swapped" boolean that we set to true only when we swap
* so at the end of the for loop, if we iterated over all elements and we swapped at least once
* then we have an ordered list, so we get the middle and return it.
*/
int addLineMiddle(Graph& graph, vector<int>& nums) {
	int ans = 0;
	// the upgrades already good, no need to swap & count.	
	bool swapped = false; 
	while (true) {
		int i=1;
		for (; i<nums.size(); i++) {
			if (!graph.hasEdge(nums[i-1], nums[i])) {
				cout << format("Swapping {}<->{}", nums[i-1],nums[i]) << endl;
				swap(nums[i-1], nums[i]);
				swapped = true;
				break;
			}
		}
		if (i == nums.size()) {
			if (!swapped) return 0; // if we did no swap, update list was good, in part2 we do not count the middle of those
			// end of the line, we have a full edge.
			int middle = (nums.size() / 2);
			cout << format("Done Swapping. Middle is {}",nums[middle] ) << endl;
			return nums[middle];
		}
	}
}		
int main(int argc, char** argv) {
	ifstream file(argv[1]);

	string line;
	Graph graph(99);
	
	while (getline(file, line) && !line.empty()) {
		int from = stoi(line.substr(0,2));
		int to = stoi(line.substr(3,2));
		graph.addEdge(from, to);
	}


	int sum = 0;
	while (getline(file, line)) {
		vector<int>  nums;
		while (line.size() > 0) {
			int num = stoi(line.substr(0,2));
			nums.push_back(num);
			line.erase(0,3);
		}

		sum += addLineMiddle(graph, nums);
		cout << line;
	
	}	
	
	cout << endl << "Sum is " << sum << endl;;
}

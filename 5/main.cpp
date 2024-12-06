#include <fstream>
#include <string>
#include <vector>
#include <iostream> 
#include <format>
#include "graph.h"
using namespace std;

int addLineMiddle(Graph& graph, string& line) {

	int prev = -1;
	vector<int> nums;
	while (line.size() > 0) {
		int num = stoi(line.substr(0,2));
		nums.push_back(num);
		if (prev != -1) {
			if (!graph.hasEdge(prev, num)) return 0 ; 
		}
							
		prev = num; 
		line.erase(0,3);
	}

	// end of the line, we have a full edge.
	int middle = (nums.size() / 2);
	cout << format("Middle ({}) = {}", middle, nums[middle]);	
	return nums[middle];
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
		sum += addLineMiddle(graph, line);
		cout << line;
	
	}	
	
	cout << endl << "Sum is " << sum << endl;;
}

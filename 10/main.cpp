#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <format> 
#include <unordered_map>
using namespace std;

struct pair_hash {
	std::size_t operator() (const pair<int,int>  &p) const {
		auto h1 = std::hash<int>{}(p.first);
		auto h2 = std::hash<int>{}(p.second);
	
		return h1^h2;
	}
};

typedef unordered_map<pair<int,int>, bool, pair_hash> visited_type;
std::pair<int,int> operator+(const std::pair<int,int>& l, const std::pair<int,int>& r) {
	return {l.first + r.first, l.second + r.second};
}


bool isValid(vector<vector<int>>& grid, pair<int,int>& pos) {
	int n = grid.size();
	int m = grid[0].size();

	return pos.first >= 0 && pos.first < n && pos.second >= 0 && pos.second < m;
}

 

void dfs(pair<int,int> head, int currentStep, vector<vector<int>>& grid, visited_type& visited, int& sum, bool all_paths)
{
	vector<pair<int,int>> directions = {
		{-1, 0}, {0, 1},{1, 0},{0,-1}

	};
	if (!isValid(grid, head)) {
		return;
	}
	

	// A bit of confusion here, let's clarify:
	// for part1 (all_paths=false), we only want to count how many DIFFERENT nines (trail ends) 
	// a head can reach. So in this case the "visited" takes count of all
	// already-reached nines. This means that if the same head leads to the same nine
	// through a different path, we don't want to increase the score.
		
	// in part 2 tho (all_paths=true) we actually DO want to check all distincts paths
	// that a head can use to reach a single nine.
	// so in part 2, "visited" is basically useless.	
	if (currentStep == 9 && grid[head.first][head.second] == 9) {
		if ((!all_paths && visited.find(head) == visited.end()) || all_paths) {	
			// in part1, we make sure this nine won't be hit again by the same head
			if (!all_paths) {
				visited[head] = true;
			}
			sum+=1;
		}
	}

	// wrong step, abort this path.
	if (grid[head.first][head.second] != currentStep) { 
		return;	
	}
	//cout << format("[{}] Chekcing neighbours of ({},{})", currentStep,head.first, head.second) << endl;
	
	// let's move along the four directions
	for (int i=0; i<4; i++) {
		dfs(head + directions[i], currentStep + 1, grid, visited, sum, all_paths);
	} 
;	

}
int main(int argc, char** argv) {
	ifstream file(argv[1]);
	string line;

	vector<vector<int>> grid;
	
	vector<pair<int,int>> trailheads;

	int i=0;
	while (getline(file, line)) {
	 	int j=0;
		vector<int> row;
		for (auto c: line) {
			row.push_back(c - '0');
			if (c == '0') {
				// in the same iteration we get the input, we save our heads in a map
				trailheads.push_back({i,j});
			}  	
			j++;
		}
		grid.push_back(row);
		i++;
	}

		
	int totalSum = 0;
	for (auto head: trailheads) {
		int sum = 0;
		// cout << format("Checking path from head ({},{})\n", head.first, head.second);
	 	visited_type visited;	
		dfs(head, 0, grid, visited, sum, false);
		totalSum += sum;
	}

	int totalSum2 = 0;;
	for (auto head: trailheads) {
		int sum = 0;
		visited_type visited;
		dfs(head, 0, grid, visited, sum, true);
		totalSum2 += sum;
	}

	cout << "Result Part 1= " << totalSum << endl;	
	cout << "Result Part 2= " << totalSum2 << endl;	

;
	
		
}

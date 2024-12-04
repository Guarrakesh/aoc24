#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <format>
using namespace std;

const vector<char> letters = {'X', 'M', 'A', 'S'};
bool isValid(vector<vector<char>>& matrix, int rows, int cols, int i, int j) {
	return i >= 0 && i < rows && j >= 0 && j < cols;
}


bool dfs(vector<vector<char>>& matrix, int rows, int cols, int i, int j, int letterIndex, pair<int, int> direction) {

	if (!isValid(matrix, rows, cols, i, j)) return false;
	if (letters[letterIndex] == 'S' && matrix[i][j] == 'S') {
		//cout << "XMAS!\n";
		return true;
	}
	if (letterIndex >= letters.size()) return false;

	int rowIdx = i + direction.first;
	int colIdx = j + direction.second;
	if (matrix[i][j] == letters[letterIndex]) {
		//cout << format("Letter {}:\n", letters[letterIndex]);
		//cout << format("({},{}): Going direction {},{}", i, j, direction.first, direction.second) << endl;	
		return dfs(matrix, rows, cols, rowIdx, colIdx, letterIndex + 1, direction); 
	} 

	return false;

	
	
}


int main(int argc, char** argv) {
	ifstream file(argv[1]);

	string line;
	vector<vector<char>> matrix{};

	while (getline(file, line)) {
		vector<char> row;
		for (char c: line) {
			row.push_back(c);
		}
		matrix.push_back(row);
	} 

	cout << format("Matrix size is {}x{}", matrix.size(), matrix[0].size());
	
	// we need to find all the Xs, and from there, moving all possible directions to find the next letter ('M'. 
	// then we move the SAME direction chosen until we find the rest of the letters ('A' and 'S')
	const int rows = matrix.size();
	const int cols = matrix[0].size();
	int sum = 0;

	for (int i=0; i<rows; i++) {
		for (int j=0; j<cols; j++) {
			char letterIndex = 0;
			if (dfs(matrix, rows, cols, i, j, letterIndex, make_pair(0,1))) sum+= 1; // right
			if (dfs(matrix, rows, cols, i, j, letterIndex, make_pair(1,1))) sum+=1; //bottom right
			if (dfs(matrix, rows, cols, i, j, letterIndex, make_pair(1,0))) sum+=1; //bottom
			if (dfs(matrix, rows, cols, i, j, letterIndex, make_pair(1,-1))) sum+=1; // bottom left
			if (dfs(matrix, rows, cols, i, j, letterIndex, make_pair(0,-1))) sum+=1; //left
			if (dfs(matrix, rows, cols, i, j, letterIndex, make_pair(-1,-1))) sum+=1; // top left
			if (dfs(matrix, rows, cols, i, j, letterIndex, make_pair(-1,0))) sum+=1; //top
			if (dfs(matrix, rows, cols, i, j, letterIndex, make_pair(-1,1))) sum+=1; //top right

			
		}
	}

	cout << "Sum is " << sum << endl;

}

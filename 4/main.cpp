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


void rotate(vector<vector<char>>& matrix,  int rows, int cols) {
	//transpose
	for (int i=0; i<rows; i++) {
		for (int j=i+1; j<cols; j++) {
			swap(matrix[i][j], matrix[j][i]);
		}	
	}

	// reverse rows
	for (int i=0; i<rows; i++) {
		for(int j=0; j<cols/2; j++) {
			swap(matrix[i][j], matrix[i][rows-j-1]);
		}
	}
}

void display(vector<vector<char>>& matrix) {
	for (auto row: matrix) {
		for (auto col: row) {
			cout << col;
		}
		cout << endl;
	}
	cout << endl << endl;
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

	cout << format("Matrix size is {}x{}\n", matrix.size(), matrix[0].size());
	
	// we need to find all the Xs, and from there, moving all possible directions to find the next letter ('M'. 
	// then we move the SAME direction chosen until we find the rest of the letters ('A' and 'S')
	const int rows = matrix.size();
	const int cols = matrix[0].size();
	int sum = 0;

	for (int rot=0; rot<4; rot++) {
		//display(matrix);
		for (int i=0; i<rows-2; i++) {
			for (int j=0; j<cols-2; j++) {
				if (matrix[i][j] == 'M'
					&& matrix[i][j+2] == 'S' 
					&& matrix[i+2][j] == 'M'
					&& matrix[i+2][j+2] == 'S'
					&& matrix[i+1][j+1] == 'A') {
						cout << format("X-MAS at ({},{})\n", i, j);
						sum+=1;
					}
			
			}
		}
		cout << "Rotating...\n";
		rotate(matrix, rows, cols);
	}	

	cout << "Sum is " << sum << endl;

}

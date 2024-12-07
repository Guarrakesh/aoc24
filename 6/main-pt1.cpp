#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <format>

using namespace std;

typedef pair<int,int> dir_t;
const dir_t TOP = make_pair(-1, 0);
const dir_t BOTTOM = make_pair(1,0);
const dir_t RIGHT = make_pair(0,1);
const dir_t LEFT = make_pair(0,-1);

void rotate(char& marker, dir_t& dir) {
    switch (marker) {
        case '>': {
            marker = 'v';
            dir = BOTTOM;
            break;
        }
        case 'v': 
            marker = '<';
            dir = LEFT;
            break;
            
        case '<': {
            marker = '^';
            dir = TOP;
            break;
        case '^': {
            marker = '>';
            dir = RIGHT;
        }
    }
}
}

int main(int argc, char** argv) {
    ifstream file (argv[1]);

    string line;
    vector<vector<char> > grid;
    pair<int,int> startPos;
    int i = 0;
   
    while (getline(file, line)) {
        
        vector<char> row;
        for (int j=0; j<line.size(); j++) {
            
            row.push_back(line[j]);
            if (line[j] == '^') {
                startPos = make_pair(i, j);
            }
        }
        i++;
        grid.push_back(row);
    }

    cout << "Start pos = " << startPos.first << "," << startPos.second << endl;
    
    int visitedCount = 1;
    dir_t currPos = startPos;
    char dirChar = grid[currPos.first][currPos.second];
    dir_t dir = TOP;
    

    while (true) {
        // if current pos is not visited yet, increment visited count
        if (grid[currPos.first][currPos.second] == '.') {
            visitedCount++;
        }
        // mark element as visited.
        grid[currPos.first][currPos.second] = 'X'; // mark as visited

        // calculate next pos based on direction
        auto nextPos = make_pair(currPos.first + dir.first, currPos.second + dir.second);
        int i = nextPos.first;
        int j = nextPos.second;
        // out of grid? patrolling is over.
        if (i < 0 || i >= grid.size() || j < 0 || j >= grid[0].size()) {
            break;
        }

        // Next one is obstacle? rotate.
        if (grid[nextPos.first][nextPos.second] == '#') {
            rotate(dirChar, dir);
            continue;
        } 

        // update currPos to nextPos        
        currPos = nextPos;
        //cout << "Next Pos = ( " << currPos.first << "," << currPos.second << ")" << endl;

    

    }

    cout << "Visited count = " << visitedCount << endl;
}
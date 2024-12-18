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

//0-no loop, 1-loop
int run(vector<vector<char> >& grid, dir_t startPos)
{
    
    dir_t currPos = startPos;
    char dirChar = grid[currPos.first][currPos.second];
    dir_t dir = TOP;
    
    map<pair<pair<int,int>, dir_t>, int> visitedMap;
    
    while (true) {
        auto key = make_pair(currPos, dir);
        // if current pos is not visited yet, increment visited count
        if (!visitedMap.contains(key)) {
            visitedMap[key] = 1;
        } else {
            visitedMap[key] = visitedMap[key] + 1;
        }

        if (visitedMap[key] == 2) {
            // loop
            return 1;
        }

        // calculate next pos based on direction
        auto nextPos = make_pair(currPos.first + dir.first, currPos.second + dir.second);
        int i = nextPos.first;
        int j = nextPos.second;
        // out of grid? patrolling is over.
        if (i < 0 || i >= grid.size() || j < 0 || j >= grid[0].size()) {
            break;
        }

        // Next one is obstacle? rotate.
        if (grid[nextPos.first][nextPos.second] == '#' ) {
            rotate(dirChar, dir);
            continue;
        } 

        // update currPos to nextPos        
        currPos = nextPos;
        //cout << "Next Pos = ( " << currPos.first << "," << currPos.second << ")" << endl;

    

    }

    return 0;
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



    int numPosition = 0;
    for (int i=0; i<grid.size(); i++) {
        for(int j=0; j<grid[0].size(); j++) {
            if (grid[i][j] == '#' || grid[i][j] == '^') continue;

            grid[i][j] = '#';

            // run and detect loop
            if (run(grid, startPos) == 1) {
                //cout << "valid obstacle: (" << i << "," << j << ")" << endl;
                numPosition++;
            }

            grid[i][j] = '.';
        }
    }
  
    cout << "Num positions " << numPosition << endl;
}
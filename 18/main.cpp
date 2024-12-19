#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <format>
#include <queue>
#include <cmath>
#include <algorithm>

using namespace std;

typedef vector<vector<char>> grid_t;
typedef pair<int,int> pii;

bool isValid(int i, int j, grid_t& grid) {
    return i >= 0 && i<grid.size() && j>=0 && j<grid[0].size() && grid[i][j] != '#';
}  

void parseInput(ifstream& file, vector<pii>& bytes) {
    string line;
    while(getline(file, line)) {
        int comma = line.find(',');
        pii byte;
        byte.second = stoi(line.substr(0,comma));
        byte.first = stoi(line.substr(comma+1));
        bytes.push_back(byte);
    }

}

long dijkstra(
    grid_t& grid, 
    pair<int,int> source,
    pair<int,int> target
    ) {
   
    vector<vector<uint64_t>> distMap { grid.size(), vector<uint64_t>(grid[0].size(), UINT64_MAX) };
    queue<pair<int,int>> q;
    q.push(source);
    distMap[source.first][source.second] = 0;

    vector<pair<int,int>> dirs = { {0, 1}, {1,0}, {0,-1}, {-1,0}};
    while (!q.empty()) {
        auto cell = q.front();
        q.pop();
      
        for (const auto d: dirs) {
            pair<int,int> next = { cell.first + d.first, cell.second + d.second }; 

            if (!isValid(next.first, next.second, grid)) {
                continue;
            }


            int offset = 1;
            uint64_t newDist = distMap[cell.first][cell.second] + offset;
          
            if (newDist < distMap[next.first][next.second]) {
               
                distMap[next.first][next.second] = newDist;
                q.push(next);
            } 
               
        }
    }


   return distMap[target.first][target.second];
}

void printGrid(grid_t& grid) {
    for (auto row: grid) {
        for (auto col: row) {
            cout << col;
        }
        cout << endl;
    }
}

void fallBytes(vector<pii>& bytes, grid_t& grid, int numBytes) {
    for (int i=0; i<numBytes; i++) {
        grid[bytes[i].first][bytes[i].second] = '#';
    }
}
int main(int argc, char** argv) {
    ifstream file(argv[1]);

    string line;
    getline(file, line);
    int rows = stoi(line.substr(0, line.find('x')));
    int cols = stoi(line.substr(line.find('x')+1));
    vector<pii> bytes;
    parseInput(file, bytes);
    cout << " We have " << bytes.size() << " bytes!"<< endl;

    grid_t grid(rows, vector<char>(cols, '.'));

    const int numBytes = stoi(argv[2]);
    fallBytes(bytes, grid, 1024);
    int min = dijkstra(grid, {0,0}, {grid.size()-1, grid.size()-1});
    cout << "[Part 1] Sol = " << min << endl;
   
    int i=1;
    while (i < bytes.size()) {
        grid_t grid(rows, vector<char>(cols, '.'));
        fallBytes(bytes, grid, i);
        int min = dijkstra(grid, {0,0}, {grid.size()-1, grid.size()-1});

        if (min == -1) break;
        i++;
    }
   
    
    cout << "[Part 2] Byte " << bytes[i-1].second << "," << bytes[i-1].first << endl;
}
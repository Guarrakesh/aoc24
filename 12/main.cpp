#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <format>
#include <queue>
#include <cmath>

using namespace std;
struct key_hash {
    template<class T1, class T2>
    std::size_t operator() (const pair<T1, T2> &key) const {
        return hash<T1>{}(key.first) ^ hash<T2>{}(key.second);
    }
};
typedef vector<vector<char>> grid_t;
typedef unordered_map<pair<int,int>, bool, key_hash> visited_t;


bool isValid(grid_t& grid, int i, int j) {
    return i>= 0 && i < grid.size() && j >= 0 && j < grid[0].size();
}

int countCorners(vector<pair<int,int>>& neighbours, pair<int,int> node, grid_t& grid ){
    // if (neighbours.size() == 0) {
    //     return 4;
    // }
    // if (neighbours.size() == 4) {
    //     return 0;
    // }
    // if (neighbours.size() == 3) {
    //     return 0;
    // }
    // if (neighbours.size() == 1) {
    //     return 2;
    // }   

    // if (neighbours.size() == 2) {
    //     if (neighbours[0].first == neighbours[1].first || neighbours[0].second == neighbours[1].second) {
    //         return 0;
    //     } else {
    //         // Invariant: given the order of our directions vector in the bfs, neighbours vector count clockwise starting from right
    //         // L corner
    //         pair<int, int> corner;
    //         if ((neighbours[0].second == node.second && neighbours[1].second == node.second-1)) {
    //             //up left or up right
    //             if (neighbours[0].first == node.first-1) {
    //                 // up left
    //                 corner = {node.first-1, node.second-1};
    //             } else {
    //                 corner = {node.first+1, node.second-1};
    //             }
            
    //         } else if (neighbours[0].second == node.second && neighbours[1].second == node.second+1) {
    //             // bottom right
    //             corner = { node.first -1, node.second + 1};
    //         } else if (neighbours[0].second == node.second+1 && neighbours[1].second == node.second) {
    //             corner = { node.first + 1, node.second + 1};
    //         } else {
    //             return 1;
    //         }

    //         return isValid(grid, corner.first, corner.second) && grid[corner.first][corner.second] == grid[node.first][node.second] ? 1 : 2;
            
    //     }
    // }

    // // if (neighbours.size() == 3) {

    // // }
    // return 0;

    //top left 
    int countCorners = 0;
    auto top = isValid(grid, node.first-1, node.second) ? grid[node.first-1][node.second] : -1;
    auto left = isValid(grid, node.first, node.second-1) ?grid[node.first][node.second-1] : -1;
    auto right = isValid(grid, node.first, node.second+1) ?grid[node.first][node.second+1] : -1;
    auto bottom = isValid(grid, node.first+1, node.second) ?grid[node.first+1][node.second] : -1;

    auto topLeft = isValid(grid, node.first-1, node.second-1) ?grid[node.first-1][node.second-1]: -1;
    auto topRight = isValid(grid, node.first-1, node.second+1) ?grid[node.first-1][node.second+1]: -1;
    auto bottomLeft = isValid(grid, node.first+1, node.second-1) ?grid[node.first+1][node.second-1]: -1;
    auto bottomRight = isValid(grid, node.first+1, node.second+1) ?grid[node.first+1][node.second+1]: -1;
    auto type = grid[node.first][node.second];

    // check top left
    if ((top != type && left != type) || (top == type && left == type && topLeft != type) ) {
        countCorners++;
    } 
    if ((top != type && right != type) || (top == type && right == type && topRight != type)) {
        // top right
        countCorners++;
    }
    if ((bottom != type && right != type) || (bottom == type && right == type && bottomRight != type)) {
        //bottom right
        countCorners++;
    }
    if ((bottom != type && left != type) || (bottom == type && left == type && bottomLeft != type)) {
        countCorners++;
    }

    return countCorners;
   
}
tuple<int,int,int> bfs(int i, int j, grid_t& grid, visited_t& visited, bool countSides) {
    queue<pair<int,int>> q;

    visited[{i,j}] = true;
    q.push({i,j});

    auto type = grid[i][j];

    vector<pair<int,int>> dirs = { {0,1}, {1,0}, {0,-1}, {-1, 0}};

    int totalPerimeter = 0;
    int totalArea = 0;
    printf("[%c] Beginning neighbours search\n", type);
    int numOfCorners = 0;

    while (!q.empty()) {
        auto curr = q.front();
        q.pop();
        int currentPerimeter = 4;
        totalArea++;

        vector<pair<int,int>> neighbours;
        for (auto dir: dirs) {
            int newX = curr.first + dir.first;
            int newY = curr.second + dir.second;
            
            if (isValid(grid, newX, newY) && grid[newX][newY] == type) {
                neighbours.push_back({newX, newY});
                const pair<int,int> dir = { abs(newX - curr.first ), abs(newY - curr.second )};
                currentPerimeter--;
                if (visited.find({newX,newY}) == visited.end()) {
                    printf("-- Pushing neighbour (%d,%d)\n", newX, newY);
                    q.push({newX, newY});

                }
                 visited[{newX,newY}] = true;
                
            }
        }
        int numCorners = countCorners(neighbours, curr, grid);
        printf("-- Adding %d corners\n", numCorners) ;
        numOfCorners += numCorners;

        totalPerimeter += currentPerimeter;
    }

    return {totalPerimeter, totalArea, numOfCorners};
}

int solve(grid_t& grid, visited_t visited, bool countSides) {
   int sum = 0;
    for (int i=0; i<grid.size(); i++) {
        for (int j=0; j<grid[0].size(); j++) {
            if (visited.find({i,j}) != visited.end()) {
                continue;
            }
            auto result = bfs(i, j, grid, visited, countSides);
            printf("[%c] Origin (%d,%d): Perimeter=%d\t Area=%d\t Corners=%d\n", grid[i][j], i,j,get<0>(result),get<1>(result), get<2>(result));
            sum += countSides 
            ? get<2>(result) * get<1>(result)
            : get<0>(result) * get<1>(result)
            ;
        }
    }

    return sum;
}
int main(int argc, char** argv) {
	fstream file(argv[1]);

	string line;
	grid_t grid;
   
    while (getline(file, line)) {
        vector<char> row;
        for (auto c: line) row.push_back(c);
        grid.push_back(row);
    }

 
    visited_t part1Visited;
    visited_t part2Visited;
    auto sum = solve(grid,part1Visited, false);
    cout << " [Part 1] Total sum = " << sum << endl;
    cout << "\n\n\n";
    sum = solve(grid,part2Visited, true);
    cout << " [Part 2] Total sum = " << sum << endl;
}   



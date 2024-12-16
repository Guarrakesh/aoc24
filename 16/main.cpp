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
typedef vector<vector<bool>> visited_t;
struct cell_t {
    int row; 
    int col;
    int64_t dist;
    pair<int,int> dir;
    cell_t* prev;
};

bool isValid(int i, int j, grid_t& grid) {
    return i >= 0 && i<grid.size() && j>=0 && j<grid[0].size() && grid[i][j] != '#';
}  

void printGrid(grid_t& grid) {
    for (auto row: grid) {
        for (auto c: row) {
            cout << c;
        }
        cout << endl;
    }
}


int findBestSpots(grid_t& grid, 
    pair<int,int> source,
    pair<int,int> target,
    vector<vector<uint64_t>>& distMap,
    vector<vector<pair<int,int>>>& prevMap,
    vector<vector<vector<tuple<int,int,int>>>> prevMap2
    ) {
    queue<pair<int,int>> q;
    
    int numOfSpots = 0;
    
    visited_t visited (grid.size(), vector<bool>(grid[0].size(), false));

    visited[target.first][target.second] = false;
    q.push(target);
    grid[target.first][target.second] = 'O';
    int currentScore = distMap[target.first][target.second];

    while (!q.empty()) {
        
        auto el = q.front();
        q.pop();
   
        auto v = prevMap2[el.first][el.second];

        vector<int> scores;

        // sort ones with lowest score
        sort(v.begin(), v.end(), [] (const tuple<int,int,int>& lhs, const tuple<int,int,int>& rhs) {
            return get<2>(lhs) > get<2>(rhs);
        });
        for (auto prev: v) {
            if (isValid(get<0>(prev), get<1>(prev), grid) && visited[get<0>(prev)][get<1>(prev)] == false) {    
              
                auto score = get<2>(prev);

                if (score <= currentScore) {
                    scores.push_back(score);
                    visited[get<0>(prev)][get<1>(prev)] = true;
                    q.push({get<0>(prev), get<1>(prev)});
                    grid[get<0>(prev)][get<1>(prev)] = 'O';
                } else {
                    printf("(%d,%d) Score=%d > %d\n",get<0>(prev), get<1>(prev), score, currentScore);
                }
            }
        }
        currentScore = scores.size() > 0 ? currentScore - (currentScore - *min_element(scores.begin(), scores.end())) : currentScore;
        
   }
  
    int numSpots = 0;
    for (auto row: grid) {
        for (auto cell: row) {
            if (cell == 'O') numOfSpots++;
        }
    }

    return numOfSpots;
}

int getScoreOffset(const pair<int,int>& cell, const pair<int,int>&next, const pair<int,int>& source, pair<int,int> dir, pair<int,int> sourceDir) {
      // Source has dir (0,0), exclude that.
    if (cell != source) {
        pair<int,int> prevDir = { 
            cell.first - next.first,
            cell.second - next.second
        };

        if (prevDir != dir) {
           //  printf("Turning (%d,%d) -> (%d,%d)\n", prevDir.first, prevDir.second, dir.first, dir.second);
            return 1000;
        }
    } else {
        if (sourceDir != dir) {
            return 1000;
        }
    }

    return 0;
}
// Dijkstra modified
long dijkstra(grid_t& grid, 
    pair<int,int> sourceDir, 
    pair<int,int> source,
    vector<vector<uint64_t>>& distMap,
    vector<vector<pair<int,int>>>& prevMap,
    pair<int,int> target
    ) {
    auto cmp = [](const pair<pair<int,int>, uint64_t>& lhs, const pair<pair<int,int>, uint64_t>& rhs) {
        return lhs.second < rhs.second;
    };

    priority_queue<
        pair<pair<int,int>, uint64_t>,
        vector<pair<pair<int,int>,uint64_t>>, 
        decltype(cmp)>
    q(cmp);
    
    q.push({source, 0});
    
    for (int i=0; i<grid.size(); i++) {
        for (int j=0; j<grid[0].size(); j++) {
            if (i != source.first && j != source.second)
            q.push({{ i, j }, UINT64_MAX});
        }
    }

    // Distance (score) from source to the cell
    distMap[source.first][source.second] = 0;

    // Best precedent node 
    

    vector<pair<int,int>> dirs = { {0, 1}, {1,0}, {0,-1}, {-1,0}};

    while (!q.empty()) {
        auto cell = q.top().first;
        
        q.pop();
        // if (grid[cell.row][cell.col] == 'E') {
        //     minDist = min(minDist, cell.dist);
        //     continue;
        // }
      
        for (auto d: dirs) {
            pair<int,int> next = { cell.first + d.first, cell.second + d.second }; 

            if (!isValid(next.first, next.second, grid)) {
                continue;
            }


            uint64_t offset = 1 + getScoreOffset(cell, prevMap[cell.first][cell.second], source, d, sourceDir);
            
        
            uint64_t newDist = distMap[cell.first][cell.second] + offset;
            //printf("(%d,%d)->(%d,%d) Curr Dist = %llu, Alt Dist = %llu\n",
            // cell.first, cell.second, next.first, next.second, distMap[next.first][next.second], newDist);
            if (newDist < distMap[next.first][next.second]) {
                distMap[next.first][next.second] = newDist;
                prevMap[next.first][next.second] = cell;
                q.push({next, newDist});
            }
                
        }
    }


   return distMap[target.first][target.second];
}
int main(int argc, char** argv) {
    ifstream file(argv[1]);
    string line;

    grid_t grid;
    visited_t visited; 
    pair<int,int> source;
    pair<int,int> target;

    int  i=0;
    while (getline(file, line)) {
        vector<bool> visitedRow;
        
        vector<char> gridRow; 
        for (int j=0; j<line.size(); j++) {
            gridRow.push_back(line[j]);
            visitedRow.push_back(line[j] == '#');

            if (line[j] == 'S') {
                source = { i, j };
            }
            if (line[j] == 'E') {
                target = { i, j };
            }
        }
        grid.push_back(gridRow);
        visited.push_back(visitedRow);
        i++;
    }
    
    vector<vector<uint64_t>> distMap ( grid.size(), vector<uint64_t>(grid[0].size(), INT64_MAX));
    vector<vector<pair<int,int>>> prevMap (grid.size(), vector<pair<int,int>>(grid[0].size()));
    
    cout << "Running BFS..." << endl;
    auto score = dijkstra(grid, {0,1}, source, distMap, prevMap, target);
    //printGrid(grid);
    cout << "[Part1] Result is " << score << endl;

    
    //bfs 
    queue<pair<int,int>> q;
    q.push(target);
    vector<pair<int,int>> dirs = { {0, 1}, {1,0}, {0,-1}, {-1,0}};
    vector<vector<uint64_t>> distMap2 ( grid.size(), vector<uint64_t>(grid[0].size(), INT64_MAX));
    vector<vector<pair<int,int>>> prevMap2 (grid.size(), vector<pair<int,int>>(grid[0].size()));
    
    pair<int,int> targetDir = { 
                    -(target.first - prevMap[target.first][target.second].first),
                    -(target.second - prevMap[target.first][target.second].second)
                };
    dijkstra(grid,targetDir, target, distMap2, prevMap2, target);

    int count = 0;
    
    while (!q.empty()) {
        auto cell = q.front();
        q.pop();
        count++;
       
        for (auto d: dirs) {
            pair<int,int> next = { cell.first + d.first, cell.second + d.second }; 
            if (!isValid(next.first, next.second, grid) || visited[next.first][next.second]== true) {
                continue;
            }
            
            int offset = 0;
            pair<int,int> diff = {prevMap[next.first][next.second].first - next.first, prevMap[next.first][next.second].second - next.second};
            printf("Dir (%d,%d) - Curr Dir (%d,%d)\n", diff.first, diff.first, d.first, d.second);
            if (diff != d) {
                offset = 1000;
            }
            if (distMap[next.first][next.second] + distMap2[next.first][next.second] + offset == score) {
                visited[next.first][next.second] = true;
                grid[next.first][next.second] = 'O';
                q.push(next);
            } else {
                printf("(%d,%d) Score=%ld. Forwrd Score Score =%llu, Back Score= %llu. Offset=%d\n",next.first, next.second,
                score,  distMap[next.first][next.second],
                 distMap2[next.first][next.second], offset);

            }
        }    
        cout << endl << endl;   
    }
    printGrid(grid);
    cout << "[Part2] Result is " << count << endl;

}

// pt2: 672 wrong


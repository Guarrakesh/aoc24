#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <format>
#include <queue>
#include <cmath>

using namespace std;

typedef vector<vector<char>> grid_t;
struct vector2d_t{ 
    int x;
    int y;
    bool operator==(const vector2d_t &other) const {
        return x == other.x && y == other.y;
    }
} ;
typedef vector<vector2d_t> moves_t;

struct key_hash {
    
    std::size_t operator() (const vector2d_t &key) const {
        return hash<int>{}(key.x) ^ hash<int>{}(key.y);
    }
};

void printGrid(grid_t& grid) {
    for (auto row: grid) {
        for (auto c: row) cout << c;
        cout << endl;
    }
}
void parseInput(ifstream& file, grid_t& grid, moves_t& moves, vector2d_t& startPos) {
    string line;

    int i = 0, j = 0;
    while (getline(file, line) && !line.empty()) {
        vector<char> row;
        j = 0;
        for (auto c: line) {
            row.push_back(c);

            if (c == '@') {
                startPos = { i, j };
            }
            j++;
        }
        grid.push_back(row);
        i++;
    }
    
    while (getline(file, line)) {
        for (auto move: line) {
            vector2d_t dir;
            if (move == '>') dir = { 0, 1 };
            else if (move == 'v') dir = { 1, 0 };
            else if (move == '<') dir = { 0, -1 };
            else if (move == '^') dir = { -1, 0 };

            moves.push_back(dir);
        }
    }
  
}

bool canMoveBox(vector2d_t dir, grid_t& grid, vector2d_t currPos) {
    queue<vector2d_t> q;
    unordered_map<vector2d_t,bool, key_hash> visited;

    visited[currPos] = true;
    q.push(currPos);
    // cout << "Running BFS on " << currPos.x << " , " << currPos.y << endl;
    while (!q.empty()) {
        auto pos = q.front();
        q.pop();

        // printf("Current (%d,%d) = %c\n", pos.x, pos.y, grid[pos.x][pos.y]);
        // printf("Queue size: %lu\n", q.size());
        vector2d_t next1 {pos.x + dir.x, pos.y + dir.y};
        char cell1 = grid[next1.x][next1.y];
        if (cell1 == '#') return false;

        if (cell1 == ']') {
            char cell2 = '[';
            vector2d_t next2 = { next1.x, next1.y-1 };
            if (visited.find(next2) == visited.end() && visited.find(next1) == visited.end()) {
                visited[next2] = true;
                visited[next1] = true;
                q.push(next1);
                q.push(next2);
            }   

        } else if (cell1 == '[') {
            char cell2 = ']';
            vector2d_t next2 = { next1.x, next1.y+1 };
            if (visited.find(next2) == visited.end() && visited.find(next1) == visited.end()) {
                visited[next2] = true;
                visited[next1] = true;
                q.push(next1);
                q.push(next2);
            }
        }




    }

    return true;
}

vector2d_t doMove(vector2d_t& dir, grid_t& grid, vector2d_t prevPos) {
    
    stack<pair<vector2d_t, char>> s;
    unordered_map<vector2d_t,bool, key_hash> visited;
    s.push({prevPos, '@'});
    while (!s.empty()) {
        auto curr = s.top();
        auto currPos = curr.first;
        auto currCell = curr.second;
           
        //cout << "Stack Top: " << endl;
        //printf("(%d,%d)=%c\n", currPos.x, currPos.y, currCell);
        vector2d_t next = { currPos.x + dir.x, currPos.y + dir.y };
        auto nextCell = grid[next.x][next.y];

        
        if (nextCell == 'O' && visited.find(next) == visited.end()) {
            s.push({ next, nextCell });
        } else if (canMoveBox(dir, grid, currPos) && nextCell == '[' && visited.find(next) == visited.end()) {
            vector2d_t closing = { next.x , next.y + 1 };
            if (visited.find(closing) == visited.end()) {
                s.push({next, nextCell});
                s.push({closing, ']'});
            }
        }else if (canMoveBox(dir, grid, currPos) && nextCell == ']' && visited.find(next) == visited.end()) {
            vector2d_t closing = { next.x , next.y -1 };
            if (visited.find(closing) == visited.end()) {

                s.push({next, nextCell});
                s.push({closing, '['});
            }
        } else {
            s.pop();
            visited[currPos] = true;
            if (nextCell == '.') {
                grid[next.x][next.y] = currCell;
                grid[currPos.x][currPos.y] = '.';
                if (currCell == '@') {
                    prevPos = next;
                }
            }
        }

       //  printGrid(grid); cout << endl;
    }

    return prevPos;

  

}

void buildGrid2(grid_t& grid1, grid_t& grid2, vector2d_t& startPos2) {
    for (int i=0; i<grid1.size(); i++) {
        for (int j=0,j2=0; j<grid1[0].size(); j++) {
            if (grid1[i][j] == '#') {
                grid2[i][j2] = '#';
                grid2[i][j2+1] = '#';
            } else if (grid1[i][j] == '.') {
                grid2[i][j2] = '.';
                grid2[i][j2+1] = '.';
            } else if (grid1[i][j] == '@') {
                grid2[i][j2] = '@';
                startPos2 = { i, j2 };
                grid2[i][j2+1] = '.';
            } else if (grid1[i][j] == 'O') {
                grid2[i][j2] = '[';
                grid2[i][j2+1] = ']';
            }

            j2 +=2;
        }

    
    }
}
int main(int argc, char** argv) {
    ifstream file(argv[1]);

    grid_t grid;

    moves_t moves;
    vector2d_t startPos; 
    vector2d_t startPos2; 
    parseInput(file, grid, moves, startPos);
    printf("Grid is %lux%lu. There are %lu moves. Start Pos = (%d,%d)\n", 
    grid.size(), grid[0].size(), moves.size(), startPos.x, startPos.y);
    
    
    grid_t grid2 { grid.size() , vector<char>(grid[0].size() * 2) };

    buildGrid2(grid, grid2, startPos2);
    printGrid(grid2);


    // Part 1
    // for (int i=0; i<moves.size(); i++) {
    //     // printf("[%d] Move: (%d,%d)\n", i, moves[i].x,moves[i].y);
    //     startPos = doMove(moves[i], grid, startPos);
    // }

    // long long sum = 0;
    // for (int i=0; i<grid.size(); i++) {
    //     for (int j=0; j<grid[0].size(); j++) {
    //         if (grid[i][j] == 'O') {
    //             sum+= 100 * i + j;
    //         }
    //     }
    // }

    // cout << "[Part1] Sum is " << sum <   < endl;

    
    for (int i=0; i<moves.size(); i++) {
        printf("[%d] Move: (%d,%d)\n", i, moves[i].x,moves[i].y);
        printf("After move %d\n", i+1);
        startPos2 = doMove(moves[i], grid2, startPos2);

    }
    long long sum = 0;
    for (int i=0; i<grid2.size(); i++) {
        for (int j=0; j<grid2[0].size(); j++) {
            if (grid2[i][j] == '[') {
                sum+= 100 * i + j;
            }
        }
    }

    cout << "[Part1] Sum is " << sum << endl;

}
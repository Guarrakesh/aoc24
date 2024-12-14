#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <format>
#include <queue>
#include <cmath>
#include <numeric>
#include <algorithm>

using namespace std;

typedef struct {
    int left;
    int top;
    int velX;
    int velY;

} robot_t;

// Thanks to https://stackoverflow.com/a/44505823
typedef unsigned char pixval_t;
void saveImage(vector<vector<int>>& grid, int seconds) {
    auto numToPixel = [](int num) -> pixval_t {
        if (num > 0) {
            return 255;
        }
      return 0;
    };
    string name = "p14_" + to_string(seconds) + ".pgm";
    

    ofstream out(name, ios::binary | ios::out | ios::trunc);
    string rows = to_string(grid.size());
    string cols = to_string(grid[0].size());
    out << "P5\n" << rows << " " << cols << "\n255\n";
    for (auto row: grid) {
        for (auto c: row) {
            const pixval_t pixval = numToPixel(c);
            const char outpv = static_cast<const char>(pixval);
            out.write(&outpv, 1);
        }
    }
}


void parseFile(ifstream& file, vector<robot_t>& robots) {
    string line;
    while (getline(file, line)) {
        robot_t robot;
        line.erase(0,2); // erase p=
        
        size_t posComma = line.find(',');
        size_t posV = line.find('v');

       
        robot.left = stoi(line.substr(0,posComma));
        robot.top = stoi(line.substr(posComma+1, posV-2));

        line.erase(0, posV); // erase up to v
        posComma = line.find(','); // second ,
        robot.velX = stoi(line.substr(2, posComma));
        robot.velY = stoi(line.substr(posComma+1));

        robots.push_back(robot);
    }
}

void placeRobot(robot_t r, int seconds, vector<vector<int>>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();

    int newTop = r.top + seconds * r.velY;
    int newLeft = r.left + seconds * r.velX;
    int x = newLeft > 0 ? newLeft % cols : (cols - (abs(newLeft) % cols)) % cols;
    int y = newTop > 0 ? newTop % rows : (rows - (abs(newTop) % rows)) % rows;
    grid[abs(y)][abs(x)] += 1;
}

/**
 * Divide the matrix in blocks and return the max entropy across all blocks.
 */
double getEntropy(vector<vector<int>>& grid, int blocks) {
    size_t rows = grid.size();
    size_t cols = grid[0].size();
    size_t strideRow = floor(rows/(blocks/2));
    size_t strideCol = floor(cols/(blocks/2));
    size_t blockSize = strideRow * strideCol;
    double maxEntropy = 0;
    
    for (size_t by = 0; by < rows; by += strideRow+1) {
        for (size_t bx = 0; bx < cols; bx += strideCol+1) {
          
            size_t count0 = 0;
            size_t count1 = 0;
            for (size_t i = 0; i<strideRow; i++) {
                for (size_t j=0; j<strideCol; j++) {
                    if (grid[i+by][j+bx] == 0) 
                        count0++;
                    else count1++;
                }
            }

            // Calc entropy
            double p_0 = static_cast<double>(count0) / (blockSize);
            double p_1 = static_cast<double>(count1) / (blockSize);
            double entropyVal = 0.0;
            if (p_0 > 0) {
                entropyVal -= p_0 * std::log2(p_0);
            }
            if (p_1 > 0) {
                entropyVal -= p_1 * std::log2(p_1);
            }
            
            maxEntropy = max(maxEntropy, entropyVal);
        }
    }
  
    return maxEntropy;

}

int main(int argc, char** argv) {
    ifstream file(argv[1]);
   
    vector<robot_t> robots;
    string line;
    getline(file, line);
    size_t commaPos = line.find(',');


    size_t blocks = argc > 2 ? atoi(argv[2]) : 8;
    
    int rows = stoi(line.substr(0,commaPos));
    int cols = stoi(line.substr(commaPos+1));
   
    parseFile(file, robots);
    vector<vector<int>> grid ( static_cast<size_t>(rows), vector<int>(cols, 0));
    for (auto r: robots) {
        placeRobot(r, 23, grid);
    }

    int middleH = floor(cols / 2);
    int middleV = floor(rows / 2);
    int sum = 1;

    

    double totalEntropy = 0;
    for (size_t by = 0; by < rows; by += middleV+1) {
        for (size_t bx = 0; bx < cols; bx += middleH+1) {
            int pSum = 0;
            for (size_t i = 0; i<middleV; i++) {
                for (size_t j=0; j<middleH; j++) {
                    pSum+= grid[i+by][j+bx];
                }
            }

            sum *= pSum;
        }
        
    }
  
    cout << "[Part 1] Result = " << sum << endl;
    
   
    int numIterations = argc > 3 ? atoi(argv[3]) : 10000;
    double treshold = argc > 4 ? atof(argv[4]) : 0.7;
    
    for (int i=0; i<numIterations; i++) {
        vector<vector<int>> grid ( static_cast<size_t>(rows), vector<int>(cols, 0));
        for (auto r: robots) {
            placeRobot(r, i, grid);
            
        }
        
        auto entropy = getEntropy(grid, blocks);
        if (entropy > treshold) {
            printf("[%d] Max Entropy = %f\n", i, entropy);
            saveImage(grid, i);
        }

    }
    
}
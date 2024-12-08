#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <format>
#include <cmath>
#include <iostream>
using namespace std;

bool isValid(pair<int, int> node, int rows, int cols)
{
    return node.first >= 0 && node.first < rows && node.second >= 0 && node.second < cols;
}

void addToAntinodes(pair<int, int> node, map<pair<int, int>, bool> &antinodes, vector<vector<char>> &grid, int &numAntinodes)
{
    if (!isValid(node, grid.size(), grid[0].size()))
    {
        // printf("(%d,%d) out of bounds\n", node.first, node.second);
        return;
    }

    auto it = antinodes.find(node);
    if (it == antinodes.end())
    {
        antinodes[node] = true;
        numAntinodes++;
        //printf("Adding (%d,%d)\n", node.first, node.second);
    }
    else
    {
         //printf("(%d,%d) exists already.\n", node.first, node.second);
    }
}

void printGrid(vector<vector<char>> &grid, map<pair<int, int>, bool> &antinodes)
{
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[0].size(); j++)
        {
            auto it = antinodes.find(make_pair(i, j));
            if (it != antinodes.end() && grid[i][j] == '.')
            {
                cout << '#';
            }
            else
            {
                cout << grid[i][j];
            }
        }
        cout << endl;
    }

    cout << endl;
}

int solve(map<char, vector<pair<int, int>>>& antennas, vector<vector<char>>& grid, bool withResonance)
{
    map<pair<int, int>, bool> antinodes;
    int numAntinodes = 0;
    // we iterate over all the antenna types
    for (auto it = antennas.begin(); it != antennas.end(); ++it)
    {
        // for each antenna types
        // we iterate over its nodes
        // we copy the vector of the positions
        // because we're going to modify it
        auto v{it->second};
        while (v.size() > 0)
        {
            // now, starting from the last one,
            // we iterate match each position
            // with the rest of the nodes
            // then we pop it and do the same
            // until the vector  becomes empty
            auto pair = v.back();

            for (int i = 0; i < v.size() - 1; i++)
            {
                // for how the antennas map is constructed
                // and because pair is the back of the vector,
                // v[i].first (row-coordinate) always comes first pair.second
                // so deltaRow always > 0
                int deltaCol = pair.second - v[i].second;
                int deltaRow = pair.first - v[i].first;

                std::pair<int, int> pos1, pos2;
                // deltaCol > 0 --> pair to the right
                // deltaCol < 0 --> pair to the left
                // deltaCol == 0 --> same column
                int direction = deltaCol > 0 ? -1 : deltaCol == 0 ? 0
                                                                  : 1;

                int tmpDeltaRow = deltaRow;
                int tmpDeltaCol = deltaCol;
                int mult = 1;
                // printf("[%c] (%d,%d)->(%d,%d)\n", it->first, pair.first, pair.second, v[i].first, v[i].second);
                
                while (tmpDeltaRow < grid.size() && abs(tmpDeltaCol) < grid[0].size()) 
                {
                    // cout << "-- Mult = " << mult << endl;
                    pos1 = make_pair(v[i].first - tmpDeltaRow, v[i].second + (abs(tmpDeltaCol) * direction));
                    pos2 = make_pair(pair.first + tmpDeltaRow, pair.second - (abs(tmpDeltaCol) * direction));
                    
                    addToAntinodes(pos1, antinodes, grid, numAntinodes);
                    addToAntinodes(pos2, antinodes, grid, numAntinodes);

                    mult++;
                    tmpDeltaRow = deltaRow * mult;
                    tmpDeltaCol = deltaCol * mult;

                    if (!withResonance) break;
                } 
                if (withResonance) {
                    addToAntinodes(pair, antinodes, grid, numAntinodes);
                    addToAntinodes(v[i], antinodes, grid, numAntinodes);
                }

   
            }

            v.pop_back();
        }
    }

    //printGrid(grid, antinodes);
    return numAntinodes;
}
int main(int argc, char **argv)
{
    fstream file(argv[1]);
    string line;

    map<char, vector<pair<int, int>>> antennas;
    vector<vector<char>> grid;

    int i = 0;
    while (getline(file, line))
    {
        vector<char> row;
        int j = 0;
        for (auto c : line)
        {

            row.push_back(c);
            if (c == '.')
            {
                j++;
                continue;
            }

            auto it = antennas.find(c);
            if (it == antennas.end())
            {
                vector<pair<int, int>> v{{make_pair(i, j++)}};
                antennas[c] = v;
            }
            else
            {
                it->second.push_back(make_pair(i, j++));
            }
        }
        i++;
        grid.push_back(row);
    }

    
    cout << "[Part1] Num antinodes = " << solve(antennas, grid, false) << endl;
    cout << "[Part2] Num antinodes = " << solve(antennas, grid, true) << endl;
}

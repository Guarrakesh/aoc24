#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <format>
#include <queue>
#include <cmath>

using namespace std;
typedef struct {
    pair<int,int> buttonA;
    pair<int,int> buttonB;
    pair<int,int> prize;
} machine_t;

void parseInput(fstream& file, vector<machine_t>& machines) {
    string line;

    while (!file.eof()) {
        machine_t machine;
        getline(file, line);
        line.erase(0, 12);
        machine.buttonA = {stoi(line.substr(0,2)), stoi(line.substr(6, 2))};
    
        getline(file, line);
        line.erase(0, 12);
        machine.buttonB = {stoi(line.substr(0,2)), stoi(line.substr(6, 2))};

        getline(file, line);
        line.erase(0,9);
        size_t comma = line.find(',');
        machine.prize = { stoi(line.substr(0,comma)), stoi(line.substr(comma+4))};
        
        machines.push_back(machine);
        if (!file.eof()) getline(file, line); // empty line
    }
}

long long solve(machine_t& machine, long long prizeOffset) {

    vector<vector<long long>> A = {
        { machine.buttonA.first, machine.buttonB.first },
        { machine.buttonA.second, machine.buttonB.second }
    };
    vector<long long> C = { machine.prize.first + prizeOffset, machine.prize.second + prizeOffset};


    long long factor = ((A[0][0] * A[1][1]) - (A[0][1] * A[1][0]));
    vector<vector<double>> B = {
        {(double)A[1][1] / factor, (double)-A[0][1] /factor},
        {(double)-A[1][0]/ factor,(double)A[0][0]/ factor }
    };
    
    vector<double> result = {
        B[0][0] * C[0] + B[0][1] * C[1], 
        B[1][0] * C[0] + B[1][1] * C[1]    
    };

    long long a = round(result[0]);
    long long b = round(result[1]);

        cout << (a * A[0][0] + b * A[0][1]) << " === " << C[0] << endl;
        cout << (a * A[1][0] + b * A[1][1]) << " === " << C[1] << endl;
    if (
        a < 0 
        || b < 0 
        || (long long)(a * A[0][0] + b * A[0][1]) != C[0] 
        || (long long)(a * A[1][0] + b * A[1][1]) != C[1]) {
        return 0;
    } else {
       return (a * 3) + b;


    }



}   
int main(int argc, char** argv) {
	fstream file(argv[1]);

    vector<machine_t> machines;
	parseInput(file, machines);
    
    long long sumTokens = 0;
    long long sumTokens2 = 0;
    for (auto m: machines) {
        
        sumTokens += solve(m, 0);
        sumTokens2 += solve(m, 10000000000000);
    }

    cout << "[Part1] Mininum tokens " << sumTokens << endl;
    cout << "[Part2] Mininum tokens " << sumTokens2 << endl;
}

//33684 too high
// 33680 too hight
// 32445 wrong
// 32363
// 32445
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <format>

#include "cpu.h"

using namespace std;



void parseFile(ifstream& file, Cpu& cpu, instructions_register_t& instr_register) {
    string line;
    

    getline(file, line);
    cpu.register_a = stoll(line.substr(line.find(':') + 2));
    getline(file, line);
    cpu.register_b = stoll(line.substr(line.find(':') + 2));
    getline(file, line);
    cpu.register_c = stoll(line.substr(line.find(':') + 2));   
    getline(file, line);
    getline(file, line);

    line.erase(line.begin(), line.begin() + line.find(':')+2);

    int pos = 0;
    while ((pos = line.find(',')) != string::npos) {
        char num = line.substr(0, pos).c_str()[0];
        instr_register.push_back(static_cast<uint64_t>(num - '0'));
        line.erase(0, pos+1);
    }   

    instr_register.push_back(static_cast<uint64_t>(line.c_str()[0] - '0'));

   

}

void printOutput(vector<string> output) {
    for (auto res: output) {
        cout << res << ",";
    }
}

void findA(uint64_t A, vector<uint64_t>& solutions, int index, instructions_register_t& instr_register, std::function<uint64_t(uint64_t)>& step) {
        cout << step(A) << " == " << instr_register[index] << endl;
        if (step(A) != instr_register[index]) {
            return;
        }

        if (index == 0) {
            solutions.push_back(A);
        } else {
            for (int i=0; i<8; i++) {
                findA(A * 8 + i, solutions, index-1, instr_register, step);
            }
        }
}

int main(int argc, char** argv) {
    ifstream file(argv[1]);

    Cpu cpu;
    instructions_register_t instr_register;

    parseFile(file, cpu, instr_register);

    cpu.loadInstructions(instr_register);
    auto output = cpu.execute();

    string result;
    for (auto res: output) {
        cout << res << ",";
        result.append(res);
    
    }
    cpu.printRegisters();

    

    

    // Steps for input (0,3,5,4,3,0)
    instr_register.clear();
    // instr_register = { 0, 3, 5, 4, 3, 0};
    // std::function<uint64_t(uint64_t)> step = [](auto A) {
    //     uint64_t B, C;
    //     A = A / 8;
    //     return A%8; 
    // };

    instr_register = { 2,4,1,1,7,5,0,3,1,4,4,5,5,5,3,0};
     std::function<uint64_t(uint64_t)> step = [](auto A) {
        uint64_t B, C;
        B = A%8; //2,4
        B = B ^ 1;  //1,1
        C = A/pow(2,B); //7,5
        A = A/8;//0,3
        B = B^4;//1,4
        B = B^C; //4.5
        return B%8; //5,5
    };
    std::cout << "\n\n\nPart 2\n";
    
    vector<uint64_t> solutions;
    int index = instr_register.size()-1;
    for (int i=0; i<8; i++) {
        findA(i, solutions, index, instr_register, step);
    }
    if (solutions.size() == 0) {
        cout << "no solutions?"<< endl;
        return 0;
    }
    cout << *min_element(solutions.begin(), solutions.end());
}


/// 4635635210 wrong
//  4635635210
// 4,0,4,7,1,2,7,1,6
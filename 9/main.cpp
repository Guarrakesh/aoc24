#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

long long checksum(vector<string>& blocks) {
    int id = 0;
    long long int sum = 0;
    for (auto str: blocks) {
        if (str == ".") continue;
        sum += id * stoi(str);
        id++;
        
    }
    return sum;

}
int main(int argc, char **argv)
{
    ifstream file(argv[1]);
    string line;

    // we can do a simple array of chars (ie string) because we would use information
    // about ID with multiple digits (>9). 
    // With an vector of strings, each file block with its ID is going to be an element of the vector
    vector<string> blocks;

    getline(file, line);
    int id = 0;
    int pos = 0;
    while (pos < line.size())
    {
        int count = line[pos] - '0';
        while (count > 0)
        {
            blocks.push_back(to_string(id));
            count--;
        }
        if (pos + 1 < line.size())
        {
            count = line[pos + 1] - '0'; // free space
            while (count > 0) {
                blocks.push_back(".");
                count--;
            }
            pos++;
        }
        pos++;
        id++;
    }

    // First step is done, now
    for (auto str: blocks) cout << str; 
    cout << endl;

    int left = 0, right = blocks.size()-1;

    // Defragment
    // Use two pointers, one at the beginning one at the end 
    // and iterate until they meet
    while (left < right) {
        // free space stays at the end
        if (blocks[right] == ".") {
            right--;
            continue;
        }
        //we keep the occupied positions on the left until we find a free space
        while (blocks[left] != ".") left++; 

        // swap letter from the end with the space on the left
        swap(blocks[left], blocks[right]);
        right--;
    }

//    for (auto str: blocks) cout << str; 
//    cout << endl;

    auto res = checksum(blocks);

    cout << "Res " << res << endl;
}
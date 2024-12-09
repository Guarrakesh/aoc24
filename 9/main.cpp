#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

long long checksum(vector<string> &blocks)
{
    long long int id = 0;
    long long int sum = 0;
    for (auto str : blocks)
    {
        if (str == ".") {
            id++;
            continue;
        }
        sum += id * stoi(str);
        id++;
    }
    return sum;
}

void compact_blocks(vector<string> &blocks)
{
    int left = 0, right = blocks.size() - 1;

    // Compactation
    // Use two pointers, one at the beginning one at the end
    // and iterate until they meet
    while (left < right)
    {

        // free space stays at the end
        if (blocks[right] == ".")
        {
            right--;
            continue;
        }
        // we keep the occupied positions on the left until we find a free space
        while (blocks[left] != ".")
            left++;

        // swap letter from the end with the space on the left
        swap(blocks[left], blocks[right]);
        right--;
    }
}

void defragment(vector<string> &blocks)
{
    int left = 0, right = blocks.size() - 1;
    while (right > 0)
    {
        if (blocks[right] == ".")
        {
            right--;
            continue;
        }

        int rightProbe = right;
        size_t fileSize = 0;
        while (blocks[rightProbe] == blocks[right])
        {
            fileSize++;
            rightProbe--;
        }
        // printf("FileSize = %zu (File = %s)\n", fileSize, blocks[right].c_str());
        size_t freeSpace = 0;
       
      
        
        int leftProbe = 0; // we count how many free spaces ('.') we have from where left is
        while (leftProbe < right && freeSpace < fileSize)
        {
            freeSpace = 0;
            if (blocks[leftProbe] != ".") {
                leftProbe++;
                continue;
            }
            while (blocks[leftProbe++] == ".") {
                freeSpace++;    
            }
          
        }

        // printf("FreeSpace=%zu\n", freeSpace);
        int left = leftProbe-freeSpace-1;
        // we have space, so move all file's file blocks to free space
        if (freeSpace >= fileSize)
        {
            for (int i = 0; i < fileSize; i++)
            {
                swap(blocks[left++], blocks[right--]);
            }
        }
        else
        {
            right -= fileSize;
        }
    }
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
            while (count > 0)
            {
                blocks.push_back(".");
                count--;
            }
            pos++;
        }
        pos++;
        id++;
    }

    // First step is done, now we compact.
    // for (auto str: blocks) cout << str;
    // cout << endl;
    vector<string> blocks2;
    copy(blocks.begin(), blocks.end(), back_inserter(blocks2));

    compact_blocks(blocks);
    defragment(blocks2);

    // for (auto str : blocks2)
    //     cout << str;
    // cout << endl;
    auto res = checksum(blocks);
    auto res2 = checksum(blocks2);

    cout << "Res 1 " << res << endl;
    cout << "Res 2 " << res2 << endl;
}
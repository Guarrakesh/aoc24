#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
 
using namespace std;

/**
 * Go backwards: start from the test number and then do backtracking
 * by performing divisions or subtractions.
 * To speed things up, ignore branch division if the modulo is non zero and branch subtraction if difference is <0
*/
bool solve(const vector<int> &nums, int i, long long int accumulator, bool concatenate)
{

    if (i == 0)
    {
        return nums[0] == accumulator;
    } 

   

    bool left = false, right = false, middle = false;

    long long int diff = accumulator - (long long int) nums[i];
    int modulo = accumulator % (long long) nums[i];
    if (diff >= 0) {
        left = solve(nums, i - 1, diff, concatenate);
    } 
    if (modulo == 0) {
        right = solve(nums, i - 1, accumulator / nums[i],concatenate);
    }

    string strAcc = to_string(accumulator);
    string strNum = to_string(nums[i]);
    int endsIdx = strAcc.rfind(strNum);
    if (concatenate && endsIdx > 0 && strAcc.ends_with(strNum)) {
        //cout << "Acc  is " << strAcc << ",num=" << strNum << endl;
        auto deconcatenated = (strAcc).substr(0, endsIdx);
        middle = solve(nums, i - 1, stoll(deconcatenated), concatenate);
    }


    return left || right || middle;
}
int main(int argc, char **argv)
{
    fstream file(argv[1]);
    string line;

    long long int sum1= 0;
    long long int sum2= 0;
    int lineNum = 0;

    while (getline(file, line))
    {

        const int colonPos = line.find(':');

        const long long int testValue = stoll(line.substr(0, colonPos));
        line.erase(0, colonPos + 2);
        vector<int> nums;

        int pos;
        while ((pos = line.find(' ')) != string::npos)
        {

            int num = stoi(line.substr(0, pos));

            nums.push_back(num);
            line.erase(0, pos + 1);
        }
        nums.push_back(stoi(line));

        if (solve(nums, nums.size() - 1, testValue, false)) sum1 += testValue;
        if (solve(nums, nums.size() - 1, testValue, true)) sum2 += testValue;
    }

    cout << "Sum1 is " << sum1 << endl;
    cout << "Sum2 is " << sum2 << endl;
}

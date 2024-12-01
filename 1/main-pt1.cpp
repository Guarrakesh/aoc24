#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>

using namespace std;

int main(int argc, char** argv) {
	ifstream file(argv[1]);

	string str;
	vector<int> v1;
	vector<int> v2;
	while (getline(file, str)) {
		int loc1 = stoi(str.substr(0, 5));
		int loc2 = stoi(str.substr(8,5));
		v1.push_back(loc1);
		v2.push_back(loc2);
	}

	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());

	long long sum = 0;
	for (int i=0; i<v1.size(); i++) {
      int distance = abs(v2[i] - v1[i]);
 	  sum += distance;
	}

	cout << "sum is " << sum << endl;
}

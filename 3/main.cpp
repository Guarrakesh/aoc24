#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include <vector>

using namespace std;

int main(int argc, char** argv) {
	ifstream file(argv[1]);

	string content;
	// thanks https://stackoverflow.com/questions/21667295/how-to-match-multiple-results-using-stdregex
	regex exp(R"((mul\(\d{1,3},\d{1,3}\))|(do\(\))|(don\'t\(\)))");
	int sum = 0;
	bool doMul = true;
	while (getline(file, content)) { 
		string::const_iterator searchStart(content.cbegin());
		smatch res;		

		sregex_iterator iter(content.begin(), content.end(), exp);
		sregex_iterator end;	
//		while (regex_search(searchStart, content.cend(), res, exp)) {
		while (iter != end) {
	//		if (searchStart == content.cbegin()) {
	//			searchStart = res.suffix().first;	
	//			continue;
	//		}
//			cout << iter->size() << " --> ";	
			for (unsigned i=0; i < iter->size(); i++) {

//			    cout << endl;		
				string match = (*iter)[i];
//				cout << match << " " ;
				if (match.size() == 0) {
				
				}
				if (match.find("do()") != string::npos) {
					doMul = true;
				//	cout << "Enabling Mul" << endl;
					break;
				} else if (match.find("don't()") != string::npos) {
					doMul = false;
				//	cout << "Disabling Mul" << endl;
					break;
				}
				
				if (!doMul) {
					continue;
				}
				// remove "mul(" and last parenthesis ")"
				match.erase(0, 4);
				match.erase(match.end()-1, match.end());
				
				const size_t commaPos = match.find(',');
				const basic_string digit1 = match.substr(0, commaPos);
				const basic_string digit2 = match.substr(commaPos+1, match.size()-commaPos);
				
				int num1 = stoi(digit1);
				int num2 = stoi(digit2);
			
				sum += num1*num2;
				break;
			}
			++iter;
			//searchStart = res.suffix().first;
		}
	}
	cout << "Sum is " << sum << endl;	
	
}

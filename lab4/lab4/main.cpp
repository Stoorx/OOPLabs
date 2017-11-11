#include "Factorer.h"
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

//#define DEBUG

using namespace std;

int main(int argc, char** argv) {
	#ifndef DEBUG
	if (argc != 3) {
		cout << "Incorrect arguments!\nUsage:\n\tfactorer inputFile outputFile";
	}
	else {
		ifstream input(argv[1]);
		ofstream output(argv[2]);
	#endif // !DEBUG

	#ifdef DEBUG
		string inName, outName;
		cin >> inName >> outName;
		ifstream input(inName);
		ofstream output(outName);	
	#endif // DEBUG

		if (input.is_open()) {
			unsigned int val;
			while (!input.eof()) {
				input >> val;
				Factorer factors(val);
				output << factors.toString() << endl;
			}
		}
#ifndef DEBUG
	}
#endif // !DEBUG
}
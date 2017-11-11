#pragma once
#include <vector>
#include <string>

using namespace std;

class Factorer {
public:
	Factorer(unsigned int value);
	const string toString();
	const vector<unsigned int>& getFactors();
	const unsigned int getValue();

	unsigned int _dbgRecalculateValue();
private:
	const string _convertInt(unsigned int value);
	
	unsigned int _value;
	vector<unsigned int> _factors;
};
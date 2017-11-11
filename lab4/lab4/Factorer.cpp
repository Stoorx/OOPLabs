#include "Factorer.h"
#include <sstream>

Factorer::Factorer(unsigned int value) {
	_value = value;
	_factors.push_back(1);

	int maxFactor = sqrt(value);
	for (int i = 2; i <= maxFactor; i++) {
		while (value % i == 0) {
			_factors.push_back(i);
			value /= i;
		}
	}

	if (value != 1) { // if the value is prime
		_factors.push_back(value);
	}

}

const string Factorer::toString() {
	string factorStr(_convertInt(_factors[0]));
	for (int i = 1; i < _factors.size(); i++) {
		factorStr += " ";
		factorStr += _convertInt(_factors[i]);
	}
	return move(factorStr);
}

const vector<unsigned int>& Factorer::getFactors() {
	return _factors;
}

const unsigned int Factorer::getValue() {
	return _value;
}

unsigned int Factorer::_dbgRecalculateValue() {
	unsigned int tmpValue = 1;
	for (int i = 0; i < _factors.size(); i++) {
		tmpValue *= _factors[i];
	}
	return tmpValue;
}

const string Factorer::_convertInt(unsigned int value) {
	std::ostringstream oss;
	oss << value;
	return move(string(oss.str()));	
}

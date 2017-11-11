#pragma once

#include <string>
#include <fstream>
#include <map>

class IniParser {
public:
	IniParser(const string &filename);
	~IniParser();
	string get(const string &section, const string &parameter) const;
private:
	map<string, map<string, string>> elements;
	void _insert(const string &section, const string &parameter, const string &value);
};




#include <string>
#include <fstream>
#include <map>
#include "IniParser.h"
#include "IParserExcept.h"


IniParser::IniParser(const string &filename) {
	ifstream file(filename);

	if (file.is_open()) {
		//cout << "File is opened.\n";
	}
	else {
		throw EParserIO(string("ERROR: Can't open the file!"));
	}

	string tmpString;
	string currentSection;

	while (!file.eof()) {
		getline(file, tmpString);
		string tmpRule;
		string tmpValue;
		if (tmpString.length() != 0) {
			bool bracket = false;
			bool isVal = false;
			
			for (int i = 0; i < tmpString.length(); ++i)
			{
				{
					if (tmpString[i] == ';')
						if (bracket)
							throw EParserFormat(string("Format error!"));
						else
							break;

					if (tmpString[i] == '[') {
						if (bracket == true)
							throw EParserFormat(string("Format error!"));
						bracket = true;
						currentSection.clear();
						continue;
					}

					if (tmpString[i] == ']') {
						if (bracket == false)
							throw EParserFormat(string("Format error!"));
						break;
					}

					if (bracket) {
						currentSection += tmpString[i];
						continue;
					}
				}//Sections

				{
					
						if (tmpString[i] == ';')
							if (!isVal)
								throw EParserFormat(string("Format error!"));
							else
								break;

						if (tmpString[i] == ' ')
							continue;
						if (tmpString[i] == '=') {
							isVal = true;
							continue;
						}
						if (!isVal)
							tmpRule += tmpString[i];
						else
							tmpValue += tmpString[i];
						
				}//data
				
				

			}
		}
		if (currentSection.length() && tmpRule.length() && tmpValue.length()) {
		//	cout << currentSection + ">" + tmpRule + "=" + tmpValue << endl;
			_insert(currentSection, tmpRule, tmpValue);
		}
		
	}
	file.close();
	//cout << "Parsing finished!\n" << endl;
}

IniParser::~IniParser() {
}

void IniParser::_insert(const string &section, const string &parameter, const string &parValue) {
	 auto it = elements.find(section);
	 if (it == elements.end()) { //if section not found
		 map<string, string> tmpMap; //create new map
		 tmpMap.insert(pair<string, string>(parameter, parValue));
		 elements.insert(pair<string, map<string, string>>(section, tmpMap));
	 }
	 else {//if section found
		 it->second.insert(pair<string, string>(parameter, parValue));
	 }
}
string IniParser::get(const string &section, const string &parameter) const {
	auto it = elements.find(section);
	if (it == elements.end()) {
		throw EParserParameterNotFound(string("There is no such SECTION!"));
	}
	else {
		auto parameterIt = it->second.find(parameter);
		if (parameterIt == it->second.end()) {
			throw EParserParameterNotFound(string("There is no such PARAMETER!"));
		}
		else {
			return parameterIt->second;
		}
	}
}
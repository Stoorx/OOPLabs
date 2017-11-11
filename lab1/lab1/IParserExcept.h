#pragma once
#include <string>

class IParserExcept {
public:
	string errorMsg;
	IParserExcept(const string &message);

};
IParserExcept::IParserExcept(const string &message){
	errorMsg = message;
}

class EParserIO : public IParserExcept{};
class EParserFormat : public IParserExcept {};
class EParserParameterNotFound : public IParserExcept {};

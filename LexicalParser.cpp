#include "LexicalParser.h"
using namespace std;
void LexicalParser::Init() {
	
	for (auto& keyword : keywords) {
		tokenStrings.resize(50);
		tokenStrings[static_cast<int>(keyword.second)] = keyword.first;
	}

	delimeters.resize(256, 0);
	for (auto x : string{ '+','-','*','/',' ',';','\t','\n','\r','>','<','=','[',']','(',')','{','}' }) {
		delimeters[x] = 1;
	}

	singleElementTerminators.resize(256, 0);
	for (auto x : string{ "=<>{}+-/*,;:[]()" }) {
		singleElementTerminators[x] = 1;
	}
}

Token LexicalParser::TokenByString(std::string tokenString)
{
	if (tokenString[0] >= 0x30 && tokenString[0] < 0x3a) {
		return Token{ Token::Type::IntergralValue,strtol(tokenString.c_str(), nullptr, 10) };
	}

	auto found = keywords.find(tokenString);
	if (found != keywords.cend()) {
		return Token{ found->second,0 };
	}
	auto result = symbolsMap.find(tokenString);
	int id = -1;
	if (result != symbolsMap.end()) {
		id = result->second;
	}
	else {
		symbols.push_back(tokenString);
		id = symbols.size() - 1;
		symbolsMap.insert({ tokenString,id });
	}
	return Token{ Token::Type::VarName,id };
}
string LexicalParser::TokenTypeToString(Token::Type type) {
	return tokenStrings[static_cast<int>(type)];
}
std::vector<Token> LexicalParser::Parse(std::string::const_iterator it, std::string::const_iterator sentinel) {
	vector<Token> tokens;
	auto itstart = it;
	bool singleElement = true;
	while (it != sentinel) {
		if (delimeters[*it] == 1) {
			if (!singleElement) {
				string tokenString{ itstart,it };
				tokens.push_back(TokenByString(string{ itstart,it }));
				itstart = it + 1;
			}
			if (singleElementTerminators[*it] == 1) {

				//two element cases
				if (*it == '<' || *it == '>' || *it == '=') {
					if (*(it + 1) == '=') {
						tokens.push_back(TokenByString(string{ it,it+2 }));
						++it;
					}
					else {
						tokens.push_back(TokenByString(string{ *it }));
					}
				}
				else
					tokens.push_back(TokenByString(string{ *it }));
			}
			singleElement = true;
			itstart = it + 1;
		}
		else {
			singleElement = false;
		}
		++it;
	}
	tokens.push_back(Token{Token::Type::None,0});
	return tokens;
}
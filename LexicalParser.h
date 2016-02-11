#pragma once
#include <vector>
#include <unordered_map>
#include <map>
#include "Token.h"
#include "SymbolsTable.h"
class LexicalParser {
public:
	void Init();

	Token TokenByString(std::string tokenString);
	std::string TokenTypeToString(Token::Type type);
	std::vector<Token> Parse(std::string::const_iterator it, std::string::const_iterator sentinel);

private:
	const std::unordered_map<std::string, Token::Type> keywords{
		{ "for",	Token::Type::For },
		{ "while",	Token::Type::While },
		{ "do",		Token::Type::Do },
		{ "if",		Token::Type::If },
		{ "break",	Token::Type::Break },
		{ "int",	Token::Type::Int },
		{ "float",	Token::Type::Float },
		{ "true",	Token::Type::True },
		{ "false",	Token::Type::False },

		{ "+",		Token::Type::AddOp },
		{ "-",		Token::Type::NegOp },
		{ "*",		Token::Type::MulOp },
		{ "/",		Token::Type::DivOp },

		{ "<",		Token::Type::LessThanOp },
		{ ">",		Token::Type::GreaterThanOp },
		{ ">=",		Token::Type::GreaterOrEqOp },
		{ "<=",		Token::Type::LessOrEqOp },
		{ "=",		Token::Type::AssignOp },
		{ "[",		Token::Type::SquareBracketL },
		{ "]",		Token::Type::SquareBracketR },
		{ "(",		Token::Type::RoundBracketL },
		{ ")",		Token::Type::RoundBracketR },

		{ "{",		Token::Type::CurlyBracketL },
		{ "}",		Token::Type::CurlyBracketR },

		{ ";",		Token::Type::Semicolon },
		{ ":",		Token::Type::Colon },
		{ ".",		Token::Type::Dot },
		{ ",",		Token::Type::Comma },
	};
	std::vector<char> delimeters;
	std::vector<char> singleElementTerminators;
	std::vector<std::string> tokenStrings;
};

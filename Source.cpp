#include <iostream>
#include "LexicalParser.h"
#include "SyntaxParser.h"
using namespace std;

char codeBlock[] =
"int i; int j; float a[100]; float v; float x;"
"while (true) {"
"	do i = i+1; while (a[i] < v );"
"	do j = i-1; while (a[j] > v );"
"	if (i>=j) break;"
"	x=a[i];a[i] = a[j]; a[j] = x;"
"}"
;

int main(){
	string codeBuffer = codeBlock;
	LexicalParser lexicalParser;
	SyntaxParser syntaxParser;
	lexicalParser.Init();
	auto tokens = lexicalParser.Parse(codeBuffer.begin(), codeBuffer.end());
	auto syntaxTree = syntaxParser.Parse(tokens);
	syntaxTree->PrintYield(0);
	int x;
	std::cin >> x;
}

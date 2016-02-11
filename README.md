# RecursiveDescentParser
Test project to obtain skills in parsing methods

This is a straight recursive descent parser, that understands basic C-style syntax like 

int i; int j; float a[100]; float v; float x;
while (true) {
	do i = i+1; while (a[i] < v );
	do j = i-1; while (a[j] > v );
	if (i>=j) break;
	x=a[i];a[i] = a[j]; a[j] = x;
}

In the code first the character string is parsed by lexical parser, that converts char stream to meaningful tokens, that are implemented as
"enum class Token". This Token enum holds most of the needed keywords (for test) like do, while, if etc, as well as 1-char operators 
like  +,-,=,< and 2-char operators like >=, <=, ==

After that the stream of tokens goes to a recursive descent parser, that creates an abstract syntax tree out of this.
So far, I have a limited wokring parser, that does not handle errors in the source code at all. 

Also I was not able to defeat the collision of left-recursion elimination and left-associative operators. So the 
expression like a+b+c in left associative manner should be deducted like (a+b)+c, instead after eliminating left-recursion I get
a+(b+c), so it's kind of a problem, i'm going to solve next


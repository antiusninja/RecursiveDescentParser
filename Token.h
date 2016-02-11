#pragma once
#include <vector>
struct Token {
	enum class Type {
	None = 0,
	Start,

	Statements,
	Statement,
		BoolExpression,
	Expression,

	VariableDeclaration,
	ArrayDeclarator,

	IntergralValue,
	VarName,
	For,
	While,
	Do,
	If,
	Break,
	Int,
	Float,
	True,
	False,

	LessThanOp,
	GreaterThanOp,
	GreaterOrEqOp,
	LessOrEqOp,
	EqualOp,

	AddOp,
	NegOp,
	MulOp,
	DivOp,

	AssignOp,
	SquareBracketL,
	SquareBracketR,
	RoundBracketL,
	RoundBracketR,

	CurlyBracketL,
	CurlyBracketR,

	Semicolon,
	Colon,
	Dot,
	Comma
};
	Type type;
	int id;
};



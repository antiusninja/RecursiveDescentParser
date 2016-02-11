#include "SyntaxParser.h"
using namespace std;
SyntaxParser::TokensIt SyntaxParser::it;

unique_ptr<SyntaxParser::AddNode> SyntaxParser::NRAdd() {
	if (it->type != Token::Type::AddOp && it->type != Token::Type::NegOp) {
		return nullptr;
	}
	auto addNode = make_unique<AddNode>();
	addNode->op = it->type;
	++it;
	addNode->term = Term();
	addNode->add = NRAdd();
	return addNode;
}
unique_ptr<SyntaxParser::AddNode> SyntaxParser::Add() {
	auto addNode = make_unique<AddNode>();
	addNode->term = Term();
	addNode->add = NRAdd();
	return addNode;
}
unique_ptr<SyntaxParser::RelNode> SyntaxParser::NRRel() {
	auto relNode = make_unique<RelNode>();
	switch (it->type) {
	case Token::Type::LessOrEqOp:
	case Token::Type::LessThanOp:
	case Token::Type::GreaterOrEqOp:
	case Token::Type::GreaterThanOp:
	case Token::Type::EqualOp:
		relNode->op = it->type;
		++it;
		relNode->add = Add();
		relNode->rel = NRRel();
		break;
	default:
		return nullptr;
	}
	return relNode;
}
unique_ptr<SyntaxParser::RelNode> SyntaxParser::Rel() {
	auto relNode = make_unique<RelNode>();
	relNode->add=Add();
	relNode->rel = NRRel();
	return relNode;
}

unique_ptr<SyntaxParser::FactorNode> SyntaxParser::Factor() {
	auto returnNode = make_unique<FactorNode>();
	if (it->type == Token::Type::RoundBracketL) {
		returnNode->expr = Expression();
		Match(Token::Type::RoundBracketR);
	}
	else 
		returnNode->id = Id();
	return returnNode;
}

unique_ptr<SyntaxParser::ArrayAccessNode> SyntaxParser::NRArrayAccess() {
	auto node = make_unique<ArrayAccessNode>();
	if (it->type != Token::Type::SquareBracketL) {
		return nullptr;
	}
	Match(Token::Type::SquareBracketL);
	NRArrayAccess();
	Match(Token::Type::SquareBracketR);
	return node;
}

unique_ptr<SyntaxParser::ArrayAccessNode> SyntaxParser::ArrayAccess() {
	auto node = make_unique<ArrayAccessNode>();
	node->factor = Factor();
	if (it->type == Token::Type::SquareBracketL) {
		++it;
		node->arrayAccessExpr = Expression();
		Match(Token::Type::SquareBracketR);
	}
	node->arrayAccess=NRArrayAccess();
	
	return node;
}
unique_ptr<SyntaxParser::TermNode> SyntaxParser::NRTerm() {
	if (it->type != Token::Type::MulOp && it->type != Token::Type::DivOp) {
		return nullptr;
	}
	auto termNode = make_unique<TermNode>();
	termNode->op = it->type;
	++it;
	termNode->arrayAccess= ArrayAccess();
	return termNode;
}

unique_ptr<SyntaxParser::TermNode> SyntaxParser::Term() {
	auto termNode = make_unique<TermNode>();
	termNode->arrayAccess = ArrayAccess();
	termNode->term = NRTerm();
	return termNode;
}

unique_ptr<SyntaxParser::ExprNode> SyntaxParser::Expression() {
	auto returnNode = make_unique<ExprNode>();
	returnNode->rel = Rel();
	if (it->type == Token::Type::AssignOp) {
		returnNode->op = it->type;
		++it;
		returnNode->expr = Expression();
	}
	return returnNode;
}
unique_ptr<SyntaxParser::IdNode> SyntaxParser::Id() {
	auto returnNode = make_unique<IdNode>();
	if (it->type == Token::Type::VarName) {
		returnNode->idType = IdType::Variable;
		returnNode->value = it->id;
	}
	else if (it->type == Token::Type::IntergralValue) {
		returnNode->idType = IdType::IntegralValue;
		returnNode->value = it->id;
	}
	else if (it->type == Token::Type::True) {
		returnNode->idType = IdType::BoolValue;
		returnNode->value = 1;
	}
	else throw SyntaxParserError();
	++it;
	return returnNode;
}



unique_ptr<SyntaxParser::StmtsNode> SyntaxParser::NRStatements() {
	auto stmtsNode = make_unique<StmtsNode>();
	auto stmt = Statement();
	if (!stmt)
		return nullptr;
	stmtsNode->stmt = move(stmt);
	stmtsNode->stmts = NRStatements();
	return stmtsNode;
}
unique_ptr<SyntaxParser::StmtsNode> SyntaxParser::Statements() {
	auto stmtsNode = make_unique<StmtsNode>();
	stmtsNode->stmt = Statement();
	stmtsNode->stmts = NRStatements();
	return stmtsNode;
}

unique_ptr<SyntaxParser::DeclNode> SyntaxParser::Declaration() {
	auto declNode = make_unique<DeclNode>();
	if (it->type != Token::Type::Int && it->type != Token::Type::Float) {
		return nullptr;
	}
	declNode->type = it->type;
	++it;
	auto idNode = Id();
	declNode->symbolId = idNode->value;
	if (it->type == Token::Type::SquareBracketL) {
		++it;
		auto numberNode = Id();
		declNode->arraySize = numberNode->value;
		Match(Token::Type::SquareBracketR);
	}
	else {
		declNode->arraySize = 0;
	}
	if (it->type == Token::Type::AssignOp) {
		++it;
		declNode->initStatement = Statement();
	}
	else {
		Match(Token::Type::Semicolon);
	}
	return declNode;
}
unique_ptr<SyntaxParser::StmtNode> SyntaxParser::Statement() {
	unique_ptr<StmtNode> returnNode;
	switch (it->type) {
	case Token::Type::None:
		return nullptr;
	case Token::Type::Int:
	case Token::Type::Float:
		returnNode = Declaration();
		break;
	case Token::Type::Do:
		returnNode = Do();
		break;
	case Token::Type::While:
		returnNode = While();
		break;
	case Token::Type::Break:
		returnNode = make_unique<BreakNode>();
		Match(Token::Type::Break);
		Match(Token::Type::Semicolon);
		break;
	case Token::Type::If:
		returnNode = If();
		break;
	case Token::Type::CurlyBracketL:
		returnNode = Block();
		break;
	case Token::Type::IntergralValue:
	case Token::Type::VarName:
		returnNode = make_unique<StmtNode>();
		returnNode->expr = Expression();
		Match(Token::Type::Semicolon);
		break;
	}
	return returnNode;
}
unique_ptr<SyntaxParser::BlockNode> SyntaxParser::Block() {
	auto blockNode = make_unique<BlockNode>();
	Match(Token::Type::CurlyBracketL);
	blockNode->statements = Statements();
	Match(Token::Type::CurlyBracketR);
	return blockNode;
}
unique_ptr<SyntaxParser::IfNode> SyntaxParser::If() {
	auto node = make_unique<IfNode>();
	Match(Token::Type::If);
	Match(Token::Type::RoundBracketL);
	node->expr = Expression();
	Match(Token::Type::RoundBracketR);
	node->stmt = Statement();
	return node;
}
std::unique_ptr<SyntaxParser::DoNode> SyntaxParser::Do() {
	auto node = make_unique<DoNode>();
	Match(Token::Type::Do);
	node->stmt = Statement();
	Match(Token::Type::While);
	Match(Token::Type::RoundBracketL);
	node->expr = Expression();
	Match(Token::Type::RoundBracketR);
	Match(Token::Type::Semicolon);
	return node;
}
unique_ptr<SyntaxParser::StmtNode> SyntaxParser::While(){
	Match(Token::Type::While);
	Match(Token::Type::RoundBracketL);
	auto expr = Expression();
	Match(Token::Type::RoundBracketR);
	auto stmt = Statement();
	return make_unique<WhileNode>(move(stmt),move(expr));
}
unique_ptr<SyntaxParser::Node> SyntaxParser::Parse(const std::vector<Token>& tokens) {
	unique_ptr<Node> node;
	it = tokens.cbegin();
	node = Statements();
	return node;
}

unique_ptr<SyntaxParser::FileNode> SyntaxParser::File() {
	return nullptr;
}
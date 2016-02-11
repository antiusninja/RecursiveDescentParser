#pragma once
#include <memory>
#include <stack>
#include <iostream>
#include "Token.h"
#include "SymbolsTable.h"
class SyntaxParser {
public:
	typedef std::vector<Token> Tokens;
	typedef Tokens::const_iterator TokensIt;
	class SyntaxParserError{};


	struct StmtNode;
	static void Match(Token::Type type) {
			if (it->type != type) {
				throw SyntaxParserError();
			}
			++it;
		}
	struct Node {
		virtual void PrintYield(int tab) {
			std::cout << "BaseNode\n";
		}
		virtual void Print() {
			std::cout << "BaseNode\n";
		}
	};
	enum class IdType {
		Variable,
		IntegralValue,
		BoolValue,
	};
	struct IdNode :public Node {
		void PrintYield(int tab)override {
			switch (idType) {
			case IdType::BoolValue:
				if (value == 0) std::cout << "False";
				else std::cout << "True";
				break;
			case IdType::IntegralValue:
				std::cout << value;
				break;
			case IdType::Variable:
				std::cout << symbols[value].c_str();
				break;
			}
		}
		void Print()override {
			std::cout << "IdNode of type:";
			switch (idType) {
				case IdType::Variable:
					std::cout << "Variable";
					break;
				case IdType::IntegralValue:
					std::cout << "IntegralValue";
					break;
				case IdType::BoolValue:
					std::cout << "BoolValue";
					break;
				default:
					throw SyntaxParserError();
			}
			std::cout << " Value: " << value << std::endl;
		}
		IdType idType;
		int value;
	};

	struct ExprNode;
	struct FactorNode:public Node{
		void Print()override{
			std::cout << "FactorNode: ";
			id->Print();
			if (expr) {
				expr->Print();
			}
		}
		void PrintYield(int tab)override {
			id->PrintYield(tab);
			if (expr)
				expr->PrintYield(tab);
		}

		std::unique_ptr<IdNode> id;
		std::unique_ptr<ExprNode> expr;

	};
	
	struct ArrayAccessNode :public Node {
		void PrintYield(int tab)override {
			factor->PrintYield(0);
			if (arrayAccessExpr) {
				std::cout << '[';
				arrayAccessExpr->PrintYield(0);
				std::cout << ']';
			}
			if (arrayAccess)
				arrayAccess->PrintYield(0);
		}
		std::unique_ptr<FactorNode> factor;
		std::unique_ptr<ExprNode> arrayAccessExpr;
		std::unique_ptr<ArrayAccessNode> arrayAccess;
	};
	
	struct TermNode : public Node{
		void PrintYield(int tab)override {
			arrayAccess->PrintYield(tab);
			switch (op) {
			case Token::Type::MulOp:
				std::cout << " * ";
				break;
			case Token::Type::DivOp:
				std::cout << " / ";
				break;
			case Token::Type::None:
				break;
			default:
				throw SyntaxParserError();
			}
			if (term)
				term->PrintYield(tab);
		}
		void Print()override {
			std::cout << "TermNode: ";
			arrayAccess->Print();
			switch (op) {
			case Token::Type::MulOp:
				std::cout << " * ";
				break;
			case Token::Type::DivOp:
				std::cout << " / ";
				break;
			case Token::Type::None:
				break;
			default:
				throw SyntaxParserError();
			}
			if (term) {
				term->Print();
			}
		}
		Token::Type op{Token::Type::None};
		std::unique_ptr<ArrayAccessNode> arrayAccess;
		std::unique_ptr<TermNode> term;
	};

	struct AddNode :public Node {
		void PrintYield(int tab)override {
			switch (op) {
			case Token::Type::AddOp:
				std::cout << " + ";
				break;
			case Token::Type::NegOp:
				std::cout << " - ";
				break;
			case Token::Type::None:
				break;
			default:
				throw SyntaxParserError();
			}
			term->PrintYield(tab);
			if (add)
				add->PrintYield(tab);
		}

		void Print()override {
			std::cout << "AddNode: ";
			term->Print();
			switch (op) {
			case Token::Type::AddOp:
				std::cout << " + ";
				break;
			case Token::Type::NegOp:
				std::cout << " - ";
				break;
			case Token::Type::None:
				break;
			default:
				throw SyntaxParserError();
			}
			if (add) {
				add->Print();
			}
		}

		Token::Type op;
		std::unique_ptr<AddNode> add;
		std::unique_ptr<TermNode> term;
	};

	struct RelNode:public Node{
		void PrintYield(int tab)override {
			switch (op) {
			case Token::Type::LessOrEqOp:
				std::cout << " <= ";
				break;
			case Token::Type::LessThanOp:
				std::cout << " < ";
				break;
			case Token::Type::GreaterOrEqOp:
				std::cout << " >= ";
				break;
			case Token::Type::GreaterThanOp:
				std::cout << " > ";
				break;
			case Token::Type::EqualOp:
				std::cout << " == ";
				break;
			case Token::Type::None:
				break;
			default:
				throw SyntaxParserError();
			}
			add->PrintYield(tab);
			if (rel) rel->PrintYield(tab);
		}

		void Print()override {
			std::cout << "RelNode: ";
			add->Print();
			switch (op) {
			case Token::Type::LessOrEqOp:
				std::cout << " <= ";
				break;
			case Token::Type::LessThanOp:
				std::cout << " < ";
				break;
			case Token::Type::GreaterOrEqOp:
				std::cout << " >= ";
				break;
			case Token::Type::GreaterThanOp:
				std::cout << " > ";
				break;
			case Token::Type::EqualOp:
				std::cout << " == ";
				break;
			case Token::Type::None:
				break;
			default:
				throw SyntaxParserError();
			}
			if (rel) {
				rel->Print();
			}
		}
		Token::Type op;
		std::unique_ptr<AddNode> add;
		std::unique_ptr<RelNode> rel;
	};

	struct ExprNode :public Node {
		void PrintYield(int tab)override {
			rel->PrintYield(tab);
			switch (op) {
			case Token::Type::AssignOp:
				std::cout << " = ";
				break;
			case Token::Type::None:
				break;
			default:
				throw SyntaxParserError();
			}
			if (expr) expr->PrintYield(tab);
		}
		void Print()override {
			std::cout << "ExprNode: ";
			rel->Print();
			switch (op) {
			case Token::Type::AssignOp:
				std::cout << " = ";
				break;
			case Token::Type::None:
				break;
			default:
				throw SyntaxParserError();
			}
			if (expr) {
				expr->Print();
			}
		}

		Token::Type op;
		std::unique_ptr<ExprNode> expr;
		std::unique_ptr<RelNode> rel;
	};
	struct StmtNode:public Node {
		void PrintYield(int tab)override {
			expr->PrintYield(tab);
			std::cout << ";";
		}
		void Print()override {
			std::cout << "StmtNode: ";
			expr->Print();
		}
		std::unique_ptr<ExprNode> expr;
	};
	struct StmtsNode :public Node {
		void PrintYield(int tab)override {
			for (int i = 0; i < tab; ++i) std::cout << '\t';
			stmt->PrintYield(tab);
			if (stmts) {
				std::cout << '\n';
				stmts->PrintYield(tab);

			}
		}
		void Print()override {
			std::cout << "StmtsNode: ";
			stmt->Print();
			if (stmts)
				stmts->Print();
		}
		std::unique_ptr<StmtNode> stmt;
		std::unique_ptr<StmtsNode> stmts;
	};

	struct DeclNode:public StmtNode {
		void PrintYield(int tab) {
			for (int i = 0; i < tab; ++i) std::cout << '\t';
			switch (type) {
			case Token::Type::Int:
				std::cout << "int "; break;
			case Token::Type::Float:
				std::cout << "float "; break;
			default:
				throw SyntaxParserError();
			}
			std::cout << ' ' << symbols[symbolId].c_str();
			if (arraySize) {
				std::cout << "[" << arraySize << "]";
			}
			if (initStatement) {
				initStatement->PrintYield(tab);
			}
			else {
				std::cout << ";\n";
			}
		}
		Token::Type type;
		int arraySize;
		int symbolId;
		std::unique_ptr<StmtNode> initStatement;
	};

	struct IfNode :public StmtNode {
		void PrintYield(int tab)override {
			for (int i = 0; i < tab; ++i) std::cout << '\t';
			std::cout << "if (";
			expr->PrintYield(tab);
			std::cout << ") ";
			stmt->PrintYield(tab);
			std::cout << '\n';
		}
		std::unique_ptr<ExprNode> expr;
		std::unique_ptr<StmtNode> stmt;
	};
	struct BlockNode :public StmtNode {
		void PrintYield(int tab)override {
			std::cout << " {\n";
			statements->PrintYield(tab+1);
			std::cout << "\n}";
		}
		void Print()override {
			std::cout << "BlockNode: ";
			if (statements)
				statements->Print();
		}
		std::unique_ptr<StmtsNode> statements;
	};

	struct DoNode :public StmtNode {
		void PrintYield(int tab) {
			for (int i = 0; i < tab; ++i) std::cout << '\t';
			std::cout << "do ";
			stmt->PrintYield(0);
			std::cout << " while (";
			expr->PrintYield(0);
			std::cout << ");\n";
		}
		std::unique_ptr<StmtNode> stmt;
		std::unique_ptr<ExprNode> expr;
	};
	struct WhileNode :public StmtNode {
		void PrintYield(int tab)override {
			for (int i = 0; i < tab; ++i) std::cout << '\t';
			std::cout << "while ( ";
			expr->PrintYield(tab);
			std::cout << " ) ";
			stmt->PrintYield(tab);
		}
		void Print()override {
			std::cout << "WhileNode: ";
			expr->Print();
			stmt->Print();
		}
		WhileNode(){}
		WhileNode(std::unique_ptr<StmtNode> stmt, std::unique_ptr<ExprNode> expr) :stmt{ std::move(stmt) }, expr{std::move(expr)} {}
		std::unique_ptr<StmtNode> stmt;
		std::unique_ptr<ExprNode> expr;
	};
	struct BreakNode :public StmtNode {
		void PrintYield(int tab)override {
			std::cout << "break;";
		}
	};
	struct FileNode:public Node{
		std::unique_ptr<StmtsNode> stmts;
	};
	std::unique_ptr<Node> Parse(const std::vector<Token>& tokens);
	static TokensIt it;


private:
	std::unique_ptr<DoNode> Do();
	std::unique_ptr<FileNode> File();
	std::unique_ptr<DeclNode> Declaration();

	std::unique_ptr<IdNode> Id();

	std::unique_ptr<ArrayAccessNode> NRArrayAccess();
	std::unique_ptr<ArrayAccessNode> ArrayAccess();

	std::unique_ptr<FactorNode> Factor();

	std::unique_ptr<TermNode> NRTerm();
	std::unique_ptr<TermNode> Term();

	std::unique_ptr<AddNode> NRAdd();
	std::unique_ptr<AddNode> Add();

	std::unique_ptr<RelNode> NRRel();
	std::unique_ptr<RelNode> Rel();

	std::unique_ptr<ExprNode> Expression();

	std::unique_ptr<StmtNode> Statement();

	std::unique_ptr<StmtsNode> NRStatements();
	std::unique_ptr<StmtsNode> Statements();
	std::unique_ptr<BlockNode> Block();


	std::unique_ptr<IfNode> If();
	std::unique_ptr<StmtNode> While();

	std::vector<Token>::const_iterator tokensEnd;
};

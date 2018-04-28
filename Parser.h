#pragma once
#include "Lexer.h"
#include "Semantics.h"

#include <deque>
#include <vector>

class Type;
class Expression;
class Statement;
class Declaration;
class Program;

using TypeList = std::vector<Type*>;
using ExpressionList = std::vector<Expression*>;
using StatementList = std::vector<Statement*>;
using DeclarationList = std::vector<Declaration*>;

class Parser {
public:
	Parser(SymbolTable& symbols, const File& file);

	Type* parseType();
	Type* parseBasicType();

	Expression* parseExpr();
	Expression* parseAssignmentExpr();
	Expression* parseConditionalExpr();
	Expression* parseLogicalOrExpr();
	Expression* parseLogicalAndExpr();
	Expression* parseBitwiseOrExpr();
	Expression* parseBitwiseXOrExpr();
	Expression* parseBitwiseAndExpr();
	Expression* parseEqualityExpr();
	Expression* parseRelationalExpr();
	Expression* parseShiftExpr();
	Expression* parseAdditiveExpr();
	Expression* parseMultiplicativeExpr();
	Expression* parseCastExpr();
	Expression* parseUnaryExpr();
	Expression* parsePostfixExpr();
	Expression* parsePrimaryExpr();

	ExpressionList parseArgumentList();

	Statement* parseStatement();
	Statement* parseBlockStatement();
	Statement* parseIfStatement();
	Statement* parseWhileStatement();
	Statement* parseBreakStatement();
	Statement* parseContinueStatement();
	Statement* parseReturnStatement();
	Statement* parseDeclarationStatement();
	Statement* parseExpressionStatement();
	
	StatementList parseStatementSequence();

	Declaration* parseDeclaration();
	Declaration* parseLocalDeclaration();
	Declaration* parseObjectDefinition();
	Declaration* parseVariableDefinition();
	Declaration* parseConstantDefinition();
	Declaration* parseValueDefinition();
	Declaration* parseFunctionDefinition();
	Declaration* parseParameter();

	DeclarationList parseParameterList();
	DeclarationList parseParameterClause();
	DeclarationList parseDeclarationSequence();

	Declaration* parseProgram();

private:
	TokenName lookahead();
	TokenName lookahead(int n);
	Token match(TokenName name);
	Token matchIf(TokenName name);
	Token matchLogicalOr();
	Token matchLogicalAnd();
	Token matchBitwiseOr();
	Token matchBitwiseXOr();
	Token matchBitwiseAnd();
	Token matchEquality();
	Token matchRelational();
	Token matchShift();
	Token matchAdditive();
	Token matchMultiplicative();

	Token accept();
	Token peek();
	void fetch();

	Lexer lex;

	std::deque<Token> tok;

	Semantics action;
};
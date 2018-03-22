#pragma once
#include "Lexer.h"
#include <deque>

class Parser {
public:
	Parser(SymbolTable& symbols, const File& file);

	void parseType();

	void parseExpr();
	void parseAssignmentExpr();
	void parseConditionalExpr();
	void parseLogicalOrExpr();
	void parseLogicalAndExpr();
	void parseBitwiseOrExpr();
	void parseBitwiseAndExpr();
	void parseEqualityExpr();
	void parseRelationalExpr();
	void parseShiftExpr();
	void parseAdditiveExpr();
	void parseMultiplicativeExpr();
	void parseCastExpr();
	void parseUnaryExpr();
	void parsePostfixExpr();
	void parsePrimaryExpr();

	void parseStatement();

	void parseDeclaration();
	void parseLocalDeclaration();
	void parseObjectDef();
	void parseVariableDef();
	void parseConstantDef();
	void parseValueDef();
	void parseFunctionDef();
	void parseDeclarationSequence();

	void parseProgram();

private:
	TokenName lookahead();
	TokenName lookahead(int n);
	Token match(TokenName name);
	Token matchIf(TokenName name);
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
};
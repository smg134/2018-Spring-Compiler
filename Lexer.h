#pragma once
#include "Token.h"
#include <unordered_map>

class File;

class Lexer {
public:
	Lexer(SymbolTable& s, const File& f);
	Token operator()() { return scan(); }
	Token scan();
	bool eof() const;
	char peek() const;
	char peek(int n) const;

private:
	char accept();
	void accept(int n);
	char ignore();
	
	void skipSpace();
	void skipNewline();
	void skipComment();

	Token lexPunctuator(TokenName n);
	Token lexRelationalOperator(int length, RelationalOperator r);
	Token lexArithmeticOperator(ArithmeticOperator a);
	Token lexArithmeticOperator(int length, ArithmeticOperator a);
	Token lexBitwiseOperator(int length, BitwiseOperator b);
	Token lexConditionalOperator();
	Token lexAssignmentOperator();
	Token lexCompoundAssignmentOperator(int length, CompoundAssignmentOperator c);
	Token lexWord();
	Token lexNumber();
	Token lexBinNumber();
	Token lexHexNumber();
	Token lexChar();
	Token lexString();

	char scanEscapeSequence();

	SymbolTable symbolTable;
	const char* first;
	const char* last;
	Location currentLocation;
	Location tokenLocation;
	std::unordered_map<Symbol,Token> reserved;
};
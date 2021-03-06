#include "stdafx.h"
#include "Lexer.h"
#include "File.h"
#include <cassert>
#include <cctype>
#include <sstream>
#include <iostream>

static bool isSpace(char c) {
	return c == ' ' || c == '\t';
}

static bool isNewline(char c) {
	return c == '\n';
}

static bool isNondigit(char c) {
	return std::isalpha(c) || c == '_';
}

static bool isDigit(char c) {
	return std::isdigit(c);
}

static bool isAlphanumeric(char c) {
	return std::isalnum(c);
}

static bool isBinaryDigit(char c) {
	return c == '0' || c == '1';
}

static bool isHexDigit(char c) {
	return std::isxdigit(c);
}

static const char* getStartOfInput(const File& f) {
	return f.getText().data();
}

static const char* getEndOfInput(const File& f) {
	return f.getText().data() + f.getText().size();
}

Lexer::Lexer(SymbolTable& s, const File& f)
	: symbolTable(s),
	first(getStartOfInput(f)),
	last(getEndOfInput(f)),
	currentLocation(f, 0, 0) {
	reserved.insert({
		//Keywords
		{symbolTable.get("as"), kw_as },
		{symbolTable.get("break"), kw_break},
		{symbolTable.get("continue"), kw_continue},
		{symbolTable.get("def"), kw_def},
		{symbolTable.get("else"), kw_else},
		{symbolTable.get("if"), kw_if},
		{symbolTable.get("let"), kw_let},
		{symbolTable.get("var"), kw_var},
		{symbolTable.get("return"), kw_return},
		{symbolTable.get("while"), kw_while},
		//Operators
		{symbolTable.get("and"), op_logicAnd},
		{symbolTable.get("or"), op_logicOr},
		{symbolTable.get("not"), op_logicNot},
		{symbolTable.get("true"), true},
		{symbolTable.get("false"), false},
		{symbolTable.get("bool"), type_bool},
		{symbolTable.get("char"), type_char},
		{symbolTable.get("int"), type_int},
		{symbolTable.get("float"), type_float}
	});
}

bool Lexer::eof() const {
	return first == last;
}

char Lexer::peek() const {
	return eof() ? 0 : *first;
}

char Lexer::peek(int n) const {
	if (n < last - first) {
		return *(first + n);
	}
	else {
		return 0;
	}
}

char Lexer::accept() {
	assert(*first != '\n');
	char c = *first;
	++first;
	++currentLocation.column;
	return c;
}

void Lexer::accept(int n) {
	while (n) {
		accept();
		--n;
	}
}

char Lexer::ignore() {
	assert(*first != '\n');
	char c = *first;
	++first;
	++currentLocation.column;
	return c;
}

Token Lexer::scan() {
	while (!eof()) {
		tokenLocation = currentLocation;
		switch (*first) {
		case ' ':
		case '\t':
			skipSpace();
			continue;
		case '\n':
			skipNewline();
			continue;
		case '#':
			skipComment();
			continue;
		case '(':
			return lexPunctuator(tok_left_paren);
		case ')':
			return lexPunctuator(tok_right_paren);
		case '[':
			return lexPunctuator(tok_left_bracket);
		case ']':
			return lexPunctuator(tok_right_bracket);
		case '{':
			return lexPunctuator(tok_left_brace);
		case '}':
			return lexPunctuator(tok_right_brace);
		case ',':
			return lexPunctuator(tok_comma);
		case ';':
			return lexPunctuator(tok_semicolon);
		case ':':
			return lexPunctuator(tok_colon);
		case '<':
			if (peek(1) == '=') {
				return lexRelationalOperator(2, op_lessEqual);
			}
			if (peek(1) == '<') {
				return lexBitwiseOperator(2, op_shiftLeft);
			}
			return lexRelationalOperator(1, op_lessThan);
		case '>':
			if (peek(1) == '=') {
				return lexRelationalOperator(2, op_greaterEqual);
			}
			if (peek(1) == '>') {
				return lexBitwiseOperator(2, op_shiftRight);
			}
			return lexRelationalOperator(1, op_greaterThan);
		case '=':
			if (peek(1) == '=') {
				return lexRelationalOperator(2, op_equal);
			}
			else {
				return lexAssignmentOperator();
			}
		case '+':
			if (peek(1) == '=') {
				return lexCompoundAssignmentOperator(2, op_addEq);
			}
			if (peek(1) == '+') {
				return lexArithmeticOperator(2, op_inc); //TODO: discriminate prefix/postfix
			}
			else {
				return lexArithmeticOperator(op_add);
			}
		case '-':
			if (peek(1) == '>') {
				return lexArrowOperator();
			}
			if (peek(1) == '=') {
				return lexCompoundAssignmentOperator(2, op_subEq);
			}
			if (peek(1) == '-') {
				return lexArithmeticOperator(2, op_dec); //TODO: discriminate prefix/postfix
			}
			return lexArithmeticOperator(op_sub);
		case '*':
			if (peek(1) == '=') {
				return lexCompoundAssignmentOperator(2, op_mulEq);
			}
			return lexArithmeticOperator(op_mul);
		case '/':
			if (peek(1) == '=') {
				return lexCompoundAssignmentOperator(2, op_divEq);
			}
			return lexArithmeticOperator(op_div);
		case '%':
			if (peek(1) == '=') {
				return lexCompoundAssignmentOperator(2, op_modEq);
			}
			return lexArithmeticOperator(op_mod);
		case '&':
			return lexBitwiseOperator(1, op_bitAnd);
		case '|':
			return lexBitwiseOperator(1, op_bitOr);
		case '^':
			return lexBitwiseOperator(1, op_bitXOR);
		case '~':
			return lexBitwiseOperator(1, op_bitNot);
		case '?':
			return lexConditionalOperator();
		case '\'':
			return lexChar();
		case '"':
			return lexString();
		default: {
			if (isNondigit(*first)) {
				return lexWord();
			}
			if (isdigit(*first)) {
				return lexNumber();
			}
			std::stringstream ss;
			ss << "Invalid character '" << *first << '\'';
			throw std::runtime_error(ss.str());
		}
		}
	}
	return {};
}

void Lexer::skipSpace() {
	assert(isSpace(*first));
	ignore();
	while (!eof() && isSpace(*first)) {
		ignore();
	}
}

void Lexer::skipNewline() {
	assert(*first == '\n');
	currentLocation.line += 1;
	currentLocation.column = 0;
	++first;
}

void Lexer::skipComment() {
	assert(*first == '#');
	ignore();
	while (!eof() && !isNewline(*first)) {
		ignore();
	}
}

Token Lexer::lexPunctuator(TokenName n) {
	accept();
	return { n, tokenLocation };
}

Token Lexer::lexRelationalOperator(int length, RelationalOperator r) {
	accept(length);
	return { r, tokenLocation };
}

Token Lexer::lexArithmeticOperator(ArithmeticOperator a) {
	accept();
	return { a, tokenLocation };
}

Token Lexer::lexArithmeticOperator(int length, ArithmeticOperator a) {
	accept(length);
	return { a, tokenLocation };
}

Token Lexer::lexBitwiseOperator(int length, BitwiseOperator b) {
	accept(length);
	return { b, tokenLocation };
}

Token Lexer::lexConditionalOperator() {
	accept();
	return { tok_conditional_operator, tokenLocation };
}

Token Lexer::lexAssignmentOperator() {
	accept();
	return { tok_assignment_operator, tokenLocation };
}

Token Lexer::lexArrowOperator() {
	accept(2);
	return { tok_arrow_operator, tokenLocation };
}

Token Lexer::lexCompoundAssignmentOperator(int length, CompoundAssignmentOperator c) {
	accept(length);
	return { c, tokenLocation};
}

Token Lexer::lexWord() {
	assert(isNondigit(*first));
	const char* start = first;
	accept();
	while (!eof() && isAlphanumeric(*first)) {
		accept();
	}

	std::string str(start, first);
	Symbol sym = symbolTable.get(str);
	auto iter = reserved.find(sym);
	if (iter != reserved.end()) {
		const Token& token = iter->second;
		return { token.getName(), token.getAttribute(), tokenLocation };
	}
	else {
		return { sym, tokenLocation };
	}
}

Token Lexer::lexNumber() {
	assert(isDigit(*first));
	const char* start = first;
	if (*first == '0') {
		char pre = peek(1);
		switch (pre) {
		case 'b': case 'B':
			return lexBinNumber();
		case 'x': case 'X':
			return lexHexNumber();
		default:
			break;
		}
	}
	accept();
	while (!eof() && isDigit(*first)) {
		accept();
	}
	if (peek() != '.') {
		std::string str(start, first);
		return { dec, std::atoi(str.c_str()), tokenLocation };
	}
	accept();
	while (!eof() && isDigit(*first)) {
		accept();
	}
	std::string str(start, first);
	return { std::atof(str.c_str()), tokenLocation };
}

Token Lexer::lexBinNumber() {
	accept(2);
	const char* start = first;
	while (!eof() && isBinaryDigit(*first)) {
		accept();
	}
	std::string str(start, first);
	return { bin, std::strtoll(str.c_str(), nullptr, 2), tokenLocation };
}

Token Lexer::lexHexNumber() {
	accept(2);
	const char* start = first;
	while (!eof() && isHexDigit(*first)) {
		accept();
	}
	std::string str(start, first);
	return { hex, std::strtoll(str.c_str(), nullptr, 16), tokenLocation };
}

static bool isCharChar(char c) {
	return c != '\n' && c != '\'';
}

char Lexer::scanEscapeSequence() {
	assert(*first == '\\');
	accept();
	if (eof()) {
		throw std::runtime_error("Unterminated escape sequence");
	}
	switch (accept()) {
	case '\'': 
		return '\'';
	case '\"':
		return '\"';
	case '\\':
		return '\\';
	case 'a':
		return '\a';
	case 'b':
		return '\b';
	case 'f':
		return '\f';
	case 'n':
		return '\n';
	case 'r':
		return '\r';
	case 't':
		return '\t';
	case 'v':
		return '\v';
	default:
		throw std::runtime_error("Invalid escape sequence");
	}
}

Token Lexer::lexChar() {
	assert(*first == '\'');
	accept();
	if (eof()) {
		throw std::runtime_error("Unterminated character literal");
	}
	char c;
	if (*first == '\\') {
		c = scanEscapeSequence();
	}
	else if (isCharChar(*first)) {
		c = accept();
	}
	else if (*first == '\'') {
		throw std::runtime_error("Invalid character literal");
	}
	else if (*first == '\n') {
		throw std::runtime_error("Invalid multi-line character");
	}
	else {
		throw std::logic_error("Unexpected character");
	}
	if (*first != '\'') {
		throw std::runtime_error("Invalid multi-byte character");
	}
	accept();
	return { c, tokenLocation };
}

static bool isStringChar(char c) {
	return c != '\n' && c != '"';
}

Token Lexer::lexString() {
	assert(*first == '"');
	accept();
	if (eof()) {
		throw std::runtime_error("Unterminated character literal");
	}
	std::string str;
	str.reserve(32);
	while (*first != '"') {
		char c;
		if (*first == '\\') {
			c = scanEscapeSequence();
		}
		else if (isStringChar(*first)) {
			c = accept();
		}
		else if (*first == '\n') {
			throw std::runtime_error("Invalid multi-line string");
		}
		str += c;
	}
	accept();

	return { StringAttribute{symbolTable.get(str)}, tokenLocation };
}
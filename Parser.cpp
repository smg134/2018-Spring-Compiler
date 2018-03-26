#include "stdafx.h"
#include "Parser.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

TokenName Parser::lookahead() {
	assert(!tok.empty());
	return tok.front().getName();
}

TokenName Parser::lookahead(int n) {
	if (n < tok.size()) {
		return tok[n].getName();
	}
	n = n - tok.size() + 1;
	while (n != 0) {
		fetch();
		n--;
	}
	return tok.back().getName();
}

Token Parser::match(TokenName name) {
	if (lookahead() == name) {
		return accept();
	}
	std::stringstream ss;
	ss << peek().getLocation() << ": syntax error.";
	throw std::runtime_error(ss.str());
}

Token Parser::matchIf(TokenName name) {
	if (lookahead() == name) {
		return accept();
	}
	else {
		return {};
	}
}

Token Parser::matchEquality() {
	if (lookahead() == tok_relational_operator) {
		switch (peek().getRelationalOperator()) {
		default:
			return {};
		case op_equal:
		case op_notEqual:
			return accept();
		}
	}
	return {};
}

Token Parser::matchRelational() {
	if (lookahead() == tok_relational_operator) {
		switch (peek().getRelationalOperator()) {
		default:
			return {};
		case op_lessThan:
		case op_greaterThan:
		case op_lessEqual:
		case op_greaterEqual:
			return accept();
		}
	}
	return {};
}

Token Parser::matchShift() {
	if (lookahead() == tok_bitwise_operator) {
		switch (peek().getBitwiseOperator()) {
		default:
			return {};
		case op_shiftLeft:
		case op_shiftRight:
			return accept();
		}
	}
	return {};
}

Token Parser::matchAdditive() {
	if (lookahead() == tok_arithmetic_operator) {
		switch (peek().getArithmeticOperator()) {
		default:
			return {};
		case op_add:
		case op_sub:
			return accept();
		}
	}
	return {};
}

Token Parser::matchMultiplicative() {
	if (lookahead() == tok_arithmetic_operator) {
		switch (peek().getArithmeticOperator()) {
		default:
			return {};
		case op_mul:
		case op_div:
		case op_mod:
			return accept();
		}
	}
	return {};
}

Token Parser::accept() {
	Token token = peek();
	tok.pop_front();
	if (tok.empty()) {
		fetch();
	}
	return token;
}

Token Parser::peek() {
	assert(!tok.empty());
	return tok.front();
}

void Parser::fetch() {
	tok.push_back(lex());
}

Parser::Parser(SymbolTable& symbols, const File& file)
	: lex(symbols, file), tok() {
	fetch();
}

void Parser::parseExpr() {
	return parseAssignmentExpr();
}

void Parser::parseAssignmentExpr() {
	return parseConditionalExpr();
}

void Parser::parseConditionalExpr() {
	return parseLogicalOrExpr();
}

void Parser::parseLogicalOrExpr() {
	return parseLogicalAndExpr();
}

void Parser::parseLogicalAndExpr() {
	return parseBitwiseOrExpr();
}

void Parser::parseBitwiseOrExpr() {
	return parseBitwiseAndExpr();
}

void Parser::parseBitwiseAndExpr() {
	return parseEqualityExpr();
}

void Parser::parseEqualityExpr() {
	return parseRelationalExpr();
}

void Parser::parseRelationalExpr() {
	return parseShiftExpr();
}

void Parser::parseShiftExpr() {
	return parseAdditiveExpr();
}

void Parser::parseAdditiveExpr() {
	return parseMultiplicativeExpr();
}

void Parser::parseMultiplicativeExpr() {
	return parseCastExpr();
}

void Parser::parseCastExpr() {
	return parseUnaryExpr();
}

void Parser::parseUnaryExpr() {
	return parsePostfixExpr();
}

void Parser::parsePostfixExpr() {
	return parsePrimaryExpr();
}

void Parser::parsePrimaryExpr() {
	switch (lookahead()) {
	case tok_binary_integer:
	case tok_decimal_integer:
	case tok_hexadecimal_integer:
	case tok_boolean:
	case tok_floating_point:
	case tok_character:
	case tok_string:
	case tok_identifier:
		accept();
		return;

	case tok_left_paren: {
		match(tok_left_paren);
		parseExpr();
		match(tok_right_paren);
		return;
	}

	default:
		break;
	}

	throw std::runtime_error("Expected primary expression");
}

void Parser::parseStatement()
{
}

void Parser::parseDeclaration()
{
}

void Parser::parseLocalDeclaration()
{
}

void Parser::parseObjectDef()
{
}

void Parser::parseVariableDef()
{
}

void Parser::parseConstantDef()
{
}

void Parser::parseValueDef()
{
}

void Parser::parseFunctionDef()
{
}

void Parser::parseDeclarationSequence()
{
}

void Parser::parseProgram()
{
}



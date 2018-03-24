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
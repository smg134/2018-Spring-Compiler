#pragma once
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
	ss << peek().getLocation() << ": syntax error";
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

Token Parser::matchLogicalOr() {
	if (lookahead() == tok_logical_operator) {
		if (peek().getLogicalOperator() == op_logicOr) {
			return accept();
		}
	}
	return {};
}

Token Parser::matchLogicalAnd() {
	if (lookahead() == tok_logical_operator) {
		if (peek().getLogicalOperator() == op_logicAnd) {
			return accept();
		}
	}
	return {};
}

Token Parser::matchBitwiseOr() {
	if (lookahead() == tok_bitwise_operator) {
		if (peek().getBitwiseOperator() == op_bitOr) {
			return accept();
		}
	}
	return {};
}

Token Parser::matchBitwiseXOr() {
	if (lookahead() == tok_bitwise_operator) {
		if (peek().getBitwiseOperator() == op_bitXOR) {
			return accept();
		}
	}
	return {};
}

Token Parser::matchBitwiseAnd() {
	if (lookahead() == tok_bitwise_operator) {
		if (peek().getBitwiseOperator() == op_bitAnd) {
			return accept();
		}
	}
	return {};
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

Type* Parser::parseType() {
	return parseBasicType();
}

Type* Parser::parseBasicType() {
	switch (lookahead()) {
	case tok_type_specifier:
		return action.onBasicType(accept());
	case tok_left_paren: {
		match(tok_left_paren);
		Type* t = parseType();
		match(tok_right_paren);
		return t;
	}
	default:
		throw std::runtime_error("Expected basic type");
	}
}

Expression* Parser::parseExpr() {
	return parseAssignmentExpr();
}

Expression* Parser::parseAssignmentExpr() {
	Expression* e1 = parseConditionalExpr();
	if (matchIf(tok_assignment_operator)) {
		Expression* e2 = parseAssignmentExpr();
		return action.onAssignmentExpression(e1, e2);
	}
	return e1;
}

Expression* Parser::parseConditionalExpr() {
	Expression* e1 = parseLogicalOrExpr();
	if (matchIf(tok_conditional_operator)) {
		Expression* e2 = parseConditionalExpr();
		match(tok_colon);
		Expression* e3 = parseConditionalExpr();
		return action.onConditationalExpression(e1, e2, e3);
	}
	return e1;
}

Expression* Parser::parseLogicalOrExpr() {
	Expression* e1 = parseLogicalAndExpr();
	while (matchLogicalOr()) {
		Expression* e2 = parseLogicalAndExpr();
		e1 = action.onLogicalOrExpression(e1, e2);
	}
	return e1;
}

Expression* Parser::parseLogicalAndExpr() {
	Expression* e1 = parseBitwiseOrExpr();
	while (matchLogicalAnd()) {
		Expression* e2 = parseBitwiseOrExpr();
		e1 = action.onLogicalAndExpression(e1, e2);
	}
	return e1;
}

Expression* Parser::parseBitwiseOrExpr() {
	Expression* e1 = parseBitwiseXOrExpr();
	while (matchBitwiseOr()) {
		Expression* e2 = parseBitwiseXOrExpr();
		e1 = action.onBitwiseOrExpression(e1, e2);
	}
	return e1;
}

Expression* Parser::parseBitwiseXOrExpr() {
	Expression* e1 = parseBitwiseAndExpr();
	while (matchBitwiseXOr()) {
		Expression* e2 = parseBitwiseAndExpr();
		e1 = action.onBitwiseXOrExpression(e1, e2);
	}
	return e1;
}

Expression* Parser::parseBitwiseAndExpr() {
	Expression* e1 = parseEqualityExpr();
	while (matchBitwiseAnd()) {
		Expression* e2 = parseEqualityExpr();
		e1 = action.onBitwiseAndExpression(e1, e2);
	}
	return e1;
}

Expression* Parser::parseEqualityExpr() {
	Expression* e1 = parseRelationalExpr();
	while (Token t = matchEquality()) {
		Expression* e2 = parseRelationalExpr();
		e1 = action.onEqualityExpression(t, e1, e2);
	}
	return e1;
}

Expression* Parser::parseRelationalExpr() {
	Expression* e1 = parseShiftExpr();
	while (Token t = matchRelational()) {
		Expression* e2 = parseShiftExpr();
		e1 = action.onRelationalExpression(t, e1, e2);
	}
	return e1;
}

Expression* Parser::parseShiftExpr() {
	Expression* e1 = parseAdditiveExpr();
	while (Token t = matchShift()) {
		Expression* e2 = parseAdditiveExpr();
		e1 = action.onShiftExpression(t, e1, e2);
	}
	return e1;
}

Expression* Parser::parseAdditiveExpr() {
	Expression* e1 = parseMultiplicativeExpr();
	while (Token t = matchAdditive()) {
		Expression* e2 = parseMultiplicativeExpr();
		e1 = action.onAdditiveExpression(t, e1, e2);
	}
	return e1;
}

Expression* Parser::parseMultiplicativeExpr() {
	Expression* e1 = parseCastExpr();
	while (Token t = matchMultiplicative()) {
		Expression* e2 = parseCastExpr();
		e1 = action.onMultiplicativeExpression(t, e1, e2);
	}
	return e1;
}

Expression* Parser::parseCastExpr() {
	Expression* e = parseUnaryExpr();
	if (matchIf(kw_as)) {
		Type* t = parseType();
		return action.onCastExpression(e, t);
	}
	return e;
}

Expression* Parser::parseUnaryExpr() {
	Token t;
	switch (lookahead()) {
	case tok_arithmetic_operator:
		switch (peek().getArithmeticOperator()) {
		case op_add:
		case op_sub:
		case op_mul:
			t = accept();
			break;
		default:
			break;
		}

	case tok_bitwise_operator:
		switch (peek().getBitwiseOperator()) {
		case op_bitNot:
		case op_bitAnd:
			t = accept();
			break;
		default:
			break;
		}

	case tok_logical_operator:
		if (peek().getLogicalOperator() == op_logicNot) {
			t = accept();
		}
		break;

	default:
		break;
	}

	if (t) {
		Expression* e = parseUnaryExpr();
		return action.onUnaryExpression(t, e);
	}

	return parsePostfixExpr();
}

Expression* Parser::parsePostfixExpr() {
	Expression* e = parsePrimaryExpr();
	while (true) {
		if (matchIf(tok_left_paren)) {
			ExpressionList args = parseArgumentList();
			match(tok_right_paren);
			e = action.onCallExpression(e, args);
		}
		else if (matchIf(tok_left_bracket)) {
			ExpressionList args = parseArgumentList();
			match(tok_right_bracket);
			e = action.onIndexExpression(e, args);
		}
		else {
			break;
		}
	}
	return e;
}

Expression* Parser::parsePrimaryExpr() {
	switch (lookahead()) {
	case tok_binary_integer:
	case tok_decimal_integer:
	case tok_hexadecimal_integer:
		return action.onIntegerLiteral(accept());
	case tok_boolean:
		return action.onBooleanLiteral(accept());
	case tok_floating_point:
		return action.onFloatLiteral(accept());
	case tok_character:
	case tok_string:
		//todo
		return {};
	case tok_identifier:
		return action.onIdExpression(accept());
	case tok_left_paren: {
		match(tok_left_paren);
		Expression* e = parseExpr();
		match(tok_right_paren);
		return e;
	}

	default:
		break;
	}

	throw std::runtime_error("Expected primary expression");
}

ExpressionList Parser::parseArgumentList() {
	ExpressionList args;
	while (true) {
		Expression* arg = parseExpr();
		args.push_back(arg);
		if (matchIf(tok_comma)) {
			continue;
		}
		if (lookahead() == tok_right_paren) {
			break;
		}
		if (lookahead() == tok_right_brace) {
			break;
		}
	}
	return args;
}

Statement* Parser::parseStatement() {
	switch (lookahead()) {
	case kw_if:
		return parseIfStatement();
	case kw_while:
		return parseWhileStatement();
	case kw_break:
		return parseBreakStatement();
	case kw_continue:
		return parseContinueStatement();
	case kw_return:
		return parseReturnStatement();
	case kw_var:
	case kw_let:
	case kw_def:
		return parseDeclarationStatement();
	case tok_left_brace:
		return parseBlockStatement();
	default:
		return parseExpressionStatement();
	}
}

Statement* Parser::parseBlockStatement() {
	match(tok_left_brace);
	action.enterBlockScope();
	action.startBlock();

	StatementList s;
	if (lookahead() != tok_right_brace) {
		s = parseStatementSequence();
	}

	action.finishBlock();
	action.leaveScope();
	match(tok_right_brace);
	return action.onBlockStatement(s);
}

Statement* Parser::parseIfStatement() {
	assert(lookahead() == kw_if);
	accept();
	match(tok_left_paren);
	Expression* e = parseExpr();
	match(tok_right_paren);
	Statement* s = parseStatement();
	match(kw_else);
	Statement* t = parseStatement();
	return action.onIfStatement(e, s, t);
}

Statement* Parser::parseWhileStatement() {
	assert(lookahead() == kw_while);
	accept();
	match(tok_left_paren);
	Expression* e = parseExpr();
	match(tok_right_paren);
	Statement* b = parseStatement();
	return action.onWhileStatement(e, b);
}

Statement* Parser::parseBreakStatement() {
	assert(lookahead() == kw_break);
	accept();
	match(tok_semicolon);
	return action.onBreakStatement();
}

Statement* Parser::parseContinueStatement() {
	assert(lookahead() == kw_continue);
	accept();
	match(tok_semicolon);
	return action.onContinueStatement();
}

Statement* Parser::parseReturnStatement() {
	assert(lookahead() == kw_return);
	accept();
	Expression* e = parseExpr();
	match(tok_semicolon);
	return action.onReturnStatement(e);
}

Statement* Parser::parseDeclarationStatement() {
	Declaration* d = parseLocalDeclaration();
	return action.onDeclareStatement(d);
}

Statement* Parser::parseExpressionStatement() {
	Expression* e = parseExpr();
	match(tok_semicolon);
	return action.onExpressionStatement(e);
}

StatementList Parser::parseStatementSequence() {
	StatementList s;
	while (true) {
		Statement* t = parseStatement();
		s.push_back(t);
		if (lookahead() == tok_right_brace) {
			break;
		}
	}
	return s;
}

Declaration* Parser::parseDeclaration() {
	switch (lookahead()) {
	default:
		throw std::runtime_error("Expected declaration");
	case kw_def: {
		TokenName name = lookahead(2);
		std::cout << to_string(name) << '\n';
		if (name == tok_colon) {
			return parseObjectDefinition();
		}
		if (name == tok_left_paren) {
			return parseFunctionDefinition();
		}
		throw std::runtime_error("Improper declaration");
	}
	case kw_let:
	case kw_var:
		return parseObjectDefinition();
	}
	return nullptr;
}

Declaration* Parser::parseLocalDeclaration() {
	return parseObjectDefinition();
}

Declaration* Parser::parseObjectDefinition() {
	switch (lookahead()) {
	default:
		throw std::runtime_error("Expected object definition");
	case kw_def:
		return parseValueDefinition();
	case kw_let:
		return parseConstantDefinition();
	case kw_var:
		return parseVariableDefinition();
	}
}

Declaration* Parser::parseVariableDefinition() {
	assert(lookahead() == kw_var);
	accept();
	Token id = match(tok_identifier);
	match(tok_colon);
	Type* t = parseType();
	Declaration* d = action.onVariableDeclaration(id, t);
	match(tok_assignment_operator);
	Expression* e = parseExpr();
	match(tok_semicolon);
	return action.onVariableDefinition(d, e);
}

Declaration* Parser::parseConstantDefinition() {
	assert(lookahead() == kw_let);
	accept();
	Token id = match(tok_identifier);
	match(tok_colon);
	Type* t = parseType();
	Declaration* d = action.onConstantDeclaration(id, t);
	match(tok_assignment_operator);
	Expression* e = parseExpr();
	match(tok_semicolon);
	return action.onConstantDefinition(d, e);
}

Declaration* Parser::parseValueDefinition() {
	assert(lookahead() == kw_def);
	accept();
	Token id = match(tok_identifier);
	match(tok_colon);
	Type* t = parseType();
	Declaration* d = action.onValueDeclaration(id, t);
	match(tok_assignment_operator);
	Expression* e = parseExpr();
	match(tok_semicolon);
	return action.onValueDefinition(d, e);
}

Declaration* Parser::parseFunctionDefinition() {
	assert(lookahead() == kw_def);
	accept();
	Token id = match(tok_identifier);
	match(tok_left_paren);
	action.enterParameterScope();
	DeclarationList parms;
	if (lookahead() != tok_right_paren) {
		parms = parseParameterClause();
	}
	action.leaveScope();
	match(tok_right_paren);
	match(tok_arrow_operator);
	Type* t = parseType();
	Declaration* d = action.onFunctionDeclaration(id, parms, t);
	Statement* s = parseBlockStatement();
	return action.onFunctionDefinition(d, s);
}

DeclarationList Parser::parseParameterClause() {
	return parseParameterList();
}

DeclarationList Parser::parseParameterList() {
	DeclarationList params;
	while (true) {
		params.push_back(parseParameter());
		if (matchIf(tok_comma)) {
			continue;
		}
		else {
			break;
		}
	}
	return params;
}

Declaration* Parser::parseParameter() {
	Token id = match(tok_identifier);
	match(tok_colon);
	Type* t = parseType();
	return action.onParameterDeclaration(id, t);
}

DeclarationList Parser::parseDeclarationSequence() {
	DeclarationList dl;
	while (peek()) {
		Declaration* d = parseDeclaration();
		dl.push_back(d);
	}
	return dl;
}

Declaration* Parser::parseProgram() {
	action.enterGlobalScope();
	DeclarationList declarations = parseDeclarationSequence();
	action.leaveScope();
	return action.onProgram(declarations);
}
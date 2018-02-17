#include "stdafx.h"
#include "Token.h"
#include <cassert>
#include <iostream>
#include <iomanip>

const char* to_string(TokenName n) {
	switch (n) {
	case tok_eof:
		return "eof";
	case tok_left_brace:
		return "left-brace";
	case tok_right_brace:
		return "right-brace";
	case tok_left_paren:
		return "left-paren";
	case tok_right_paren:
		return "right-paren";
	case tok_left_bracket:
		return "left-bracket";
	case tok_right_bracket:
		return "right-bracket";
	case tok_comma:
		return "comma";
	case tok_semicolon:
		return "semicolon";
	case tok_colon:
		return "colon";
	case tok_relational_operator:
		return "relational-operator";
	case tok_arithmetic_operator:
		return "arithmetic-operator";
	case tok_bitwise_operator:
		return "bitwise-operator";
	case tok_logical_operator:
		return "logical-operator";
	case tok_conditional_operator:
		return "conditional-operator";
	case tok_assignment_operator:
		return "assignment-operator";
	case tok_compound_assignment_operator:
		return "compound-assignment-operator";
	case tok_def:
		return "def";
	case tok_else:
		return "else";
	case tok_if:
		return "if";
	case tok_let:
		return "let";
	case tok_var:
		return "var";
	case tok_identifier:
		return "identifier";
	case tok_binary_integer:
		return "binary-integer";
	case tok_decimal_integer:
		return "decimal-integer";
	case tok_hexadecimal_integer:
		return "hexadecimal-integer";
	case tok_boolean:
		return "boolean";
	case tok_floating_point:
		return "floating-point";
	case tok_character:
		return "character";
	case tok_string:
		return "string";
	case tok_type_specifier:
		return "type-specifier";
	}
}

const char* to_string(RelationalOperator r) {
	switch (r) {
	case op_equal:
		return "equal";
	case op_notEqual:
		return "not-equal";
	case op_lessThan:
		return "less-than";
	case op_greaterThan:
		return "greater-than";
	case op_lessEqual:
		return "less-equal";
	case op_greaterEqual:
		return "greater-equal";
	}
}

const char* to_string(ArithmeticOperator a) {
	switch (a) {
	case op_add:
		return "add";
	case op_sub:
		return "sub";
	case op_mul:
		return "mul";
	case op_div:
		return "div";
	case op_mod:
		return "mod";
	case op_inc:
		return "increment";
	case op_dec:
		return "decrement";
	}
}

const char* to_string(BitwiseOperator b) {
	switch (b) {
	case op_bitAnd:
		return "bitwise-and";
	case op_bitOr:
		return "bitwise-or";
	case op_bitXOR:
		return "bitwise-xor";
	case op_bitNot:
		return "bitwise-not";
	case op_shiftLeft:
		return "shift-left";
	case op_shiftRight:
		return "shift-right";
	}
}

const char* to_string(LogicalOperator l) {
	switch (l) {
	case op_logicAnd:
		return "logical-and";
	case op_logicOr:
		return "logical-or";
	case op_logicNot:
		return "logical-not";
	}
}

const char* to_string(CompoundAssignmentOperator c)
{
	switch (c) {
	case op_addEq:
		return "addition-assignment";
	case op_subEq:
		return "subtraction-assignment";
	case op_mulEq:
		return "multiplication-assignment";
	case op_divEq:
		return "division-assignment";
	case op_modEq:
		return "mod-assignment";
	}
}

const char* to_string(TypeSpecifier t) {
	switch (t) {
	case type_bool:
		return "bool";
	case type_int:
		return "int";
	case type_char:
		return "char";
	case type_float:
		return "float";
	}
}

Token::Token()
	: name(tok_eof) {}

Token::Token(TokenName n, TokenAttribute a, Location l)
	: name(n), attr(a), location(l) {}

static bool hasAttribute(TokenName n) {
	switch (n) {
	default:
		return false;
	case tok_relational_operator:
	case tok_arithmetic_operator:
	case tok_bitwise_operator:
	case tok_logical_operator:
	case tok_identifier:
	case tok_binary_integer:
	case tok_decimal_integer:
	case tok_hexadecimal_integer:
	case tok_boolean:
	case tok_floating_point:
	case tok_character:
	case tok_string:
	case tok_type_specifier:
		return true;
	}
}

Token::Token(TokenName n, Location l)
	: name(n), location(l) {
	assert(!hasAttribute(n));
}

Token::Token(Symbol s, Location l)
	: name(tok_identifier), attr(s), location(l) {}

Token::Token(RelationalOperator r, Location l)
	: name(tok_relational_operator), attr(r), location(l) {}

Token::Token(ArithmeticOperator a, Location l)
	: name(tok_arithmetic_operator), attr(a), location(l) {}

Token::Token(BitwiseOperator b, Location l)
	: name(tok_bitwise_operator), attr(b), location(l) {}

Token::Token(LogicalOperator lo, Location l)
	: name(tok_logical_operator), attr(lo), location(l) {}

Token::Token(CompoundAssignmentOperator c, Location l) 
	: name(tok_compound_assignment_operator), attr(c), location(l) {}

Token::Token(long long value, Location l)
	: Token(tok_decimal_integer, dec, value, l) {}

static TokenName getTokenName(Radix r) {
	switch (r) {
	case bin:
		return tok_binary_integer;
	case dec:
		return tok_decimal_integer;
	case hex:
		return tok_hexadecimal_integer;
	}
}

Token::Token(Radix r, long long value, Location l)
	: Token(getTokenName(r), r, value, l) {}

static bool checkRadix(TokenName n, Radix r) {
	switch (n) {
	case tok_binary_integer:
		return r == bin;
	case tok_decimal_integer:
		return r == dec;
	case tok_hexadecimal_integer:
		return r = hex;
	default:
		throw std::logic_error("Invalid token name.");
	}
}

Token::Token(TokenName n, Radix r, long long value, Location l)
	: name(n), attr(IntegerAttribute{ r, value }), location(l) {
	assert(checkRadix(n, r));
}

Token::Token(double d, Location l)
	: name(tok_floating_point), attr(d), location(l) {}

Token::Token(bool b, Location l)
	: name(tok_boolean), attr(b), location(l) {}

Token::Token(char c, Location l)
	: name(tok_character), attr(c), location(l) {}

Token::Token(StringAttribute s, Location l)
	: name(tok_string), attr(s), location(l) {}

Token::Token(TypeSpecifier s, Location l)
	: name(tok_type_specifier), attr(s), location(l) {}

static std::string escape(char c) {
	switch (c) {
	default:
		return { c };
	case '\'':
		return "\\\'";
	case '\"': 
		return "\\\"";
	case '\\': 
		return "\\\\";
	case '\a': 
		return "\\a";
	case '\b': 
		return "\\b";
	case '\f': 
		return "\\f";
	case '\n': 
		return "\\n";
	case '\r': 
		return "\\r";
	case '\t': 
		return "\\t";
	case '\v': 
		return "\\v";
	}
}

static std::string escape(const std::string& s) {
	std::string result;
	for (char c : s) {
		result += escape(c);
	}
	return result;
}

std::ostream& operator<<(std::ostream& os, Token t) {
	os << '<';
	os << t.getLocation() << ':';
	os << to_string(t.getName());
	switch (t.getName()) {
	default:
		break;
	case tok_relational_operator:
		os << ':' << to_string(t.getRelationalOperator());
		break;
	case tok_arithmetic_operator:
		os << ':' << to_string(t.getArithmeticOperator());
		break;
	case tok_bitwise_operator:
		os << ':' << to_string(t.getBitwiseOperator());
		break;
	case tok_logical_operator:
		os << ':' << to_string(t.getLogicalOperator());
		break;
	case tok_compound_assignment_operator:
		os << ':' << to_string(t.getCompoundAssignmentOperator());
		break;
	case tok_identifier:
		os << ':' << *t.getIdentifier();
		break;
	case tok_binary_integer:
	case tok_decimal_integer:
	case tok_hexadecimal_integer:
		os << ':' << std::setbase(t.getRadix()) << t.getInteger();
		break;
	case tok_boolean:
		os << ':' << t.getBool();
		break;
	case tok_floating_point:
		os << ':' << t.getFloatingPoint();
		break;
	case tok_character:
		os << ':' << escape(t.getChar());
		break;
	case tok_string:
		os << ':' << escape(t.getString());
		break;
	case tok_type_specifier:
		os << ':' << to_string(t.getTypeSpecifier());
		break;
	}
	os << '>';
	return os;
}

bool Token::isInteger() const {
	return tok_binary_integer <= name && name <= tok_hexadecimal_integer;
}

bool Token::isFloatingPoint() const {
	return name == tok_floating_point;
}

Symbol Token::getIdentifier() const {
	assert(name == tok_identifier);
	return attr.symbol;
}

RelationalOperator Token::getRelationalOperator() const {
	assert(name == tok_relational_operator);
	return attr.relOp;
}

ArithmeticOperator Token::getArithmeticOperator() const {
	assert(name == tok_arithmetic_operator);
	return attr.arithOp;
}

BitwiseOperator Token::getBitwiseOperator() const {
	assert(name == tok_bitwise_operator);
	return attr.bitOp;
}

LogicalOperator Token::getLogicalOperator() const {
	assert(name == tok_logical_operator);
	return attr.logOp;
}

CompoundAssignmentOperator Token::getCompoundAssignmentOperator() const {
	assert(name == tok_compound_assignment_operator);
	return attr.compOp;
}

long long Token::getInteger() const {
	assert(isInteger());
	return attr.intValue.value;
}

Radix Token::getRadix() const {
	assert(isInteger());
	return attr.intValue.radix;
}

double Token::getFloatingPoint() const {
	assert(name == tok_floating_point);
	return attr.floatValue;
}

bool Token::getBool() const {
	assert(name == tok_boolean);
	return attr.boolValue;
}

char Token::getChar() const {
	assert(name == tok_character);
	return attr.charValue;
}

const std::string& Token::getString() const {
	assert(name == tok_string);
	return *attr.strValue.symbol;
}

TypeSpecifier Token::getTypeSpecifier() const {
	assert(name == tok_type_specifier);
	return attr.typeSpec;
}
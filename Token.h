#pragma once
#include "Symbol.h"
#include "Location.h"
#include <cassert>

enum TokenName {
	tok_eof,
	//Punctuation
	tok_left_brace,
	tok_right_brace,
	tok_left_paren,
	tok_right_paren,
	tok_left_bracket,
	tok_right_bracket,
	tok_comma,
	tok_semicolon,
	tok_colon,
	//Ops
	tok_relational_operator,
	tok_arithmetic_operator,
	tok_logical_operator,
	tok_bitwise_operator,
	tok_conditional_operator,
	tok_assignment_operator,
	tok_compound_assignment_operator,
	//Keyword
	key_def,
	key_else,
	key_if,
	key_let,
	key_var,
	//Identifier
	tok_identifier,
	tok_binary_integer,
	tok_decimal_integer,
	tok_hexadecimal_integer,
	tok_boolean,
	tok_floating_point,
	tok_character,
	tok_string,
	tok_type_specifier
};

const char* to_string(TokenName n);

enum RelationalOperator {
	op_equal,
	op_notEqual,
	op_lessThan,
	op_greaterThan,
	op_lessEqual,
	op_greaterEqual
};

enum ArithmeticOperator { 
	op_add,
	op_sub,
	op_mul,
	op_div,
	op_mod,
	op_inc, //TODO: discriminate prefix/postfix
	op_dec //TODO: discriminate prefix/postfix
};

enum BitwiseOperator {
	op_bitAnd,
	op_bitOr,
	op_bitXOR,
	op_bitNot,
	op_shiftLeft,
	op_shiftRight
};

enum LogicalOperator {
	op_logicAnd,
	op_logicOr,
	op_logicNot
};

enum CompoundAssignmentOperator {
	op_addEq,
	op_subEq,
	op_mulEq,
	op_divEq,
	op_modEq
};

enum TypeSpecifier {
	type_char,
	type_bool,
	type_int,
	type_float
};

const char* to_string(RelationalOperator op);
const char* to_string(ArithmeticOperator op);
const char* to_string(BitwiseOperator op);
const char* to_string(LogicalOperator op);
const char* to_string(CompoundAssignmentOperator op);
const char* to_string(TypeSpecifier ts);

enum Radix {
	bin = 2,
	dec = 10,
	hex = 16
};

class IntegerAttribute {
public:
	Radix radix;
	long long value;
};

class StringAttribute {
public:
	Symbol symbol;
};

union TokenAttribute {
	TokenAttribute() = default;
	TokenAttribute(Symbol sym)
		: symbol(sym) {}
	TokenAttribute(RelationalOperator op)
		: relOp(op) {}
	TokenAttribute(ArithmeticOperator op)
		: arithOp(op) {}
	TokenAttribute(BitwiseOperator op)
		: bitOp(op) {}
	TokenAttribute(LogicalOperator op)
		: logOp(op) {}
	TokenAttribute(CompoundAssignmentOperator op)
		: compOp(op) {}
	TokenAttribute(IntegerAttribute i)
		: intValue(i) {}
	TokenAttribute(double d)
		: floatValue(d) {}
	TokenAttribute(bool b)
		: boolValue(b) {}
	TokenAttribute(char c)
		: charValue(c) {}
	TokenAttribute(StringAttribute s)
		: strValue(s) {}
	TokenAttribute(TypeSpecifier t)
		: typeSpec(t) {}

	Symbol symbol;
	RelationalOperator relOp;
	ArithmeticOperator arithOp;
	BitwiseOperator bitOp;
	LogicalOperator logOp;
	CompoundAssignmentOperator compOp;
	IntegerAttribute intValue;
	double floatValue;
	bool boolValue;
	char charValue;
	StringAttribute strValue;
	TypeSpecifier typeSpec;
};

class Token {
public:
	Token();
	Token(TokenName n, Location l = {});
	Token(TokenName n, TokenAttribute a, Location l = {});
	Token(Symbol s, Location l = {});
	Token(RelationalOperator op, Location l = {});
	Token(ArithmeticOperator op, Location l = {});
	Token(BitwiseOperator op, Location l = {});
	Token(LogicalOperator op, Location l = {});
	Token(CompoundAssignmentOperator op, Location l = {});
	Token(long long value, Location l = {});
	Token(Radix r, long long value, Location l = {});
	Token(TokenName n, Radix r, long long value, Location l = {});
	Token(double d, Location l = {});
	Token(bool b, Location l = {});
	Token(char c, Location l = {});
	Token(StringAttribute s, Location l = {});
	Token(TypeSpecifier t, Location l = {});

	operator bool() const { return name != tok_eof; }

	TokenName getName() const { return name; }
	TokenAttribute getAttribute() const { return attr; }
	Location getLocation() const { return location; }

	bool isInteger() const;
	bool isFloatingPoint() const;

	Symbol getIdentifier() const;
	RelationalOperator getRelationalOperator() const;
	ArithmeticOperator getArithmeticOperator() const;
	BitwiseOperator getBitwiseOperator() const;
	LogicalOperator getLogicalOperator() const;
	CompoundAssignmentOperator getCompoundAssignmentOperator() const;
	long long getInteger() const;
	double getFloatingPoint() const;
	Radix getRadix() const;
	bool getBool() const;
	char getChar() const;
	const std::string& getString() const;
	TypeSpecifier getTypeSpecifier() const;

private:
	TokenName name;
	TokenAttribute attr;
	Location location;
};


std::ostream& operator<<(std::ostream& os, Token t);
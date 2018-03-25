#pragma once
#include "Token.h"

//I started off with what I remembered from your Theory of Programming Languages class
//with expressions that went in our AST last semester. Then I realized that the specification
//in the actual assignment had a bunch of different types of expressions, so I just commented
//all this out. I'm not sure if I have the new expression correct, though.

//I see now that I skipped the "Types" section of the specification.. maybe doing that first will help.

//class NumericExpression;
//class BooleanExpression;
//
//enum NumericExpressionType {
//	integer,
//	argument,
//	arithmetic,
//	conditional
//};
//
//class NumericExpression {
//public:
//	NumericExpression(NumericExpressionType t)
//		: type(t) {}
//
//private:
//	NumericExpressionType type;
//
//};
//
//class IntegerExpression : NumericExpression {
//public:
//	IntegerExpression(int v)
//		: NumericExpression(integer), val(v) {}
//
//private:
//	int val;
//};
//
//class ArgumentExpression : NumericExpression {
//public:
//	ArgumentExpression(int a)
//		: NumericExpression(argument), args(a) {}
//
//private:
//	int args;
//};
//
//class ArithmeticExpression : NumericExpression {
//public:
//	ArithmeticExpression(ArithmeticOperator op, NumericExpression* left, NumericExpression* right)
//		: NumericExpression(arithmetic), op(op), left(left), right(right) {}
//
//private:
//	ArithmeticOperator op;
//	NumericExpression* left;
//	NumericExpression* right;
//};
//
//class ConditionalExpression : NumericExpression {
//public:
//	ConditionalExpression(BooleanExpression* test, NumericExpression* pass, NumericExpression* fail) 
//		: NumericExpression(conditional), test(test), pass(pass), fail(fail) {}
//	
//private:
//	BooleanExpression* test;
//	NumericExpression* pass;
//	NumericExpression* fail;
//};
//
//enum BooleanExpressionType {
//	boolean,
//	relational,
//	logical
//};
//
//class BooleanExpression {
//public:
//	BooleanExpression(BooleanExpressionType t)
//		: type(t) {}
//
//private:
//	BooleanExpressionType type;
//};
//
////this naming bothers me, but idk what else to name it
//class BoolExpression : BooleanExpression {
//public:
//	BoolExpression(bool v) 
//		: BooleanExpression(boolean), val(v) {}
//
//private:
//	bool val;
//};
//
//class RelationalExpression : BooleanExpression {
//public:
//	RelationalExpression(RelationalOperator op, NumericExpression* left, NumericExpression* right)
//		: BooleanExpression(relational), op(op), left(left), right(right) {}
//
//private:
//	RelationalOperator op;
//	NumericExpression* left;
//	NumericExpression* right;
//};

class PrimaryExpression {
public:
	PrimaryExpression(Token tok)
		: tok(tok) {}
	PrimaryExpression(PrimaryExpression* expr)
		: subExpr(expr) {}

private:
	Token tok;
	PrimaryExpression* subExpr;
};

//TODO: postfix expression
class PostfixExpression {};

enum UnaryOperator {
	plus,
	minus,
	tilde,
	exclamation,
	ampersand,
	asterisk
};

class UnaryExpression {
public:
	UnaryExpression(UnaryOperator op, UnaryExpression* operand)
		: op(op), operand(operand) {}
	UnaryExpression(PostfixExpression* operand)
		: postfixOperand(operand) {}
private:
	UnaryOperator op;
	UnaryExpression* operand;
	PostfixExpression* postfixOperand;
};

enum CastType {
	int_t,
	bool_t,
	//TODO: add more types
};

class CastExpression {
public:

private:
	CastType type;
	UnaryExpression* expr;
};
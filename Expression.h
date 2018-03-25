#pragma once
#include "Token.h"
#include "Type.h"

//I started off with what I remembered from your Theory of Programming Languages class
//with expressions that went in our AST last semester. Then I realized that the specification
//in the actual assignment had a bunch of different types of expressions, so I just commented
//all this out. I'm not sure if I have the new expression correct, though.

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

enum ExpressionType {
	primary,
	postfix,
	unary,
	cast,
	multiplicative,
	additive,
	shift,
	relational,
	equality,
	bitwiseAnd,
	bitwiseXor,
	bitwiseOr,
	logicalAnd,
	logicalOr,
	conditional,
	assignment
};

class Expression {
public:
	Expression(ExpressionType t)
		: type(t) {}

private:
	ExpressionType type;
};

class PrimaryExpression : Expression {
public:
	PrimaryExpression(Token tok)
		: Expression(primary), tok(tok) {}
	PrimaryExpression(Expression* expr)
		: Expression(primary), subExpr(expr) {}

private:
	Token tok;
	Expression* subExpr;
};

//TODO: figure out postfix expression
class PostfixExpression : Expression {};

enum UnaryOperator {
	plus,
	minus,
	tilde,
	exclamation,
	ampersand,
	asterisk
};

class UnaryExpression : Expression {
public:
	UnaryExpression(UnaryOperator op, UnaryExpression* operand)
		: Expression(unary), op(op), operand(operand) {}
	UnaryExpression(PostfixExpression* operand)
		: Expression(unary), postfixOperand(operand) {}
private:
	UnaryOperator op;
	UnaryExpression* operand;
	PostfixExpression* postfixOperand;
};

class CastExpression : Expression {
public:
	CastExpression(Type t)
		: Expression(cast), type(t) {}

private:
	Type type;
	UnaryExpression* expr;
};

enum MultiplicativeOperator {
	mul_op,
	div_op,
	mod_op
};

class MultiplicativeExpression : Expression {
public:
	MultiplicativeExpression(MultiplicativeOperator op, MultiplicativeExpression* left, CastExpression* right)
		: Expression(multiplicative), op(op), left(left), right(right) {}
	MultiplicativeExpression(CastExpression* c)
		: Expression(multiplicative), content(c) {}

private:
	MultiplicativeOperator op;
	MultiplicativeExpression* left;
	CastExpression* right;

	CastExpression* content;
};


#pragma once
#include "Token.h"

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
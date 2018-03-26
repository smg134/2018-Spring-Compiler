#pragma once
#include "Type.h"
//include declaration

class Expression {
public:
	Expression(Type* t)
		: type(t) {}

private:
	Type* type;
};

class IntegerExpression : Expression {
public:
	IntegerExpression(Type* t, int n)
		: Expression(t), val(n) {}

private:
	int val;
};

class BooleanExpression : Expression {
public:
	BooleanExpression(Type* t, bool b)
		: Expression(t), val(b) {}
private:
	bool val;
};

class ReferenceExpression : Expression {
public:
	ReferenceExpression(Type* t, Declaration* d)
		: Expression(t), reference(d) {}
private:
	Declaration* reference;
};
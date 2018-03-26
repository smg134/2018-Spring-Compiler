#pragma once
#include "Expression.h"
#include "Declaration.h"

class Statement {};

class ExpressionStatement : Statement {
public:
	ExpressionStatement(Expression* e)
		: expression(e) {}

private:
	Expression* expression;
};

class DeclarationStatement : Statement {
public:
	DeclarationStatement(Declaration* d)
		: entity(d) {}

private:
	Declaration* entity;
};
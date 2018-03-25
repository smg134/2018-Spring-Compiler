#pragma once
#include "Token.h"

enum NumericExpressionType {
	integer,
	argument,
	arithmetic,
	conditional
};

class NumericExpression {
public:
	NumericExpression(NumericExpressionType t)
		: type(t) {}

private:
	NumericExpressionType type;

};


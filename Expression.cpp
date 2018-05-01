#pragma once
#include "Expression.h"
#include "Type.h"

Type* Expression::getObjectType() const {
	return type->getObjectType();
}

bool Expression::hasType(const Type* t) const {
	return areSame(type, t);
}

bool Expression::isBool() const
{
	return type->isBool();
}

bool Expression::isInt() const
{
	return type->isInt();
}

bool Expression::isFloat() const
{
	return type->isFloat();
}

bool Expression::isFunction() const
{
	return type->isFunction();
}

bool Expression::isArithmetic() const
{
	return type->isArithmetic();
}

bool Expression::isNumeric() const
{
	return type->isNumeric();
}

bool Expression::isScalar() const
{
	return type->isScalar();
}


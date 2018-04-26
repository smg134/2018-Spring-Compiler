#pragma once
#include "stdafx.h"
#include "Declaration.h"
#include "Type.h"

#include <iostream>

bool ObjectDeclaration::isReference() const {
	return getType()->isReference();
}

FunctionType* FunctionDeclaration::getType() const {
	return static_cast<FunctionType*>(type);
}

Type* FunctionDeclaration::getReturnType() const {
	return getType()->getReturnType();
}
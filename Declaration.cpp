#pragma once
#include "stdafx.h"
#include "Declaration.h"
#include "Type.h"
#include "Debug.h"

#include <iostream>

void Declaration::debug() const {
	DebugPrinter d(std::cerr);
	::debug(d, this);
}


bool ObjectDeclaration::isReference() const {
	return getType()->isReference();
}

FunctionType* FunctionDeclaration::getType() const {
	return static_cast<FunctionType*>(type);
}

Type* FunctionDeclaration::getReturnType() const {
	return getType()->getReturnType();
}
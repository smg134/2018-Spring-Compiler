#pragma once
#include "Symbol.h"
#include "Type.h"
#include "Expression.h"
#include "Statement.h"
#include <vector>

class Declaration {};

class VariableDeclaration : Declaration {
public:
	VariableDeclaration(Symbol* s, Type* t)
		: name(s), type(t), init(nullptr) {}

private:
	Symbol* name;
	Type* type;
	Expression* init;
};

class ProgramDeclaration : Declaration, std::vector<Statement*> {};
#pragma once
#include "Symbol.h"
#include <vector>

class Type;
class FunctionType;
class Expression;
class Statement;

class Declaration {
public:
	enum Kind {
		program_kind,
		variable_kind,
		constant_kind,
		value_kind,
		parameter_kind,
		function_kind
	};

protected:
	Declaration(Kind k, Symbol s)
		: kind(k), name(s) {}

public:
	virtual ~Declaration() = default;

	Kind getKind() const { return kind; }

	bool isProgram() const { return kind == program_kind; }
	bool isVariable() const { return kind == variable_kind; }
	
	Symbol getName() const { return name; }

	void debug() const;

private:
	Kind kind;
	Symbol name;
};

using DeclarationList = std::vector<Declaration*>;

struct ProgramDeclaration : Declaration {
	ProgramDeclaration(const DeclarationList& d)
		: Declaration(program_kind, nullptr), declarations(d) {}

	const DeclarationList& getDeclarations() const { return declarations; }
	 DeclarationList& getDeclarations()  { return declarations; }

	DeclarationList declarations;
};

struct TypedDeclaration : Declaration {
protected:
	TypedDeclaration(Kind k, Symbol s, Type* t)
		: Declaration(k, s), type(t) {}

public:
	Type * getType() const { return type; }
	void setType(Type* t) { type = t; }

protected:
	Type* type;
};

struct ObjectDeclaration : TypedDeclaration {
protected:
	ObjectDeclaration(Kind k, Symbol s, Type* t, Expression* e)
		: TypedDeclaration(k, s, t), init(e) {}

public:
	bool isReference() const;

	Expression* getInit() const { return init; }

	void setInit(Expression* e) { init = e; }

protected:
	Expression* init;
};

struct VariableDeclaration : ObjectDeclaration {
	VariableDeclaration(Symbol s, Type* t, Expression* e = nullptr)
		: ObjectDeclaration(variable_kind, s, t, e) {}
};

struct ConstantDeclaration : ObjectDeclaration {
	ConstantDeclaration(Symbol s, Type* t, Expression* e = nullptr)
		: ObjectDeclaration(constant_kind, s, t, e) {}
};

struct ValueDeclaration : ObjectDeclaration {
	ValueDeclaration(Symbol s, Type* t, Expression* e = nullptr)
		: ObjectDeclaration(value_kind, s, t, e) {}
};

struct ParameterDeclaration : ObjectDeclaration {
	ParameterDeclaration(Symbol s, Type* t)
		: ObjectDeclaration(parameter_kind, s, t, nullptr) {}
};

struct FunctionDeclaration : TypedDeclaration {
	FunctionDeclaration(Symbol s, Type* t, const DeclarationList& params, Statement* stmt = nullptr)
		: TypedDeclaration(function_kind, s, t), params(params), body(stmt) {}

	const DeclarationList& getParameters() const { return params; }
	DeclarationList& getParameters() { return params; }

	FunctionType* getType() const;
	
	Type* getReturnType() const;

	Statement* getBody() const { return body; }
	void setBody(Statement* s) { body = s; }

	DeclarationList params;
	Statement* body;
};
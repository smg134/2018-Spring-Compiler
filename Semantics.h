#pragma once
#include "Token.h"

class Type;
class Expression;
class Statement;
class Declaration;
class FunctionDeclaration;

using TypeList = std::vector<Type*>;
using ExpressionList = std::vector<Expression*>;
using StatementList = std::vector<Statement*>;
using DeclarationList = std::vector<Declaration*>;

class Scope;

class Semantics {
public:
	Semantics();
	~Semantics();

	Type* onBasicType(Token t);

	// Expression
	Expression* onAssignmentExpression(Expression* e1, Expression* e2);
	Expression* onConditationalExpression(Expression* e1, Expression* e2, Expression* e3);
	Expression* onLogicalOrExpression(Expression* e1, Expression* e2);
	Expression* onLogicalAndExpression(Expression* e1, Expression* e2);
	Expression* onBitwiseOrExpression(Expression* e1, Expression* e2);
	Expression* onBitwiseXOrExpression(Expression* e1, Expression* e2);
	Expression* onBitwiseAndExpression(Expression* e1, Expression* e2);
	Expression* onEqualityExpression(Token t, Expression* e1, Expression* e2);
	Expression* onRelationalExpression(Token t, Expression* e1, Expression* e2);
	Expression* onShiftExpression(Token t, Expression* e1, Expression* e2);
	Expression* onAdditiveExpression(Token t, Expression* e1, Expression* e2);
	Expression* onMultiplicativeExpression(Token t, Expression* e1, Expression* e2);
	Expression* onCastExpression(Expression* e, Type* t);
	Expression* onUnaryExpression(Token t, Expression* e);
	Expression* onCallExpression(Expression* e, const ExpressionList& args);
	Expression* onIndexExpression(Expression* e, const ExpressionList& args);
	Expression* onIdExpression(Token t);
	Expression* onIntegerLiteral(Token t);
	Expression* onBooleanLiteral(Token t);
	Expression* onFloatLiteral(Token t);

	// Statement
	Statement* onBlockStatement(const StatementList& s);
	void startBlock();
	void finishBlock();
	Statement* onIfStatement(Expression* e, Statement* s1, Statement* s2);
	Statement* onWhileStatement(Expression* e, Statement* s);
	Statement* onBreakStatement();
	Statement* onContinueStatement();
	Statement* onReturnStatement(Expression* e);
	Statement* onDeclareStatement(Declaration* d);
	Statement* onExpressionStatement(Expression* e);

	// Declaration
	Declaration* onVariableDeclaration(Token t, Type* y);
	Declaration* onVariableDefinition(Declaration* d, Expression* e);
	Declaration* onConstantDeclaration(Token t, Type* y);
	Declaration* onConstantDefinition(Declaration* d, Expression* e);
	Declaration* onValueDeclaration(Token t, Type* y);
	Declaration* onValueDefinition(Declaration* d, Expression* e);
	Declaration* onParameterDeclaration(Token t, Type* y);
	Declaration* onFunctionDeclaration(Token t, const DeclarationList& d, Type* y);
	Declaration* onFunctionDefinition(Declaration* d, Statement* s);

	Declaration* onProgram(const DeclarationList& d);

	// Scope
	void enterGlobalScope();
	void enterParameterScope();
	void enterBlockScope();
	void leaveScope();
	
	Scope* getCurrentScope() const { return scope; }

	// Context
	FunctionDeclaration* getCurrentFunction() const { return function; }

	// Declaration
	void declare(Declaration* d);

	// Name lookup
	Declaration* lookup(Symbol s);

	// Type checking
	Expression* requireReference(Expression* e);
	Expression* requireValue(Expression* e);
	Expression* requireInteger(Expression* e);
	Expression* requireBoolean(Expression* e);
	Expression* requireFunction(Expression* e);
	Expression* requireArithmetic(Expression* e);
	Expression* requireNumeric(Expression* e);
	Expression* requireScalar(Expression* e);

	Type* requireSame(Type* t1, Type* t2);
	Type* commonType(Type* t1, Type* t2);

	// Conversion
	Expression* convertToValue(Expression* e);
	Expression* convertToBool(Expression* e);
	Expression* convertToChar(Expression* e);
	Expression* convertToInt(Expression* e);
	Expression* convertToFloat(Expression* e);
	Expression* convertToType(Expression* e, Type* t);

private:
	Scope* scope;

	FunctionDeclaration* function;

	Type* _bool;
	Type* _char;
	Type* _int;
	Type* _float;
};
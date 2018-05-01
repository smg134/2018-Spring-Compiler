#pragma once
#include "Semantics.h"
#include "Type.h"
#include "Expression.h"
#include "Statement.h"
#include "Declaration.h"
#include "Scope.h"

#include <sstream>

Semantics::Semantics()
	: scope(nullptr),
	function(nullptr),
	_bool(new BoolType()),
	_char(new CharType()),
	_int(new IntType()),
	_float(new FloatType()) {}

Semantics::~Semantics() {
	assert(!scope);
	assert(!function);
}

Type* Semantics::onBasicType(Token t) {
	switch (t.getTypeSpecifier()) {
	case type_bool:
		return _bool;
	case type_char:
		return _char;
	case type_int:
		return _int;
	case type_float:
		return _float;
	}
}

Expression* Semantics::onAssignmentExpression(Expression* e1, Expression* e2) {
	e1 = requireReference(e1);
	e2 = requireValue(e2);

	Type* t1 = e1->getObjectType();
	Type* t2 = e2->getType();
	requireSame(t1, t2);

	return new AssignmentExpression(e1->getType(), e1, e2);
}

Expression* Semantics::onConditationalExpression(Expression* e1, Expression* e2, Expression* e3) {
	e1 = requireBoolean(e1);

	Type* t = commonType(e1->getType(), e2->getType());
	e2 = convertToType(e2, t);
	e3 = convertToType(e3, t);

	return new ConditionalExpression(t, e1, e2, e3);
}

Expression* Semantics::onLogicalOrExpression(Expression* e1, Expression* e2) {
	e1 = requireBoolean(e1);
	e2 = requireBoolean(e2);
	return new BinopExpression(_bool, bo_lor, e1, e2);
}

Expression* Semantics::onLogicalAndExpression(Expression* e1, Expression* e2) {
	e1 = requireBoolean(e1);
	e2 = requireBoolean(e2);
	return new BinopExpression(_bool, bo_land, e1, e2);
}

Expression* Semantics::onBitwiseOrExpression(Expression* e1, Expression* e2) {
	e1 = requireInteger(e1);
	e2 = requireInteger(e2);
	return new BinopExpression(_int, bo_ior, e1, e2);
}

Expression* Semantics::onBitwiseXOrExpression(Expression* e1, Expression* e2) {
	e1 = requireInteger(e1);
	e2 = requireInteger(e2);
	return new BinopExpression(_int, bo_xor, e1, e2);
}

Expression* Semantics::onBitwiseAndExpression(Expression* e1, Expression* e2) {
	e1 = requireInteger(e1);
	e2 = requireInteger(e2);
	return new BinopExpression(_int, bo_and, e1, e2);
}

static binop getRelationalOperator(RelationalOperator r) {
	switch (r) {
	case op_equal: return bo_eq;
	case op_notEqual: return bo_ne;
	case op_lessThan: return bo_lt;
	case op_greaterThan: return bo_gt;
	case op_lessEqual: return bo_le;
	case op_greaterEqual: return bo_ge;
	}
}

Expression* Semantics::onEqualityExpression(Token t, Expression* e1, Expression* e2) {
	e1 = requireScalar(e1);
	e2 = requireScalar(e2);
	RelationalOperator r = t.getRelationalOperator();
	return new BinopExpression(_bool, getRelationalOperator(r), e1, e2);
}

Expression* Semantics::onRelationalExpression(Token t, Expression* e1, Expression* e2) {
	e1 = requireNumeric(e1);
	e2 = requireNumeric(e2);
	RelationalOperator r = t.getRelationalOperator();
	return new BinopExpression(_bool, getRelationalOperator(r), e1, e2);
}

static binop getBitwiseOperator(BitwiseOperator b) {
	switch (b) {
	case op_bitAnd: return bo_and;
	case op_bitOr: return bo_ior;
	case op_bitXOR: return bo_xor;
	case op_shiftLeft: return bo_shl;
	case op_shiftRight: return bo_shr;
	default:
		throw std::logic_error("Invalid operator");
	}
}

Expression* Semantics::onShiftExpression(Token t, Expression* e1, Expression* e2) {
	e1 = requireInteger(e1);
	e2 = requireInteger(e2);
	BitwiseOperator b = t.getBitwiseOperator();
	return new BinopExpression(_int, getBitwiseOperator(b), e1, e2);
}

static binop getArithmeticOperator(ArithmeticOperator a) {
	switch (a) {
	case op_add: return bo_add;
	case op_sub: return bo_sub;
	case op_mul: return bo_mul;
	case op_div: return bo_quo;
	case op_mod: return bo_rem;

	}
}

Expression* Semantics::onAdditiveExpression(Token t, Expression* e1, Expression* e2) {
	e1 = requireArithmetic(e1);
	e2 = requireArithmetic(e2);
	Type* y = requireSame(e1->getType(), e2->getType());

	ArithmeticOperator a = t.getArithmeticOperator();
	return new BinopExpression(y, getArithmeticOperator(a), e1, e2);
}

Expression* Semantics::onMultiplicativeExpression(Token t, Expression* e1, Expression* e2) {
	e1 = requireArithmetic(e1);
	e2 = requireArithmetic(e2);
	Type* y = requireSame(e1->getType(), e2->getType());

	ArithmeticOperator a = t.getArithmeticOperator();
	return new BinopExpression(y, getArithmeticOperator(a), e1, e2);
}

Expression* Semantics::onCastExpression(Expression* e, Type* t) {
	return new CastExpression(convertToType(e, t), t);
}

static unop getUnaryOperator(Token t) {
	switch (t.getName()) {
	case tok_arithmetic_operator:
		if (t.getArithmeticOperator() == op_add)
			return uo_pos;
		else if (t.getArithmeticOperator() == op_sub)
			return uo_neg;
		else
			throw std::logic_error("invalid operator");
	case tok_bitwise_operator:
		if (t.getBitwiseOperator() == op_bitNot)
			return uo_cmp;
		else
			throw std::logic_error("invalid operator");
	case tok_logical_operator:
		if (t.getLogicalOperator())
			return uo_not;
		else
			throw std::logic_error("invalid operator");
	default:
		throw std::logic_error("invalid token");

	}
}

Expression* Semantics::onUnaryExpression(Token t, Expression* e) {
	unop u = getUnaryOperator(t);
	Type* y;
	switch (u) {
	case uo_pos:
	case uo_neg:
		e = requireArithmetic(e);
		y = e->getType();
		break;
	case uo_cmp:
		e = requireInteger(e);
		y = _int;
		break;
	case uo_not:
		e = requireBoolean(e);
		y = _bool;
		break;
	case uo_addr:
	case uo_deref:
		//todo
		break;
	}
	return new UnopExpression(u, e);
}

Expression* Semantics::onCallExpression(Expression* e, const ExpressionList& args) {
	e = requireFunction(e);
	FunctionType* t = static_cast<FunctionType*>(e->getType());

	TypeList& parameters = t->getParamTypes();
	if (parameters.size() < args.size()) {
		throw std::runtime_error("Too many arguments");
	}
	if (args.size() < parameters.size()) {
		throw std::runtime_error("Too few arguments");
	}

	for (size_t i = 0; i != parameters.size(); i++) {
		Type* p = parameters[i];
		Expression* a = args[i];
		if (!a->hasType(p)) {
			throw std::runtime_error("Argument does not match type");
		}
	}

	return new CallExpression(t->getReturnType(), e, args);
}

Expression* Semantics::onIndexExpression(Expression* e, const ExpressionList& args) {
	//todo
	return {};
}

Expression* Semantics::onIdExpression(Token t) {
	Symbol s = t.getIdentifier();

	Declaration* d = lookup(s);
	if (!d) {
		std::stringstream ss;
		ss << "No matching declaration for '" << *s << "'";
		throw std::runtime_error(ss.str());
	}

	Type* y;
	TypedDeclaration* td = dynamic_cast<TypedDeclaration*>(d);
	if (td->isVariable()) {
		y = new ReferenceType(td->getType());
	}
	else {
		y = td->getType();
	}

	return new IdExpression(y, d);
}

Expression* Semantics::onIntegerLiteral(Token t) {
	int val = t.getInteger();
	return new IntExpression(_int, val);
}

Expression* Semantics::onBooleanLiteral(Token t) {
	int val = t.getBool();
	return new BoolExpression(_bool, val);
}

Expression* Semantics::onFloatLiteral(Token t) {
	int val = t.getFloatingPoint();
	return new FloatExpression(_float, val);
}

Statement* Semantics::onBlockStatement(const StatementList& s) {
	return new BlockStatement(s);
}

void Semantics::startBlock() {
	Scope* parent = getCurrentScope()->parent;
	if (dynamic_cast<GlobalScope*>(parent)) {
		FunctionDeclaration* function = getCurrentFunction();
		for (Declaration* param : function->getParameters()) {
			declare(param);
		}
	}
}

void Semantics::finishBlock() {
	//todo
}

Statement* Semantics::onIfStatement(Expression* e, Statement* s1, Statement* s2) {
	return new IfStatement(e, s1, s2);
}

Statement* Semantics::onWhileStatement(Expression* e, Statement* s) {
	return new WhileStatement(e, s);
}

Statement* Semantics::onBreakStatement() {
	return new BreakStatement();
}

Statement* Semantics::onContinueStatement() {
	return new ContinueStatement();
}

Statement* Semantics::onReturnStatement(Expression* e) {
	return new ReturnStatement(e);
}

Statement* Semantics::onDeclareStatement(Declaration* d) {
	return new DeclareStatement(d);
}

Statement* Semantics::onExpressionStatement(Expression* e) {
	return new ExpressionStatement(e);
}

void Semantics::declare(Declaration* d) {
	Scope* s = getCurrentScope();
	if (s->lookup(d->getName())) {
		std::stringstream ss;
		ss << "Redeclaration of '" << d->getName() << "' unallowed.";
		throw std::runtime_error(ss.str());
	}
	s->declare(d->getName(), d);
}

Declaration* Semantics::onVariableDeclaration(Token t, Type* y) {
	Declaration* var = new VariableDeclaration(t.getIdentifier(), y);
	declare(var);
	return var;
}

Declaration* Semantics::onVariableDefinition(Declaration* d, Expression* e) {
	VariableDeclaration* var = static_cast<VariableDeclaration*>(d);
	var->setInit(e);
	return var;
}

Declaration* Semantics::onConstantDeclaration(Token t, Type* y) {
	Declaration* var = new ConstantDeclaration(t.getIdentifier(), y);
	declare(var);
	return var;
}

Declaration* Semantics::onConstantDefinition(Declaration* d, Expression* e) {
	ConstantDeclaration* var = static_cast<ConstantDeclaration*>(d);
	var->setInit(e);
	return var;
}

Declaration* Semantics::onValueDeclaration(Token t, Type* y) {
	Declaration* var = new ValueDeclaration(t.getIdentifier(), y);
	declare(var);
	return var;
}

Declaration* Semantics::onValueDefinition(Declaration* d, Expression* e) {
	ValueDeclaration* var = static_cast<ValueDeclaration*>(d);
	var->setInit(e);
	return var;
}

Declaration* Semantics::onParameterDeclaration(Token t, Type* y) {
	Declaration* param = new ParameterDeclaration(t.getIdentifier(), y);
	declare(param);
	return param;
}

static TypeList getParameterTypes(const DeclarationList& d) {
	TypeList types;
	for (const Declaration* decl : d) {
		types.push_back(static_cast<const ParameterDeclaration*>(decl)->getType());
	}
	return types;
}

Declaration* Semantics::onFunctionDeclaration(Token t, const DeclarationList& params, Type* y) {
	FunctionType* ft = new FunctionType(getParameterTypes(params), y);
	FunctionDeclaration* fd = new FunctionDeclaration(t.getIdentifier(), ft, params);
	fd->setType(ft);
	declare(fd);

	assert(!function);
	function = fd;

	return fd;
}

Declaration* Semantics::onFunctionDefinition(Declaration* d, Statement* s) {
	FunctionDeclaration* f = static_cast<FunctionDeclaration*>(d);
	f->setBody(s);

	assert(function == f);
	function = nullptr;

	return f;
}

Declaration* Semantics::onProgram(const DeclarationList& d) {
	return new ProgramDeclaration(d);
}

void Semantics::enterGlobalScope() {
	assert(!scope);
	scope = new GlobalScope();
}

void Semantics::enterParameterScope() {
	scope = new ParamenterScope(scope);
}

void Semantics::enterBlockScope() {
	scope = new BlockScope(scope);
}

void Semantics::leaveScope() {
	Scope* s = scope;
	scope = s->parent;
	delete s;
}

Declaration* Semantics::lookup(Symbol s) {
	Scope* sc = getCurrentScope();
	while (sc) {
		if (Declaration* d = sc->lookup(s)) {
			return d;
		}
		sc = sc->parent;
	}
	return nullptr;
}

Expression* Semantics::requireReference(Expression* e) {
	Type* t = e->getType();
	if (!t->isReference()) {
		throw std::runtime_error("Expected a reference type");
	}
	return e;
}

Expression* Semantics::requireValue(Expression* e) {
	return convertToValue(e);
}

Expression* Semantics::requireArithmetic(Expression* e) {
	e = requireValue(e);
	if (!e->isArithmetic()) {
		throw std::runtime_error("Expected an arithmetic expression");
	}
	return e;
}

Expression* Semantics::requireNumeric(Expression* e) {
	e = requireValue(e);
	if (!e->isNumeric()) {
		throw std::runtime_error("Expected a numeric expression");
	}
	return e;
}

Expression* Semantics::requireScalar(Expression* e) {
	e = requireValue(e);
	if (!e->isScalar()) {
		throw std::runtime_error("Expected a scalar expression");
	}
	return e;
}

Expression* Semantics::requireInteger(Expression* e)
{
	e = requireValue(e);
	if (!e->isInt())
		throw std::runtime_error("Expected an integer expression");
	return e;
}

Expression* Semantics::requireBoolean(Expression* e)
{
	e = requireValue(e);
	if (!e->isBool())
		throw std::runtime_error("Expected a boolean expression");
	return e;
}

Expression* Semantics::requireFunction(Expression* e)
{
	e = requireValue(e);
	if (!e->isFunction())
		throw std::runtime_error("Expected a function");
	return e;
}

Type* Semantics::requireSame(Type* t1, Type* t2)
{
	if (!areSame(t1, t2))
		throw std::runtime_error("Type mismatch");
	return t1;
}


Type* Semantics::commonType(Type* t1, Type* t2)
{
	if (areSame(t1, t2))
		return t1;
	if (t1->isReferenceTo(t2))
		return t2;
	if (t2->isReferenceTo(t1))
		return t1;
	throw std::runtime_error("No common type");
}

Expression* Semantics::convertToValue(Expression* e) {
	Type* t = e->getType();
	if (t->isReference()) {
		return new ConversionExpression(e, conv_value, t->getObjectType());
	}
	return e;
}

Expression* Semantics::convertToBool(Expression* e) {
	e = convertToValue(e);
	Type* t = e->getType();
	switch (t->getKind()) {
	case Type::bool_kind:
		return e;
	case Type::char_kind:
	case Type::int_kind:
	case Type::float_kind:
	case Type::pointer_kind:
	case Type::function_kind:
		return new ConversionExpression(e, conv_bool, _bool);
	default:
		throw std::runtime_error("Cannot convert to bool");
	}
}

Expression* Semantics::convertToChar(Expression* e) {
	e = convertToValue(e);
	Type* t = e->getType();
	switch (t->getKind()) {
	case Type::char_kind:
		return e;
	case Type::int_kind:
		return new ConversionExpression(e, conv_char, _char);
	default:
		throw std::runtime_error("Cannot convert to char");
	}
}

Expression* Semantics::convertToInt(Expression* e) {
	e = convertToValue(e);
	Type* t = e->getType();
	switch (t->getKind()) {
	case Type::int_kind:
		return e;
	case Type::bool_kind:
	case Type::char_kind:
		return new ConversionExpression(e, conv_int, _int);
	case Type::float_kind:
		return new ConversionExpression(e, conv_trunc, _int);
	default:
		throw std::runtime_error("Cannot convert to int");
	}
}

Expression* Semantics::convertToFloat(Expression* e) {
	e = convertToValue(e);
	Type* t = e->getType();
	switch (t->getKind()) {
	case Type::int_kind:
		return new ConversionExpression(e, conv_ext, _float);
	case Type::float_kind:
		return e;
	default:
		throw std::runtime_error("Cannot convert to float");
	}
}

Expression* Semantics::convertToType(Expression* e, Type* t) {
	if (t->isObject()) {
		e = convertToValue(e);
	}

	if (e->hasType(t)) {
		return e;
	}

	switch (t->getKind()) {
	case Type::bool_kind:
		return convertToBool(e);
	case Type::char_kind:
		return convertToChar(e);
	case Type::int_kind:
		return convertToInt(e);
	case Type::float_kind:
		return convertToFloat(e);
	default:
		throw std::runtime_error("Cannot convert");
	}
}
#pragma once
#include "Token.h"
#include <vector>

class Type;
class Declaration;

class Expression {
public:
	enum Kind {
		bool_kind,
		int_kind,
		float_kind,
		id_kind,
		unop_kind,
		binop_kind,
		ptr_kind,
		call_kind,
		index_kind,
		cast_kind,
		assign_kind,
		cond_kind,
		conv_kind
	};

protected:
	Expression(Kind k)
		: kind(k), type() {}

	Expression(Kind k, Type* t)
		: kind(k), type(t) {}

public:
	virtual ~Expression() = default;

	Kind getKind() const { return kind; }
	Type* getType() const { return type; }
	Type* getObjectType() const;

	bool hasType(const Type* t) const;

	bool isBool() const;
	bool isInt() const;
	bool isFloat() const;
	bool isFunction() const;
	bool isArithmetic() const;
	bool isNumeric() const;
	bool isScalar() const;

private:
	Kind kind;
	Type* type;
};

using ExpressionList = std::vector<Expression*>;

struct BoolExpression : Expression {
	BoolExpression(Type* t, bool b)
		: Expression(bool_kind, t), val(b) {}

	bool getValue() const { return val; }

	bool val;
};

struct IntExpression : Expression {
	IntExpression(Type* t, int i)
		: Expression(int_kind, t), val(i) {}

	int getValue() const { return val; }

	int val;
};

struct FloatExpression : Expression {
	FloatExpression(Type* t, double d)
		: Expression(float_kind, t), val(d) {}

	double getValue() const { return val; }

	double val;
};

struct IdExpression : Expression {
	IdExpression(Type* t, Declaration* d)
		: Expression(id_kind, t), ref(d) {}

	Declaration* getDeclaration() const { return ref; }

	Declaration* ref;
};

enum unop {
	uo_pos,
	uo_neg,
	uo_cmp,
	uo_not,
	uo_addr,
	uo_deref
};

struct UnopExpression : Expression {
	UnopExpression(unop op, Expression* e)
		: Expression(unop_kind), op(op), arg(e) {}

	unop getOperator() const { return op; }
	Expression* getOperand() const { return arg; }

	unop op;
	Expression* arg;
};

enum binop {
	bo_add,
	bo_sub,
	bo_mul,
	bo_quo,
	bo_rem,
	bo_and,
	bo_ior,
	bo_xor,
	bo_shl,
	bo_shr,
	bo_land,
	bo_lor,
	bo_eq,
	bo_ne,
	bo_lt,
	bo_gt,
	bo_le,
	bo_ge
};

struct BinopExpression : Expression {
	BinopExpression(Type* t, binop op, Expression* e1, Expression* e2)
		: Expression(binop_kind, t), op(op), lhs(e1), rhs(e2) {}

	binop getOperator() const { return op; }

	Expression* getLHS() const { return lhs; }
	Expression* getRHS() const { return rhs; }

	binop op;
	Expression* lhs;
	Expression* rhs;
};

struct PostfixExpression : Expression {
	PostfixExpression(Kind k, Type* t, Expression* e, const ExpressionList& args)
		: Expression(k), base(e), args(args) {}

	const ExpressionList& getArguments() const { return args; }
	ExpressionList& getArguments() { return args; }

	Expression* base;
	ExpressionList args;
};

struct CallExpression : PostfixExpression {
	CallExpression(Type* t, Expression* e, const ExpressionList& args)
		: PostfixExpression(call_kind, t, e, args) {}

	Expression* getCallee() const { return base; }
};

struct IndexExpression : PostfixExpression {
	IndexExpression(Type* t, Expression* e, const ExpressionList& args)
		: PostfixExpression(index_kind, t, e, args) {}
};

struct CastExpression : Expression {
	CastExpression(Expression* e, Type* t)
		: Expression(cast_kind, t), source(e), destination(t) {}

	Expression* source;
	Type* destination;
};

struct AssignExpression : Expression {
	AssignExpression(Type* t, Expression* e1, Expression* e2)
		: Expression(assign_kind), lhs(e1), rhs(e2) {}

	Expression* getLHS() const { return lhs; }
	Expression* getRHS() const { return rhs; }

	Expression* lhs;
	Expression* rhs;
};

struct ConditionalExpression : Expression {
	ConditionalExpression(Type* t, Expression* e1, Expression* e2, Expression* e3)
		: Expression(cond_kind, t), condition(e1), pass(e2), fail(e3) {}

	Expression* getCondition() const { return condition; }
	Expression* getPassValue() const { return pass; }
	Expression* getFailValue() const { return fail; }

	Expression* condition;
	Expression* pass;
	Expression* fail;
};

enum Conversion {
	conv_identity,
	conv_value,
	conv_bool, // T to bool
	conv_char, // int to char
	conv_int, // T to int
	cont_ext, // int to float
	conv_trunc // float to int
};

struct ConversionExpression : Expression {
	ConversionExpression(Expression* e1, Conversion c, Type* t)
		: Expression(conv_kind, t), source(e1), conversion(c) {}

	Conversion getConversion() const { return conversion; }
	Expression* getSource() const { return source; }

	Expression* source;
	Conversion conversion;
};
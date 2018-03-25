#pragma once

enum TypeType {
	basic,
	postfix,
	reference,
	type
};

class Type {
public:
	Type(TypeType t)
		: type(t) {}

private:
	TypeType type;
};

class VoidType : Type {
public:
	VoidType()
		: Type(basic) {}
};

class BoolType : Type {
public:
	BoolType()
		: Type(basic) {}
};

class IntType : Type {
public:
	IntType()
		: Type(basic) {}
};

class FloatType : Type {
public:
	FloatType()
		: Type(basic) {}
};

class CharType : Type {
public:
	CharType()
		: Type(basic) {}
};

enum PostfixTypeOperator {
	op_asterisk,
	op_const,
	op_volatile,
	op_bracketExpr,
	op_bracket
};

class PostfixType : Type {
public:
	PostfixType(PostfixTypeOperator o, Type c)
		: Type(postfix), op(o), content(c) {}

private:
	PostfixTypeOperator op;
	Type content;
	//TODO: add expression member for PostfixTypeOperator::op_bracketExpr operations
};

class ReferenceType : Type {
public:
	ReferenceType(Type o)
		: Type(reference), obj(o) {}

private:
	Type obj;
};
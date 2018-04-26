#pragma once
#include <vector>

class Type {
public:
	enum Kind {
		bool_kind,
		char_kind,
		int_kind,
		float_kind,
		pointer_kind,
		reference_kind,
		function_kind
	};

protected:
	Type(Kind k)
		: kind(k) {}

public:
	virtual ~Type() = default;

	Kind getKind() const { return kind; }

	bool isInt() const { return kind == int_kind; }
	bool isBool() const { return kind == bool_kind; }
	bool isFloat() const { return kind == float_kind; }
	bool isChar() const { return kind == char_kind; }
	bool isReference() const { return kind == reference_kind; }
	bool isReferenceTo(const Type* t);
	bool isPointer() const { return kind == pointer_kind; }
	bool isPointerTo(const Type* t);
	bool isFunction() const { return kind == function_kind; }
	bool isObject() const { return !isReference(); }
	bool isArithmetic() const;
	bool isNumeric() const;
	bool isScalar() const { return true; }
	
	Type* getObjectType() const;

private:
	Kind kind;
};

using TypeList = std::vector<Type*>;

struct BoolType : Type {
	BoolType()
		: Type(bool_kind) {}
};

struct CharType : Type {
	CharType()
		: Type(char_kind) {}
};

struct IntType : Type {
	IntType()
		: Type(int_kind) {}
};

struct FloatType : Type {
	FloatType()
		: Type(float_kind) {}
};

struct PointerType : Type {
	PointerType()
		: Type(pointer_kind) {}

	Type* getElementType() const { return element; }

	Type* element;
};

struct ReferenceType : Type {
	ReferenceType()
		: Type(reference_kind) {}

	Type* getObjectType() const { return element; }

	Type* element;
};

struct FunctionType : Type {
	FunctionType(const TypeList& p, Type* r)
		: Type(function_kind), params(p), returnType(r) {}

	const TypeList& getParamTypes() const { return params; }
	TypeList& getParamTypes() { return params; }
	Type* getReturnType() const { return returnType; }

	TypeList params;
	Type* returnType;
};

bool areSame(const Type* t1, const Type* t2);
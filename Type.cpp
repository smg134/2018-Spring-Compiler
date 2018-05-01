#include "stdafx.h"
#include "Type.h"

bool Type::isReferenceTo(const Type* t) {
	if (const ReferenceType* r = dynamic_cast<const ReferenceType*>(this)) {
		return areSame(r->getObjectType(), t);
	}
	return false;
}

bool Type::isPointerTo(const Type* t) {
	if (const PointerType* p = dynamic_cast<const PointerType*>(this)) {
		return areSame(p->getElementType(), t);
	}
	return false;
}

bool Type::isArithmetic() const {
	return kind == int_kind || kind == float_kind;
}

bool Type::isNumeric() const {
	switch (kind) {
	case bool_kind:
	case char_kind:
	case int_kind:
	case float_kind:
		return true;
	default:
		return false;
	}
}

Type* Type::getObjectType() const {
	if (const ReferenceType* r = dynamic_cast<const ReferenceType*>(this)) {
		return r->getObjectType();
	}
	return const_cast<Type*>(this);
}

bool areSamePointers(const PointerType* t1, const PointerType* t2) {
	return areSame(t1->getElementType(), t2->getElementType());
}

bool areSameReferences(const ReferenceType* t1, const ReferenceType* t2) {
	return areSame(t1->getObjectType(), t2->getObjectType());
}

bool areSameFunctions(const FunctionType* t1, const FunctionType* t2) {
	auto cmp = [](const Type* a, const Type* b) {
		return areSame(a, b);
	};
	const TypeList& p1 = t1->getParamTypes();
	const TypeList& p2 = t2->getParamTypes();
	return std::equal(p1.begin(), p1.end(), p2.begin(), p2.end(), cmp);
}

bool areSame(const Type* t1, const Type* t2) {
	if (t1 == t2) {
		return true;
	}

	if (t1->getKind() != t2->getKind()) {
		return false;
	}

	switch (t1->getKind()) {
	case Type::bool_kind:
	case Type::char_kind:
	case Type::int_kind:
	case Type::float_kind:
		return true;
	case Type::pointer_kind:
		return areSamePointers(static_cast<const PointerType*>(t1), static_cast<const PointerType*>(t2));
	case Type::reference_kind:
		return areSameReferences(static_cast<const ReferenceType*>(t1), static_cast<const ReferenceType*>(t2));
	case Type::function_kind:
		return areSameFunctions(static_cast<const FunctionType*>(t1), static_cast<const FunctionType*>(t2));
	}
}
#pragma once

class Type {};
class BooleanType {};
class IntegerType {};

class ReferenceType {
public:
	ReferenceType(Type* t)
		: object(t) {}

private:
	Type* object;
};
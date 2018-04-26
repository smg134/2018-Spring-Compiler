#pragma once
#include <iosfwd>

class Type;
class Expression;
class Statement;
class Declaration;

struct DebugPrinter {
	DebugPrinter(std::ostream& os)
		: os(os), depth(0) {}

	std::ostream& getStream() { return os; }

	int nesting() const { return depth; }
	void indent() { ++depth; }
	void undent() { --depth; }

	std::ostream& os;
	int depth;
};

void debug(DebugPrinter& d, const Type* t);
void debug(DebugPrinter& d, const Expression* e);
void debug(DebugPrinter& d, const Statement* s);
void debug(DebugPrinter& d, const Declaration* dc);
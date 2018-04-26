#pragma once
#include "Symbol.h"
#include <cassert>
#include <unordered_map>

class Declaration;

struct Scope : std::unordered_map<Symbol, Declaration*> {
	Scope(Scope* s = nullptr)
		: parent(s) {}

	virtual ~Scope() = default;

	Declaration* lookup(Symbol s) const {
		auto iterator = find(s);
		return iterator == end() ? nullptr : iterator->second;
	}

	void declare(Symbol s, Declaration* d) {
		assert(count(s) == 0);
		emplace(s, d);
	}

	Scope* parent;
};

struct GlobalScope : Scope {
	using Scope::Scope;
};

struct ParamenterScope : Scope {
	using Scope::Scope;
};

struct BlockScope : Scope {
	using Scope::Scope;
};
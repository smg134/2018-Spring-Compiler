#pragma once
#include <string>
#include <unordered_set>

using Symbol = const std::string*;

class SymbolTable {
public:
	Symbol get(const char* str);
	Symbol get(const std::string& str);

private:
	std::unordered_set<std::string> symbols;
};
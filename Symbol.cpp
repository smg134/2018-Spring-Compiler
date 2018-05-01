#pragma once
#include "stdafx.h"
#include "Symbol.h"

Symbol SymbolTable::get(const char* str) {
	return &*symbols.insert(str).first;
}

Symbol SymbolTable::get(const std::string& str) {
	return &*symbols.insert(str).first;
}
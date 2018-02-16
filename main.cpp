#include "stdafx.h"
#include "File.h"
#include "Lexer.h"
#include <iostream>

int main()
{
	File input("testFile.txt");
	SymbolTable syms;
	Lexer lex(syms, input);
	while (Token tok = lex())
		std::cout << tok << '\n';
}
#include "File.h"
#include "Lexer.h"
#include "Parser.h"
#include "Declaration.h"

int main() {
	File input("testFile.txt");
	SymbolTable syms;
	Parser p(syms, input);
	Declaration* d = p.parseProgram();	
}
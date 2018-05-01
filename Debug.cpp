#include "stdafx.h"
#include "Debug.h"
#include "Type.h"
#include "Expression.h"
#include "Statement.h"
#include "Declaration.h"

struct NodeFont {
	NodeFont(const char* c)
		: name(c) {}

	const char* name;
};

std::ostream& operator<<(std::ostream& os, NodeFont font) {
	return os << font.name;
}

struct AddressFont {
	AddressFont(const void* v)
		: pointer(v) {}

	const void* pointer;
};

std::ostream& operator<<(std::ostream& os, AddressFont font) {
	return os << font.pointer;
}

static void debugNode(DebugPrinter& d, const char* node, const void* pointer) {
	std::string tab(d.nesting() * 2, ' ');
	d.getStream() << tab << NodeFont(node) << ' ' << AddressFont(pointer) << '\n';
}

static const char* getNodeName(const Type* t) {
	switch (t->getKind()) {
	case Type::bool_kind: return "bool-type";
	case Type::char_kind: return "char-type";
	case Type::int_kind: return "int-type";
	case Type::float_kind: return "float-type";
	case Type::pointer_kind: return "pointer-type";
	case Type::reference_kind: return "reference-type";
	case Type::function_kind: return "function-type";
	}
}

void debug(DebugPrinter& d, const Type* t) {
	debugNode(d, getNodeName(t), t);
}

static const char* getNodeName(const Expression* e) {
	switch (e->getKind()) {
	case Expression::bool_kind: return "bool-expr";
	case Expression::int_kind: return "int-expr";
	case Expression::float_kind: return "float-expr";
	case Expression::id_kind: return "id-expr";
	case Expression::unop_kind: return "unop-expr";
	case Expression::binop_kind: return "binop-expr";
	case Expression::ptr_kind: return "ptr-expr";
	case Expression::call_kind: return "call-expr";
	case Expression::index_kind: return "index-expr";
	case Expression::cast_kind: return "cast-expr";
	case Expression::assign_kind: return "assign-expr";
	case Expression::cond_kind: return "cond-expr";
	case Expression::conv_kind: return "conv-expr";
	}
}

void debug(DebugPrinter& d, const Expression* e) {
	debugNode(d, getNodeName(e), e);
}

static const char* getNodeName(const Statement* s) {
	switch (s->getKind()) {
	case Statement::block_kind: return "block-stmt";
	case Statement::when_kind: return "when-stmt";
	case Statement::if_kind: return "if-stmt";
	case Statement::while_kind: return "while-stmt";
	case Statement::break_kind: return "break-stmt";
	case Statement::cont_kind: return "cont-stmt";
	case Statement::ret_kind: return "ret-stmt";
	case Statement::decl_kind: return "decl-stmt";
	case Statement::expr_kind: return "expr-stmt";
	}
}

static void debugChildren(DebugPrinter& d, const StatementList& stmts) {
	d.indent();
	for (const Statement* s : stmts) {
		debug(d, s);
	}
	d.undent();
}

void debugStatement(DebugPrinter& d, const BlockStatement* s) {
	debugChildren(d, s->getStatements());
}

void debugStatement(DebugPrinter& d, const WhenStatement* s) {
	d.indent();
	debug(d, s->getCondition());
	debug(d, s->getBody());
	d.undent();
}

void debugStatement(DebugPrinter& d, const IfStatement* s) {
	d.indent();
	debug(d, s->getCondition());
	debug(d, s->getPassValue());
	debug(d, s->getFailValue());
	d.undent();
}

void debugStatement(DebugPrinter& d, const WhileStatement* s) {
	d.indent();
	debug(d, s->getCondition());
	debug(d, s->getBody());
	d.undent();
}

void debugStatement(DebugPrinter& d, const ReturnStatement* s) {
	d.indent();
	debug(d, s->getValue());
	d.undent();
}

void debugStatement(DebugPrinter& d, const DeclareStatement* s) {
	d.indent();
	debug(d, s->getDeclaration());
	d.undent();
}

void debugStatement(DebugPrinter& d, const ExpressionStatement* s) {
	d.indent();
	debug(d, s->getExpression());
	d.undent();
}

void debug(DebugPrinter& d, const Statement* s) {
	debugNode(d, getNodeName(s), s);
	switch (s->getKind()) {
	case Statement::block_kind:
		return debugStatement(d, static_cast<const BlockStatement*>(s));
	case Statement::when_kind:
		return debugStatement(d, static_cast<const WhenStatement*>(s));
	case Statement::if_kind:
		return debugStatement(d, static_cast<const IfStatement*>(s));
	case Statement::while_kind:
		return debugStatement(d, static_cast<const WhileStatement*>(s));
	case Statement::break_kind:
		return; //todo
	case Statement::cont_kind:
		return; //todo
	case Statement::ret_kind:
		return debugStatement(d, static_cast<const ReturnStatement*>(s));
	case Statement::decl_kind:
		return debugStatement(d, static_cast<const DeclareStatement*>(s));
	case Statement::expr_kind:
		return debugStatement(d, static_cast<const ExpressionStatement*>(s));
	}
}

static const char* getNodeName(const Declaration* d) {
	switch (d->getKind()) {
	case Declaration::program_kind: return "program-decl";
	case Declaration::variable_kind: return "variable-decl";
	case Declaration::constant_kind: return "constant-decl";
	case Declaration::value_kind: return "value-decl";
	case Declaration::parameter_kind: return "parameter-decl";
	case Declaration::function_kind: return "function-decl";
	}
}

static void debugNode(DebugPrinter& d, const char* node, Declaration* dc) {
	std::string tab(d.nesting() * 2, ' ');
	d.getStream() << tab << NodeFont(node) << AddressFont(dc);
	if (dc->getName())
		d.getStream() << ' ' << "name=" << *dc->getName();
	d.getStream() << '\n';
}

static void debugChildren(DebugPrinter& d, const DeclarationList& dl) {
	d.indent();
	for (const Declaration* dc : dl) {
		debug(d, dc);
	}
	d.undent();
}

static void debugDeclaration(DebugPrinter& d, const ProgramDeclaration* p) {
	debugChildren(d, p->getDeclarations());
}

static void debugDeclaration(DebugPrinter& d, const ObjectDeclaration* o) {
	d.indent();
	debug(d, o->getType());
	if (const Expression* e = o->getInit()) {
		debug(d, e);
	}
	d.undent();
}

static void debugDeclaration(DebugPrinter& d, const FunctionDeclaration* f) {
	debugChildren(d, f->getParameters());
	d.indent();
	debug(d, f->getReturnType());
	debug(d, f->getBody());
	d.undent();
}

void debug(DebugPrinter& d, const Declaration* dc) {
	debugNode(d, getNodeName(dc), dc);
	switch (dc->getKind()) {
	case Declaration::program_kind:
		return debugDeclaration(d, static_cast<const ProgramDeclaration*>(dc));
	case Declaration::variable_kind:
		return debugDeclaration(d, static_cast<const VariableDeclaration*>(dc));
	case Declaration::constant_kind:
		return debugDeclaration(d, static_cast<const ConstantDeclaration*>(dc));
	case Declaration::value_kind:
		return debugDeclaration(d, static_cast<const ValueDeclaration*>(dc));
	case Declaration::parameter_kind:
		return debugDeclaration(d, static_cast<const ParameterDeclaration*>(dc));
	case Declaration::function_kind:
		return debugDeclaration(d, static_cast<const FunctionDeclaration*>(dc));
	}
}
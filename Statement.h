#pragma once
#include <vector>

class Expression;
class Declaration;

class Statement {
public:
	enum Kind {
		block_kind,
		when_kind,
		if_kind,
		while_kind,
		break_kind,
		cont_kind,
		ret_kind,
		decl_kind,
		expr_kind,
	};

protected:
	Statement(Kind k)
		: kind(k) {}

public:
	virtual ~Statement() = default;

	Kind getKind() const { return kind; }

private:
	Kind kind;
};

using StatementList = std::vector<Statement*>;

struct BlockStatement : Statement {
	BlockStatement(const StatementList& s)
		: Statement(block_kind), statements(s) {}

	const StatementList& getStatements() const { return statements; }
	StatementList& getStatements() { return statements; }

	StatementList statements;
};

struct WhenStatement : Statement {
	WhenStatement(Expression* e, Statement* s)
		: Statement(when_kind), condition(e), body(s) {}

	Expression* getCondition() const { return condition; }
	Statement* getBody() const { return body; }

	Expression* condition;
	Statement* body;
};

struct IfStatement : Statement {
	IfStatement(Expression* e, Statement* s1, Statement* s2)
		: Statement(if_kind), condition(e), pass(s1), fail(s2) {}

	Expression* getCondition() const { return condition; }
	Statement* getPassValue() const { return pass; }
	Statement* getFailValue() const { return fail; }

	Expression* condition;
	Statement* pass;
	Statement* fail;
};

struct WhileStatement : Statement {
	WhileStatement(Expression* e, Statement* s)
		: Statement(while_kind), condition(e), body(s) {}

	Expression* getCondition() const { return condition; }
	Statement* getBody() const { return body; }

	Expression* condition;
	Statement* body;
};

struct BreakStatement : Statement {
	BreakStatement()
		: Statement(break_kind) {}
};

struct ContinueStatement : Statement {
	ContinueStatement()
		: Statement(cont_kind) {}
};

struct ReturnStatement : Statement {
	ReturnStatement(Expression* e)
		: Statement(ret_kind), val(e) {}

	Expression* getValue() const { return val; }

	Expression* val;
};

struct DeclareStatement : Statement {
	DeclareStatement(Declaration* d)
		: Statement(decl_kind), declaration(d) {}

	Declaration* getDeclaration() const { return declaration; }

	Declaration* declaration;
};

struct ExpressionStatement : Statement {
	ExpressionStatement(Expression* e)
		: Statement(expr_kind), expression(e) {}

	Expression* getExpression() const { return expression; }

	Expression* expression;
};
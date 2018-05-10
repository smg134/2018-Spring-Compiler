#pragma once

class Type;
class Expression;
class Declaration;
class Statement;

class Context {
public:
	Context()
		: context(new llvm::LLVMContext()) {}

	~Context() { delete context; }

	llvm::LLVMContext *getContext() const { return context; }

	std::string getName(const Declaration* d);

	llvm::Type*	getType(const Type* t);
	llvm::Type* getBoolType(const BoolType* b);
	llvm::Type* getCharType(const CharType* c);
	llvm::Type* getIntType(const IntType* i);
	llvm::Type* getFloatType(const FloatType* f);
	llvm::Type* getReferenceType(const ReferenceType* r);
	llvm::Type* getFunctionType(const FunctionType* f);

	llvm::Type* getType(const TypedDeclaration* d);

private:
	llvm::LLVMContext* context;
};

class Module {
public:
	Module(Context& c, const ProgramDeclaration* p);

	llvm::LLVMContext* getContext() const { return parent->getContext(); }
	llvm::LLVMModule* getModule() const { return mod; }
	std::string getName(const Declaration* d) { return parent->getName(d); }
	llvm::Type* getType(const Type* t) { return parent->getType(t); }
	llvm::Type* getType(const TypedDeclaration* t) { return parent->getType(d); }

	void declare(const Declaration* d, llvm::GlobalValue* v);

	llvm::GlobalValue* lookup(const Declaration* d) const;

	void generate();
	void generate(const Declaration* d);
	void generateVariableDeclaration(const VariableDeclaration* d);
	void generateFunctionDeclaration(const FunctionDeclaration* d);

private:
	Context * parent;
	const ProgramDeclaration* program;
	llvm::Module* mod;
	VariableMap globalVariables;
};

class Function {
	Function(Module& m, const FunctionDeclaration* f);

	llvm::LLVMContext* getContext() const { return parent->getContext(); }
	llvm::Module* getModule() const { return parent->getModule(); }
	llvm::Function* getFunction() const { return function; }

	std::string getName(const Declaration* d) { return parent->getName(d); }

	llvm::Type* getType(const Type* t) { return parent->getType(t); }
	llvm::Type* get_type(const Expression* e) { return getType(e->getType()); }
	llvm::Type* get_type(const TypedDeclaration* t) { return parent->getType(t); }

	void declare(const Declaration* d, llvm::Value* v);

	llvm::Value* lookup(const Declaration* d) const;

	void define();

	llvm::BasicBlock* getEntryBlock() const { return entry; }
	llvm::BasicBlock* getCurrentBlock() const { return current; }
	llvm::BasicBlock* makeBlock(const char* c);

	void emitBlock(llvm::BasicBlock* b);

	llvm::Value* generateExpression(const Expression* e);
	llvm::Value* generateBoolExpression(const BoolExpression* e);
	llvm::Value* generateIntegerExpression(const IntExpression* e);
	llvm::Value* generateFloatExpression(const FloatExpression* e);
	llvm::Value* generateIdExpression(const IdExpression* e);
	llvm::Value* generateUnopExpression(const UnopExpression* e);
	llvm::Value* generateArithmeticExpression(const UnopExpression* e);
	llvm::Value* generateIntegerExpression(const UnopExpression* e);
	llvm::Value* generateFloatExpression(const UnopExpression* e);
	llvm::Value* generateBitwiseExpression(const UnopExpression* e);
	llvm::Value* generateLogicalExpression(const UnopExpression* e);
	llvm::Value* generateAddressExpression(const UnopExpression* e);
	llvm::Value* generateDereferenceExpression(const UnopExpression* e);
	llvm::Value* generateBinopExpression(const BinopExpression* e);
	llvm::Value* generateArithmeticExpression(const BinopExpression* e);
	llvm::Value* generateIntegerExpression(const BinopExpression* e);
	llvm::Value* generateFloatExpression(const BinopExpression* e);
	llvm::Value* generateBitwiseExpression(const BinopExpression* e);
	llvm::Value* generateLogicalExpression(const BinopExpression* e);
	llvm::Value* generateAndExpression(const BinopExpression* e);
	llvm::Value* generateOrExpression(const BinopExpression* e);
	llvm::Value* generateRelationalExpression(const BinopExpression* e);
	llvm::Value* generateCallExpression(const CallExpression* e);
	llvm::Value* generateIndexExpression(const IndexExpression* e);
	llvm::Value* generateCastExpression(const CastExpression* e);
	llvm::Value* generateConditionalExpression(const ConditionalExpression* e);
	llvm::Value* generateAssignmentExpression(const AssignmentExpression* e);
	llvm::Value* generateConversionExpression(const ConversionExpression* e);

	// Statements
	void generateStatement(const Statement* s);
	void generateBlockStatement(const BlockStatement* s);
	void generateWhen(const WhenStatement* s);
	void generateIfStatement(const IfStatement* s);
	void generateWhileStatement(const WhileStatement* s);
	void generateBreakStatement(const BreakStatement* s);
	void generateContinueStatement(const ContinueStatement* s);
	void generateReturnStatement(const ReturnStatement* s);
	void generateDeclarationStatement(const DeclareStatement* s);
	void generateExpressionStatement(const ExpressionStatement* s);

	// Local declarations
	void generateDeclaration(const Declaration* d);
	void generateVariableDeclaration(const VariableDeclaration* d);

	void makeVariable(const VariableDeclaration* d);
	void makeReference(const VariableDeclaration* d);


private:
	Module * parent;
	const FunctionDeclaration* source;
	llvm::Function* function;
	llvm::BasicBlock* entry;
	llvm::BasicBlock* current;
	VariableMap locals;
};


void generate(const Declaration* d);
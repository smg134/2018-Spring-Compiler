#include "CodeGen.h"
#include "Type.h"
#include "Expression.h"
#include "Declaration.h"
#include "Statement.h"

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/raw_ostream.h>

#include <sstream>
#include <unordered_map>

using VariableMap = std::unordered_map<const Declaration*, llvm::Value*>;

std::string Context::getName(const Declaration* d) {
	return *d->getName();
}

llvm::Type* Context::getType(const Type* t)
{
	switch (t->getKind()) {
	case Type::bool_kind:
		return getBoolType(static_cast<const BoolType*>(t));
	case Type::char_kind:
		return getCharType(static_cast<const CharType*>(t));
	case Type::int_kind:
		return getIntType(static_cast<const IntType*>(t));
	case Type::float_kind:
		return getFloatType(static_cast<const FloatType*>(t));
	case Type::reference_kind:
		return getReferenceType(static_cast<const ReferenceType*>(t));
	case Type::function_kind:
		return getFunctionType(static_cast<const FunctionType*>(t));

	}
}

llvm::Type* Context::getBoolType(const BoolType* b)
{
	return llvm::Type::getInt1Ty(*context);
}

llvm::Type* Context::getCharType(const CharType* c)
{
	return llvm::Type::getInt8Ty(*context);
}

llvm::Type* Context::getIntType(const IntType* i)
{
	return llvm::Type::getInt32Ty(*context)
}

llvm::Type* Context::getFloatType(const FloatType* f)
{
	return llvm::Type::getFloatTy(*context);
}

llvm::Type* Context::getReferenceType(const ReferenceType* r)
{
	llvm::Type* object = getType(r->getObjectType());
	return object->getPointerTo();
}

llvm::Type* Context::getFunctionType(const FunctionType* f)
{
	const TypeList& t = f->getParamTypes();
	std::vector<llvm::Type*> params(t.size());
	std::transform(t.begin(), t.end(), params.begin(), [this](const Type* y) {
		return getType(y);
	});
	llvm::Type* ret = getType(f->getReturnType());
	llvm::Type* base = llvm::FunctionType::get(ret, params, false);
	return base->getPointerTo();

}

llvm::Type* Context::getType(const TypedDeclaration* d)
{
	return getType(d->getType());
}

Module::Module(Context& c, const ProgramDeclaration* p)
	: parent(&c), program(p), mod(new llvm::Module("a.ll", *getContext())) {}

void Module::declare(const Declaration* d, llvm::GlobalValue* v)
{
	globalVariables.emplace(d, v);
}

llvm::GlobalValue* Module::lookup(const Declaration* d) const
{
	auto iterator = globalVariables.find(d);
	if (iterator != globalVariables.end())
		return llvm::cast<llvm::GlobalValue>(iterator->second);
	else
		return nullptr;
}

void Module::generate()
{
	for (const Declaration* d : program->getDeclarations())
		generate(d);
}

void Module::generate(const Declaration* d)
{
	switch (d->getKind()) {
	case Declaration::variable_kind:
		return generateVariableDeclaration(static_cast<const VariableDeclaration*>(d));
	case Declaration::function_kind:
		return generateFunctionDeclaration(static_cast<const FunctionDeclaration*>(d));
	}
}

void Module::generateVariableDeclaration(const VariableDeclaration* d)
{
	std::string n = getName(d);
	llvm::Type* t = getType(d->getType());
	llvm::Constant* c = llvm::Constant::getNullValue(t);
	llvm::GlobalVariable* g = new llvm::GlobalVariable(
		*mod, t, false, llvm::GlobalVariable::ExternalLinkage, c, n);
	declare(d, g);
}

void Module::generateFunctionDeclaration(const FunctionDeclaration* d)
{
	Function function(*this, d);
	function.define();
}

static llvm::FunctionType* getFunctionType(llvm::Type* t) {
	return llvm::cast<llvm::FunctionType>(t->getPointerElementType());
}

Function::Function(Module& m, const FunctionDeclaration* f)
	: parent(&m), source(f) {
	std::string n = getName(f);
	llvm::Type* t = getType(f);
	function = llvm::Function::Create(getFunctionType(f), llvm::Function::ExternalLinkage, n, getModule());

	parent->declare(f, function);

	entry = makeBlock("entry");
	emitBlock(entry);

	llvm::IRBuilder<> ir(getCurrentBlock());

	auto pi = f->getParameters().begin();
	auto ai = function->arg_begin();
	while (ai != fn->arg_end()) {
		const ParameterDeclaration* param = static_cast<const ParameterDeclaration*>(*pi);
		llvm::Argument& arg = *ai;

		// Configure each parameter.
		arg.setName(getName(param));

		// Declare local variable for each parameter and initialize it
		// with wits corresponding value.
		llvm::Value* var = ir.CreateAlloca(arg.getType(), nullptr, arg.getName());
		declare(param, var);

		// Initialize with the value of the argument.
		ir.CreateStore(&arg, var);

		++ai;
		++pi;
	}
}

void Function::declare(const Declaration* d, llvm::Value* v)
{
	localVariables.emplace(d, v);
}

llvm::Value* Function::lookup(const Declaration* d) const
{
	auto iter = localVariables.find(d);
	if (iter != localVariables.end())
		return iter->second;
	else
		return parent->lookup(d);
}

llvm::BasicBlock* Function::makeBlock(const char* c)
{
	return llvm::BasicBlock::Create(*getContext(), c);
}

void Function::emitBlock(llvm::BasicBlock* b)
{
	b->insertInto(getFunction());
	current = b;
}

void Function::define()
{
	generateStatement(source->getBody());
}


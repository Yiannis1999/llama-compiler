#include "ast.hpp"

using namespace llvm;

LLVMContext AST::TheContext;
IRBuilder<> AST::Builder(TheContext);
std::unique_ptr<Module> AST::TheModule;
std::unique_ptr<legacy::FunctionPassManager> AST::TheFPM;

GlobalVariable *AST::TheVars;
GlobalVariable *AST::TheNL;

// Write Functions
Function *AST::TheWriteBoolean;
Function *AST::TheWriteInteger;
Function *AST::TheWriteChar;
Function *AST::TheWriteString;
Function *AST::TheWriteReal;

// Read Functions
Function *AST::TheReadInteger;
Function *AST::TheReadBoolean;
Function *AST::TheReadChar;
Function *AST::TheReadReal;
Function *AST::TheReadString;

// Math Functions
Function *AST::abs;
Function *AST::fabs;
Function *AST::sqrt;
Function *AST::cos;
Function *AST::tan;
Function *AST::atan;
Function *AST::exp;
Function *AST::ln;
Function *AST::pi;

// Type Declarations
llvm::Type *AST::i8;
llvm::Type *AST::i32;
llvm::Type *AST::i64;
llvm::Type *AST::flo;
llvm::Type *AST::voi;

void Program::llvm_compile_and_dump(bool optimize = false)
{
  // Initialize
  TheModule = std::make_unique<Module>("Llama program", TheContext);
  TheFPM = std::make_unique<legacy::FunctionPassManager>(TheModule.get());
  if (optimize)
  {
    TheFPM->add(createPromoteMemoryToRegisterPass());
    TheFPM->add(createInstructionCombiningPass());
    TheFPM->add(createReassociatePass());
    TheFPM->add(createGVNPass());
    TheFPM->add(createCFGSimplificationPass());
  }
  TheFPM->doInitialization();

  // Initialize types
  i8 = IntegerType::get(TheContext, 8);
  i32 = IntegerType::get(TheContext, 32);
  i64 = IntegerType::get(TheContext, 64);
  flo = llvm::Type::getFloatTy(TheContext);
  voi = llvm::Type::getVoidTy(TheContext);

  // Initialize Write Library Functions
  FunctionType *writeInteger_type = FunctionType::get(voi, {i64}, false);
  TheWriteInteger = Function::Create(writeInteger_type, Function::ExternalLinkage, str_print_int, TheModule.get());
  FunctionType *writeBoolean_type = FunctionType::get(voi, {i8}, false);
  TheWriteBoolean = Function::Create(writeBoolean_type, Function::ExternalLinkage, str_print_bool, TheModule.get());
  FunctionType *writeChar_type = FunctionType::get(voi, {i8}, false);
  TheWriteChar = Function::Create(writeChar_type, Function::ExternalLinkage, str_print_char, TheModule.get());
  FunctionType *writeReal_type = FunctionType::get(voi, {flo}, false);
  TheWriteReal = Function::Create(writeReal_type, Function::ExternalLinkage, str_print_float, TheModule.get());
  FunctionType *writeString_type = FunctionType::get(voi, {PointerType::get(i8, 0)}, false);
  TheWriteString = Function::Create(writeString_type, Function::ExternalLinkage, str_print_string, TheModule.get());

  // Initialize Read Library Functions
  FunctionType *ReadInteger_type = FunctionType::get(i64, {}, false);
  TheReadInteger = Function::Create(ReadInteger_type, Function::ExternalLinkage, "readInteger", TheModule.get());
  FunctionType *ReadBoolean_type = FunctionType::get(i8, {}, false);
  TheReadBoolean = Function::Create(ReadBoolean_type, Function::ExternalLinkage, "readBoolean", TheModule.get());
  FunctionType *ReadChar_type = FunctionType::get(i8, {}, false);
  TheReadChar = Function::Create(ReadChar_type, Function::ExternalLinkage, "readChar", TheModule.get());
  FunctionType *ReadReal_type = FunctionType::get(flo, {}, false);
  TheReadReal = Function::Create(ReadReal_type, Function::ExternalLinkage, "readReal", TheModule.get());
  FunctionType *ReadString_type = FunctionType::get(PointerType::get(PointerType::get(i8, 0), 0), {}, false);
  TheReadString = Function::Create(ReadString_type, Function::ExternalLinkage, "readString", TheModule.get());

  // Initialize Math Functions

  // Define and start the main function.
  FunctionType *main_type = FunctionType::get(i64, {}, false);
  Function *main = Function::Create(main_type, Function::ExternalLinkage, "main", TheModule.get());
  BasicBlock *BB = BasicBlock::Create(TheContext, "entry", main);
  Builder.SetInsertPoint(BB);
  // Emit the program code.
  compile();
  Builder.CreateRet(c64(0));
  // Verify the IR.
  bool bad = verifyModule(*TheModule, &errs());
  if (bad)
  {
    std::cout << std::endl
              << std::endl
              << std::endl;
    std::cerr << "The IR is bad!" << std::endl;
    TheModule->print(errs(), nullptr);
    std::exit(1);
  }
  // Optimize!
  TheFPM->run(*main);
  // Print out the IR.
  TheModule->print(outs(), nullptr);
}

Value *Program::compile() const
{
  // Compile each statement
  for (Stmt *stmt : *statements)
  {
    stmt->compile();
  }
  return nullptr;
}

Value *LetDef::compile() const
{
  for (Def *def : *def_vec)
  {
    def->compile();
  }
  return nullptr;
}

Value *NormalDef::compile() const
{
  if (par_vec->size() == 0) // variable
  {
    Value *v = expr->compile();
    if (typ->get_type() != type_unit)
    {
      llvm::Type *t = typ->compile();
      llvm::GlobalVariable *globalVar = new GlobalVariable(*TheModule, t, false, GlobalValue::PrivateLinkage, ConstantAggregateZero::get(t), id);
      Builder.CreateCall(TheWriteChar, std::vector<Value *>{nullptr});
      Builder.CreateStore(v, globalVar);
    }
  }

  return nullptr;
}

Value *call::compile() const
{
  std::vector<Value *> value_vec;
  for (Expr *expr : *expr_vec)
  {
    value_vec.push_back(expr->compile());
  }
  Value *ret = Builder.CreateCall(TheModule->getFunction(id), value_vec);
  return ret;
}

Value *Int_Expr::compile() const
{
  return c64(num);
}

Value *Float_Expr::compile() const
{
  return cfloat(num);
}

Value *Char_Expr::compile() const
{
  return c8(chr);
}

Value *Str_Expr::compile() const
{
  Value *strPtr = Builder.CreateGlobalStringPtr(str);
  return strPtr;
}

Value *Bool_Expr::compile() const
{
  return c8(var);
}

Value *id_Expr::compile() const
{
  llvm::GlobalVariable *var = TheModule->getGlobalVariable(id, true);
  if (var == nullptr)
    return nullptr;
  return Builder.CreateLoad(var, id);
}
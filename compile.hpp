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
  TheReadInteger = Function::Create(ReadInteger_type, Function::ExternalLinkage, str_read_int, TheModule.get());
  FunctionType *ReadBoolean_type = FunctionType::get(i8, {}, false);
  TheReadBoolean = Function::Create(ReadBoolean_type, Function::ExternalLinkage, str_read_bool, TheModule.get());
  FunctionType *ReadChar_type = FunctionType::get(i8, {}, false);
  TheReadChar = Function::Create(ReadChar_type, Function::ExternalLinkage, str_read_char, TheModule.get());
  FunctionType *ReadReal_type = FunctionType::get(flo, {}, false);
  TheReadReal = Function::Create(ReadReal_type, Function::ExternalLinkage, str_read_float, TheModule.get());
  FunctionType *ReadString_type = FunctionType::get(PointerType::get(PointerType::get(i8, 0), 0), {}, false);
  TheReadString = Function::Create(ReadString_type, Function::ExternalLinkage, str_read_string, TheModule.get());

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
  for (Stmt *stmt : *statements)
  {
    stmt->compile();
  }
  return nullptr;
}

llvm::Type *Type_Func::compile() const
{
  std::vector<::Type *> tmp_vec = {from};
  std::vector<llvm::Type *> from_vec = {};
  ::Type *tmp = to;
  while (tmp->get_type() == type_func)
  {
    tmp_vec.push_back(tmp->getChild1());
    tmp = tmp->getChild2();
  }
  for (::Type *t : tmp_vec)
  {
    switch (t->get_type())
    {
    case type_unit:
      break;
    default:
      from_vec.push_back(t->compile());
      break;
    }
  }
  FunctionType *fn_type = FunctionType::get(tmp->compile(), from_vec, false);
  PointerType *fn_ptr_type = PointerType::getUnqual(fn_type);
  return fn_ptr_type;
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
      GlobalVariable *globalVar = new GlobalVariable(*TheModule, t, false, GlobalValue::PrivateLinkage, ConstantAggregateZero::get(t), id);
      Builder.CreateStore(v, globalVar);
    }
  }
  else // function
  {
    std::vector<llvm::Type *> from = {};
    for (Par *par : *par_vec)
    {
      llvm::Type *t = par->typ->compile();
      switch (par->typ->get_type())
      {
      case type_unit:
        break;
      default:
        from.push_back(t);
        break;
      }
    }
    llvm::Type *to = typ->compile();
    FunctionType *fn_type = FunctionType::get(to, from, false);
    Function *func = Function::Create(fn_type, Function::ExternalLinkage, id, TheModule.get());
    Function::arg_iterator arg = func->arg_begin();
    for (Par *par : *par_vec)
    {
      switch (par->typ->get_type())
      {
      case type_unit:
        break;
      default:
        arg->setName(par->id);
        std::advance(arg, 1);
        break;
      }
    }
    BasicBlock *PrevBB = Builder.GetInsertBlock();
    BasicBlock *BodyBB = BasicBlock::Create(TheContext, id, func);
    Builder.SetInsertPoint(BodyBB);
    Builder.CreateRet(expr->compile());
    Builder.SetInsertPoint(PrevBB);
  }
  return nullptr;
}

Value *call::compile() const
{
  std::vector<Value *> value_vec;
  for (Expr *expr : *expr_vec)
  {
    if (expr->typ->get_type() != type_unit)
      value_vec.push_back(expr->compile());
  }
  Function *func = TheModule->getFunction(id);
  if (func == nullptr) // argument
  {
    Function *f = Builder.GetInsertBlock()->getParent();
    for (Function::arg_iterator arg = f->arg_begin(); arg != f->arg_end(); arg++)
    {
      if (arg->getName() == id)
      {
        Value *fptr = arg;
        PointerType *fn_ptr_type = dyn_cast<PointerType>(arg->getType());
        FunctionType *fn_type = dyn_cast<FunctionType>(fn_ptr_type->getElementType());
        if (typ->get_type() == type_unit)
        {
          Builder.CreateCall(fn_type, fptr, value_vec);
          return nullptr;
        }
        return Builder.CreateCall(fn_type, fptr, value_vec, "calltmp");
      }
    }
  }
  if (typ->get_type() == type_unit)
  {
    Builder.CreateCall(func, value_vec);
    return nullptr;
  }
  return Builder.CreateCall(func, value_vec, "calltmp");
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
  return c8(ch);
}

Value *Str_Expr::compile() const
{
  Value *strPtr = Builder.CreateGlobalStringPtr(str);
  return strPtr;
}

Value *Bool_Expr::compile() const
{
  return c8(boolean);
}

Value *id_Expr::compile() const
{
  // constant or variable
  GlobalVariable *var = TheModule->getGlobalVariable(id, true);
  if (var != nullptr)
    return Builder.CreateLoad(var, id);
  // function
  Function *func = TheModule->getFunction(id);
  if (func != nullptr)
  {
    FunctionType *fn_type = func->getFunctionType();
    PointerType *fn_ptr_type = PointerType::getUnqual(fn_type);
    Value *fptr = ConstantExpr::getBitCast(func, fn_ptr_type);
    return fptr;
  }
  // argument
  Function *f = Builder.GetInsertBlock()->getParent();
  for (Function::arg_iterator arg = f->arg_begin(); arg != f->arg_end(); arg++)
    if (arg->getName() == id)
      return arg;
  return nullptr;
}
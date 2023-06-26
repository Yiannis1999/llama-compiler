#include "ast.hpp"

using namespace llvm;

LLVMContext AST::TheContext;
IRBuilder<> AST::Builder(TheContext);
std::unique_ptr<Module> AST::TheModule;
std::unique_ptr<legacy::FunctionPassManager> AST::TheFPM;

// Type Declarations
llvm::Type *AST::i1;
llvm::Type *AST::i8;
llvm::Type *AST::i32;
llvm::Type *AST::i64;
llvm::Type *AST::flo;
StructType *AST::voi;

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
  i1 = IntegerType::get(TheContext, 1);
  i8 = IntegerType::get(TheContext, 8);
  i32 = IntegerType::get(TheContext, 32);
  i64 = IntegerType::get(TheContext, 64);
  flo = llvm::Type::getFloatTy(TheContext);
  voi = StructType::create(TheContext, {}, "void");

  // Initialize malloc
  FunctionType *malloc_type = FunctionType::get(PointerType::get(i64, 0), {i64}, false);
  Function::Create(malloc_type, Function::ExternalLinkage, "malloc", TheModule.get());
  FunctionType *free_type = FunctionType::get(voi, {PointerType::get(i64, 0)}, false);
  Function::Create(free_type, Function::ExternalLinkage, "free", TheModule.get());

  // Initialize exit
  FunctionType *exit_type = FunctionType::get(voi, {i64}, false);
  Function::Create(exit_type, Function::ExternalLinkage, "exit", TheModule.get());

  // Initialize pow
  FunctionType *pow_type = FunctionType::get(flo, {flo, flo}, false);
  Function::Create(pow_type, Function::ExternalLinkage, "powf", TheModule.get());

  // Initialize Write Functions
  FunctionType *print_int_type = FunctionType::get(voi, {i64}, false);
  Function::Create(print_int_type, Function::ExternalLinkage, str_print_int, TheModule.get());
  FunctionType *print_bool_type = FunctionType::get(voi, {i1}, false);
  Function::Create(print_bool_type, Function::ExternalLinkage, str_print_bool, TheModule.get());
  FunctionType *print_char_type = FunctionType::get(voi, {i8}, false);
  Function::Create(print_char_type, Function::ExternalLinkage, str_print_char, TheModule.get());
  FunctionType *print_float_type = FunctionType::get(voi, {flo}, false);
  Function::Create(print_float_type, Function::ExternalLinkage, str_print_float, TheModule.get());
  FunctionType *print_string_type = FunctionType::get(voi, {PointerType::get(i8, 0)}, false);
  Function::Create(print_string_type, Function::ExternalLinkage, str_print_string, TheModule.get());

  // Initialize Read Functions
  FunctionType *read_int_type = FunctionType::get(i64, {voi}, false);
  Function::Create(read_int_type, Function::ExternalLinkage, str_read_int, TheModule.get());
  FunctionType *read_bool_type = FunctionType::get(i1, {voi}, false);
  Function::Create(read_bool_type, Function::ExternalLinkage, str_read_bool, TheModule.get());
  FunctionType *read_char_type = FunctionType::get(i8, {voi}, false);
  Function::Create(read_char_type, Function::ExternalLinkage, str_read_char, TheModule.get());
  FunctionType *read_float_type = FunctionType::get(flo, {voi}, false);
  Function::Create(read_float_type, Function::ExternalLinkage, str_read_float, TheModule.get());
  FunctionType *read_string_type = FunctionType::get(voi, {PointerType::get(i8, 0)}, false);
  Function::Create(read_string_type, Function::ExternalLinkage, str_read_string, TheModule.get());

  // Initialize Math Functions
  FunctionType *abs_type = FunctionType::get(i64, {i64}, false);
  Function::Create(abs_type, Function::ExternalLinkage, str_abs, TheModule.get());
  FunctionType *fabs_type = FunctionType::get(flo, {flo}, false);
  Function::Create(fabs_type, Function::ExternalLinkage, str_fabs, TheModule.get());
  FunctionType *sqrt_type = FunctionType::get(flo, {flo}, false);
  Function::Create(sqrt_type, Function::ExternalLinkage, str_sqrt, TheModule.get());
  FunctionType *sin_type = FunctionType::get(flo, {flo}, false);
  Function::Create(sin_type, Function::ExternalLinkage, str_sin, TheModule.get());
  FunctionType *cos_type = FunctionType::get(flo, {flo}, false);
  Function::Create(cos_type, Function::ExternalLinkage, str_cos, TheModule.get());
  FunctionType *tan_type = FunctionType::get(flo, {flo}, false);
  Function::Create(tan_type, Function::ExternalLinkage, str_tan, TheModule.get());
  FunctionType *atan_type = FunctionType::get(flo, {flo}, false);
  Function::Create(atan_type, Function::ExternalLinkage, str_atan, TheModule.get());
  FunctionType *exp_type = FunctionType::get(flo, {flo}, false);
  Function::Create(exp_type, Function::ExternalLinkage, str_exp, TheModule.get());
  FunctionType *ln_type = FunctionType::get(flo, {flo}, false);
  Function::Create(ln_type, Function::ExternalLinkage, str_ln, TheModule.get());
  FunctionType *pi_type = FunctionType::get(flo, {voi}, false);
  Function::Create(pi_type, Function::ExternalLinkage, str_pi, TheModule.get());

  // Initialize incr decr
  FunctionType *incr_type = FunctionType::get(voi, {PointerType::get(i64, 0)}, false);
  Function::Create(incr_type, Function::ExternalLinkage, str_incr, TheModule.get());
  FunctionType *decr_type = FunctionType::get(voi, {PointerType::get(i64, 0)}, false);
  Function::Create(decr_type, Function::ExternalLinkage, str_decr, TheModule.get());

  // Initialize Convertion Functions
  FunctionType *float_of_int_type = FunctionType::get(flo, {i64}, false);
  Function::Create(float_of_int_type, Function::ExternalLinkage, str_float_of_int, TheModule.get());
  FunctionType *int_of_float_type = FunctionType::get(i64, {flo}, false);
  Function::Create(int_of_float_type, Function::ExternalLinkage, str_int_of_float, TheModule.get());
  FunctionType *round_type = FunctionType::get(i64, {flo}, false);
  Function::Create(round_type, Function::ExternalLinkage, str_round, TheModule.get());
  FunctionType *int_of_char_type = FunctionType::get(i64, {i8}, false);
  Function::Create(int_of_char_type, Function::ExternalLinkage, str_int_of_char, TheModule.get());
  FunctionType *char_of_int_type = FunctionType::get(i8, {i64}, false);
  Function::Create(char_of_int_type, Function::ExternalLinkage, str_char_of_int, TheModule.get());

  // Initialize String Functions
  FunctionType *strlen_type = FunctionType::get(i64, {PointerType::get(i8, 0)}, false);
  Function::Create(strlen_type, Function::ExternalLinkage, str_strlen, TheModule.get());
  FunctionType *strcmp_type = FunctionType::get(i64, {PointerType::get(i8, 0), PointerType::get(i8, 0)}, false);
  Function::Create(strcmp_type, Function::ExternalLinkage, str_strcmp, TheModule.get());
  FunctionType *strcpy_type = FunctionType::get(voi, {PointerType::get(i8, 0), PointerType::get(i8, 0)}, false);
  Function::Create(strcpy_type, Function::ExternalLinkage, str_strcpy, TheModule.get());
  FunctionType *strcat_type = FunctionType::get(voi, {PointerType::get(i8, 0), PointerType::get(i8, 0)}, false);
  Function::Create(strcat_type, Function::ExternalLinkage, str_strcat, TheModule.get());

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

void Program::compile() const
{
  for (Stmt *stmt : *statements)
  {
    stmt->compile();
  }
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
    from_vec.push_back(t->compile());
  }
  FunctionType *fn_type = FunctionType::get(tmp->compile(), from_vec, false);
  PointerType *fn_ptr_type = PointerType::getUnqual(fn_type);
  return fn_ptr_type;
}

llvm::Type *Type_Ref::compile() const
{
  return PointerType::get(typ->compile(), 0);
}

llvm::Type *Type_id::compile() const
{
  return PointerType::get(i64, 0);
}

Value *call::compile() const
{
  std::vector<Value *> value_vec;
  for (Expr *expr : *expr_vec)
  {
    value_vec.push_back(expr->compile());
  }
  Function *func = TheModule->getFunction(id);
  if (func == nullptr) // argument
  {
    GlobalVariable *var = TheModule->getGlobalVariable(id, true);
    Value *fptr = Builder.CreateLoad(var);
    PointerType *fn_ptr_type = dyn_cast<PointerType>(fptr->getType());
    FunctionType *fn_type = dyn_cast<FunctionType>(fn_ptr_type->getElementType());
    return Builder.CreateCall(fn_type, fptr, value_vec, "calltmp");
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
  std::vector<Constant *> value_vec;
  value_vec.push_back(c8(str.length()));
  for (int i = 0; i < 7; i++)
    value_vec.push_back(c8(0));
  for (char c : str)
    value_vec.push_back(c8(c));
  value_vec.push_back(c8(0));
  ArrayType *str_type = ArrayType::get(i8, value_vec.size());
  Value *str = new GlobalVariable(*TheModule, str_type, true, GlobalValue::PrivateLinkage, ConstantArray::get(str_type, value_vec), "str");
  return Builder.CreateGEP(str, {c64(0), c64(8)}, "strtmp");
}

Value *Bool_Expr::compile() const
{
  return c1(boolean);
}

Value *Unit_Expr::compile() const
{
  return cvoid();
}

Value *Array::compile() const
{
  Value *ptr = Builder.CreateLoad(TheModule->getGlobalVariable(id, true));
  Value *ptr64 = Builder.CreateBitCast(ptr, PointerType::get(i64, 0));
  Value *offset = c64(0);
  Value *coeff = c64(1);
  int i = -expr_vec->size();
  for (auto e = expr_vec->rbegin(); e != expr_vec->rend(); e++)
  {
    Value *v = (*e)->compile();
    offset = Builder.CreateAdd(offset, Builder.CreateMul(v, coeff));
    Value *dim = Builder.CreateLoad(Builder.CreateGEP(ptr64, {c64(i++)}));
    coeff = Builder.CreateMul(coeff, dim);
  }
  return Builder.CreateGEP(ptr, {offset}, id + "_ptr");
}

Value *Dim::compile() const
{
  Value *ptr = Builder.CreateLoad(TheModule->getGlobalVariable(id, true));
  Value *ptr64 = Builder.CreateBitCast(ptr, PointerType::get(i64, 0));
  return Builder.CreateLoad(Builder.CreateGEP(ptr64, {c64(-ind)}, "dimtmp"));
}

Value *id_Expr::compile() const
{
  // constant or variable
  GlobalVariable *var = TheModule->getGlobalVariable(id, true);
  if (var != nullptr)
    return Builder.CreateLoad(var, "idtmp");
  // function
  Function *func = TheModule->getFunction(id);
  if (func != nullptr)
  {
    FunctionType *fn_type = func->getFunctionType();
    PointerType *fn_ptr_type = PointerType::getUnqual(fn_type);
    Value *fptr = ConstantExpr::getBitCast(func, fn_ptr_type);
    return fptr;
  }
  return nullptr;
}

Value *Id_Expr::compile() const
{
  Function *func = TheModule->getFunction(Id);
  return Builder.CreateCall(func, {}, "calltmp");
}

Value *While::compile() const
{
  BasicBlock *PrevBB = Builder.GetInsertBlock();
  Function *TheFunction = PrevBB->getParent();
  BasicBlock *LoopBB = BasicBlock::Create(TheContext, "loop", TheFunction);
  BasicBlock *BodyBB = BasicBlock::Create(TheContext, "body", TheFunction);
  BasicBlock *AfterBB = BasicBlock::Create(TheContext, "endwhile", TheFunction);
  Builder.CreateBr(LoopBB);
  Builder.SetInsertPoint(LoopBB);
  Value *loop_cond = cond->compile();
  Builder.CreateCondBr(loop_cond, BodyBB, AfterBB);
  Builder.SetInsertPoint(BodyBB);
  stmt->compile();
  Builder.CreateBr(LoopBB);
  Builder.SetInsertPoint(AfterBB);
  return cvoid();
}

Value *For::compile() const
{
  GlobalVariable *var = new GlobalVariable(*TheModule, i64, false, GlobalValue::PrivateLinkage, ConstantAggregateZero::get(i64), id);
  Builder.CreateStore(start->compile(), var);
  Value *v = end->compile();
  BasicBlock *PrevBB = Builder.GetInsertBlock();
  Function *TheFunction = PrevBB->getParent();
  BasicBlock *LoopBB = BasicBlock::Create(TheContext, "loop", TheFunction);
  BasicBlock *BodyBB = BasicBlock::Create(TheContext, "body", TheFunction);
  BasicBlock *AfterBB = BasicBlock::Create(TheContext, "endfor", TheFunction);
  Builder.CreateBr(LoopBB);
  Builder.SetInsertPoint(LoopBB);
  Value *iter = Builder.CreateLoad(var, "iter");
  Value *loop_cond;
  if (down)
    loop_cond = Builder.CreateICmpSGE(iter, v, "loop_cond");
  else
    loop_cond = Builder.CreateICmpSLE(iter, v, "loop_cond");
  Builder.CreateCondBr(loop_cond, BodyBB, AfterBB);
  Builder.SetInsertPoint(BodyBB);
  stmt->compile();
  Value *new_iter;
  if (down)
    new_iter = Builder.CreateSub(iter, c64(1), "new_iter");
  else
    new_iter = Builder.CreateAdd(iter, c64(1), "new_iter");
  Builder.CreateStore(new_iter, var);
  Builder.CreateBr(LoopBB);
  Builder.SetInsertPoint(AfterBB);
  return cvoid();
}

Value *UnOp::compile() const
{
  Value *v = expr->compile();
  switch (op)
  {
  case unop_plus:
    return v;
  case unop_minus:
    return Builder.CreateNeg(v, "negtmp");
  case unop_float_plus:
    return v;
  case unop_float_minus:
    return Builder.CreateFNeg(v, "fnegtmp");
  case unop_exclamation:
    return Builder.CreateLoad(v, "dereftmp");
  case unop_not:
    return Builder.CreateNot(v, "nottmp");
  case unop_delete:
    v = Builder.CreateBitCast(v, PointerType::get(i64, 0));
    return Builder.CreateCall(TheModule->getFunction("free"), {v});
  default:
    return cvoid();
  }
}

Value *BinOp::compile() const
{
  Value *l = left->compile();
  ::Type *l_typ = left->typ;
  if (op == binop_and)
  {
    Function *TheFunction = Builder.GetInsertBlock()->getParent();
    BasicBlock *ThenBB = BasicBlock::Create(TheContext, "then", TheFunction);
    BasicBlock *ElseBB = BasicBlock::Create(TheContext, "else", TheFunction);
    BasicBlock *AfterBB = BasicBlock::Create(TheContext, "endif", TheFunction);
    Builder.CreateCondBr(l, ThenBB, ElseBB);
    Builder.SetInsertPoint(ThenBB);
    Value *r = right->compile();
    ThenBB = Builder.GetInsertBlock();
    Builder.CreateBr(AfterBB);
    Builder.SetInsertPoint(ElseBB);
    Builder.CreateBr(AfterBB);
    Builder.SetInsertPoint(AfterBB);
    PHINode *phi = Builder.CreatePHI(typ->compile(), 2, "phi");
    phi->addIncoming(r, ThenBB);
    phi->addIncoming(c1(false), ElseBB);
    return phi;
  }
  if (op == binop_or)
  {
    Function *TheFunction = Builder.GetInsertBlock()->getParent();
    BasicBlock *ThenBB = BasicBlock::Create(TheContext, "then", TheFunction);
    BasicBlock *ElseBB = BasicBlock::Create(TheContext, "else", TheFunction);
    BasicBlock *AfterBB = BasicBlock::Create(TheContext, "endif", TheFunction);
    Builder.CreateCondBr(l, ThenBB, ElseBB);
    Builder.SetInsertPoint(ThenBB);
    Builder.CreateBr(AfterBB);
    Builder.SetInsertPoint(ElseBB);
    Value *r = right->compile();
    ElseBB = Builder.GetInsertBlock();
    Builder.CreateBr(AfterBB);
    Builder.SetInsertPoint(AfterBB);
    PHINode *phi = Builder.CreatePHI(typ->compile(), 2, "phi");
    phi->addIncoming(c1(true), ThenBB);
    phi->addIncoming(r, ElseBB);
    return phi;
  }
  Value *r = right->compile();
  switch (op)
  {
  case binop_plus:
    return Builder.CreateAdd(l, r, "addtmp");
  case binop_minus:
    return Builder.CreateSub(l, r, "subtmp");
  case binop_mult:
    return Builder.CreateMul(l, r, "multmp");
  case binop_div:
    return Builder.CreateSDiv(l, r, "divtmp");
  case binop_mod:
    return Builder.CreateSRem(l, r, "modtmp");
  case binop_float_plus:
    return Builder.CreateFAdd(l, r, "faddtmp");
  case binop_float_minus:
    return Builder.CreateFSub(l, r, "fsubtmp");
  case binop_float_mult:
    return Builder.CreateFMul(l, r, "fmultmp");
  case binop_float_div:
    return Builder.CreateFDiv(l, r, "fdivtmp");
  case binop_pow:
    return Builder.CreateCall(TheModule->getFunction("powf"), {l, r}, "fpowtmp");
  case binop_struct_eq:
    while (l_typ->get_type() == type_ref)
    {
      l_typ = l_typ->getChild1();
      l = Builder.CreateLoad(l);
      r = Builder.CreateLoad(r);
    }
    switch (l_typ->get_type())
    {
    case type_unit:
      return c1(true);
    case type_float:
      return Builder.CreateFCmpOEQ(l, r, "eqtmp");
    case type_id:
      return Builder.CreateCall(TheModule->getFunction(l_typ->get_id() + "_cmp"), {l, r}, "eqtmp");
    default:
      return Builder.CreateICmpEQ(l, r, "eqtmp");
    }
  case binop_struct_ne:
    while (l_typ->get_type() == type_ref)
    {
      l_typ = l_typ->getChild1();
      l = Builder.CreateLoad(l);
      r = Builder.CreateLoad(r);
    }
    switch (l_typ->get_type())
    {
    case type_unit:
      return c1(true);
    case type_float:
      return Builder.CreateFCmpONE(l, r, "netmp");
    case type_id:
      return Builder.CreateNot(Builder.CreateCall(TheModule->getFunction(l_typ->get_id() + "_cmp"), {l, r}), "netmp");
    default:
      return Builder.CreateICmpNE(l, r, "netmp");
    }
  case binop_phys_eq:
    switch (l_typ->get_type())
    {
    case type_unit:
      return c1(true);
    case type_float:
      return Builder.CreateFCmpOEQ(l, r, "eqtmp");
    default:
      return Builder.CreateICmpEQ(l, r, "eqtmp");
    }
  case binop_phys_ne:
    switch (l_typ->get_type())
    {
    case type_unit:
      return c1(true);
    case type_float:
      return Builder.CreateFCmpONE(l, r, "netmp");
    default:
      return Builder.CreateICmpNE(l, r, "netmp");
    }
  case binop_l:
    if (l_typ->get_type() == type_float)
      return Builder.CreateFCmpOLT(l, r, "lttmp");
    return Builder.CreateICmpSLT(l, r, "lttmp");
  case binop_g:
    if (l_typ->get_type() == type_float)
      return Builder.CreateFCmpOGT(l, r, "gttmp");
    return Builder.CreateICmpSGT(l, r, "gttmp");
  case binop_leq:
    if (l_typ->get_type() == type_float)
      return Builder.CreateFCmpOLE(l, r, "letmp");
    return Builder.CreateICmpSLE(l, r, "letmp");
  case binop_geq:
    if (l_typ->get_type() == type_float)
      return Builder.CreateFCmpOGE(l, r, "getmp");
    return Builder.CreateICmpSGE(l, r, "getmp");
  case binop_assign:
    Builder.CreateStore(r, l);
    return cvoid();
  case binop_semicolon:
    return r;
  default:
    return cvoid();
  }
}

Value *If::compile() const
{
  Value *cond = expr1->compile();
  Function *TheFunction = Builder.GetInsertBlock()->getParent();
  BasicBlock *ThenBB = BasicBlock::Create(TheContext, "then", TheFunction);
  BasicBlock *ElseBB = BasicBlock::Create(TheContext, "else", TheFunction);
  BasicBlock *AfterBB = BasicBlock::Create(TheContext, "endif", TheFunction);
  Builder.CreateCondBr(cond, ThenBB, ElseBB);
  Builder.SetInsertPoint(ThenBB);
  Value *v2 = expr2->compile();
  ThenBB = Builder.GetInsertBlock();
  Builder.CreateBr(AfterBB);
  Builder.SetInsertPoint(ElseBB);
  Value *v3 = cvoid();
  if (expr3 != nullptr)
    v3 = expr3->compile();
  ElseBB = Builder.GetInsertBlock();
  Builder.CreateBr(AfterBB);
  Builder.SetInsertPoint(AfterBB);
  PHINode *phi = Builder.CreatePHI(typ->compile(), 2, "phi");
  phi->addIncoming(v2, ThenBB);
  phi->addIncoming(v3, ElseBB);
  return phi;
}

Value *New::compile() const
{
  llvm::Type *t = ty->compile();
  DataLayout dataLayout("");
  Value *size = c64(dataLayout.getTypeSizeInBits(t) / 8);
  Value *alloc = Builder.CreateCall(TheModule->getFunction("malloc"), {size});
  Value *ptr = Builder.CreateBitCast(alloc, PointerType::get(t, 0));
  return ptr;
}

Value *Pattern_Int_Expr::compile(Value *v) const
{
  return Builder.CreateICmpEQ(v, c64(num), "pat_cond");
}

Value *Pattern_Float_Expr::compile(Value *v) const
{
  return Builder.CreateFCmpOEQ(v, cfloat(num), "pat_cond");
}

Value *Pattern_Char_Expr::compile(Value *v) const
{
  return Builder.CreateICmpEQ(v, c8(ch), "pat_cond");
}

Value *Pattern_Bool_Expr::compile(Value *v) const
{
  return Builder.CreateICmpEQ(v, c1(boolean), "pat_cond");
}

Value *Pattern_id::compile(Value *v) const
{
  llvm::Type *t = v->getType();
  GlobalVariable *var = new GlobalVariable(*TheModule, t, false, GlobalValue::PrivateLinkage, ConstantAggregateZero::get(t), id);
  Builder.CreateStore(v, var);
  return c1(true);
}

Value *Pattern_Id::compile(Value *v) const
{
  std::string str = "";
  for (auto it = Id.rbegin(); it != Id.rend(); ++it)
  {
    if (*it == '_')
      break;
    str = *it + str;
  }
  int num = stoi(str);
  return Builder.CreateICmpEQ(Builder.CreateLoad(v), c64(num), "pat_cond");
}

Value *Pattern_Call::compile(Value *v) const
{
  std::string str = "";
  for (auto it = Id.rbegin(); it != Id.rend(); ++it)
  {
    if (*it == '_')
      break;
    str = *it + str;
  }
  int num = stoi(str);
  Value *cond = Builder.CreateICmpEQ(Builder.CreateLoad(v), c64(num), "pat_cond");
  Function *TheFunction = Builder.GetInsertBlock()->getParent();
  BasicBlock *ThenBB = BasicBlock::Create(TheContext, "then", TheFunction);
  BasicBlock *ElseBB = BasicBlock::Create(TheContext, "else", TheFunction);
  BasicBlock *AfterBB = BasicBlock::Create(TheContext, "endif", TheFunction);
  Builder.CreateCondBr(cond, ThenBB, ElseBB);
  Builder.SetInsertPoint(ThenBB);
  llvm::StructType *t = nullptr;
  for (auto &structType : TheModule->getIdentifiedStructTypes())
  {
    if (structType->getName() == Id)
    {
      t = structType;
      break;
    }
  }
  Value *alloc = Builder.CreateBitCast(v, PointerType::get(t, 0));
  int i = 1;
  for (Pattern *pat : *pattern_vec)
  {
    llvm::Value *MemberPointer = Builder.CreateStructGEP(t, alloc, i++);
    cond = Builder.CreateAnd(cond, pat->compile(Builder.CreateLoad(MemberPointer)));
  }
  ThenBB = Builder.GetInsertBlock();
  Builder.CreateBr(AfterBB);
  Builder.SetInsertPoint(ElseBB);
  Builder.CreateBr(AfterBB);
  Builder.SetInsertPoint(AfterBB);
  PHINode *phi = Builder.CreatePHI(i1, 2, "phi");
  phi->addIncoming(cond, ThenBB);
  phi->addIncoming(c1(false), ElseBB);
  return phi;
}

Value *Match::compile() const
{
  Value *v = expr->compile();
  std::vector<Value *> value_vec;
  std::vector<BasicBlock *> block_vec;
  Function *TheFunction = Builder.GetInsertBlock()->getParent();
  BasicBlock *ThenBB = nullptr;
  BasicBlock *ElseBB = nullptr;
  BasicBlock *AfterBB = BasicBlock::Create(TheContext, "endif", TheFunction);
  for (Clause *cl : *vec)
  {
    ThenBB = BasicBlock::Create(TheContext, "then", TheFunction);
    ElseBB = BasicBlock::Create(TheContext, "else", TheFunction);
    Value *cond = cl->pat->compile(v);
    Builder.CreateCondBr(cond, ThenBB, ElseBB);
    Builder.SetInsertPoint(ThenBB);
    value_vec.push_back(cl->expr->compile());
    ThenBB = Builder.GetInsertBlock();
    block_vec.push_back(ThenBB);
    Builder.CreateBr(AfterBB);
    Builder.SetInsertPoint(ElseBB);
  }
  std::string msg = "Runtime Error: No matching pattern found\n";
  Builder.CreateCall(TheModule->getFunction("print_string_4"), {Builder.CreateGlobalStringPtr(msg)});
  Builder.CreateCall(TheModule->getFunction("exit"), {c64(1)});
  Builder.CreateBr(ElseBB);
  Builder.SetInsertPoint(AfterBB);
  PHINode *phi = Builder.CreatePHI(typ->compile(), value_vec.size(), "phi");
  for (size_t i = 0; i < value_vec.size(); i++)
  {
    phi->addIncoming(value_vec[i], block_vec[i]);
  }
  return phi;
}

void Constr::compile() const
{
  std::string str = "";
  for (auto it = Id.rbegin(); it != Id.rend(); ++it)
  {
    if (*it == '_')
      break;
    str = *it + str;
  }
  int num = stoi(str);
  std::vector<llvm::Type *> from = {};
  std::vector<llvm::Type *> members = {i64};
  for (::Type *typ : *type_vec)
  {
    llvm::Type *t = typ->compile();
    from.push_back(t);
    members.push_back(t);
  }
  llvm::Type *t = StructType::create(TheContext, {members}, Id);
  // Constructor
  FunctionType *fn_type = FunctionType::get(PointerType::get(i64, 0), from, false);
  Function *func = Function::Create(fn_type, Function::ExternalLinkage, Id, TheModule.get());
  BasicBlock *PrevBB = Builder.GetInsertBlock();
  BasicBlock *BodyBB = BasicBlock::Create(TheContext, "body", func);
  Builder.SetInsertPoint(BodyBB);
  DataLayout dataLayout("");
  Value *size = c64(dataLayout.getTypeSizeInBits(t) / 8);
  Value *alloc = Builder.CreateCall(TheModule->getFunction("malloc"), {size});
  Value *ptr = Builder.CreateBitCast(alloc, PointerType::get(t, 0));
  Value *MemberPointer = Builder.CreateStructGEP(t, ptr, 0);
  Builder.CreateStore(c64(num), MemberPointer);
  int i = 1;
  for (Function::arg_iterator arg = func->arg_begin(); arg != func->arg_end(); arg++)
  {
    MemberPointer = Builder.CreateStructGEP(t, ptr, i++);
    Builder.CreateStore(arg, MemberPointer);
  }
  Builder.CreateRet(alloc);
  // Comparator
  fn_type = FunctionType::get(i1, {PointerType::get(i64, 0), PointerType::get(i64, 0)}, false);
  func = Function::Create(fn_type, Function::ExternalLinkage, Id + "_cmp", TheModule.get());
  BodyBB = BasicBlock::Create(TheContext, "body", func);
  Builder.SetInsertPoint(BodyBB);
  Function::arg_iterator arg = func->arg_begin();
  Value *l_ptr = Builder.CreateBitCast(arg++, PointerType::get(t, 0));
  Value *r_ptr = Builder.CreateBitCast(arg, PointerType::get(t, 0));
  Value *cond = c1(true);
  i = 1;
  for (::Type *typ : *type_vec)
  {
    Value *l = Builder.CreateLoad(Builder.CreateStructGEP(t, l_ptr, i));
    Value *r = Builder.CreateLoad(Builder.CreateStructGEP(t, r_ptr, i++));
    while (typ->get_type() == type_ref)
    {
      typ = typ->getChild1();
      l = Builder.CreateLoad(l);
      r = Builder.CreateLoad(r);
    }
    switch (typ->get_type())
    {
    case type_unit:
      continue;
    case type_float:
      cond = Builder.CreateAnd(cond, Builder.CreateFCmpOEQ(l, r));
      continue;
    case type_id:
      cond = Builder.CreateAnd(cond, Builder.CreateCall(TheModule->getFunction(typ->get_id() + "_cmp"), {l, r}));
      continue;
    default:
      cond = Builder.CreateAnd(cond, Builder.CreateICmpEQ(l, r));
      continue;
    }
  }
  Builder.CreateRet(cond);
  Builder.SetInsertPoint(PrevBB);
}

void TDef::compile() const
{
  FunctionType *fn_type = FunctionType::get(i1, {PointerType::get(i64, 0), PointerType::get(i64, 0)}, false);
  Function::Create(fn_type, Function::ExternalLinkage, id + "_cmp", TheModule.get());
}

void TDef::compile2() const
{
  Function *func = TheModule->getFunction(id + "_cmp");
  std::vector<Value *> value_vec;
  std::vector<BasicBlock *> block_vec;
  BasicBlock *PrevBB = Builder.GetInsertBlock();
  BasicBlock *HeadBB = BasicBlock::Create(TheContext, "head", func);
  BasicBlock *BodyBB = BasicBlock::Create(TheContext, "body", func);
  BasicBlock *ThenBB = nullptr;
  BasicBlock *ElseBB = nullptr;
  BasicBlock *AfterBB = BasicBlock::Create(TheContext, "endif", func);
  Builder.SetInsertPoint(BodyBB);
  Function::arg_iterator arg = func->arg_begin();
  Value *l_ptr = arg++;
  Value *r_ptr = arg;
  for (Constr *constr : *constr_vec)
  {
    constr->compile();
    std::string str = "";
    for (auto it = constr->Id.rbegin(); it != constr->Id.rend(); ++it)
    {
      if (*it == '_')
        break;
      str = *it + str;
    }
    int num = stoi(str);
    ThenBB = BasicBlock::Create(TheContext, "then", func);
    ElseBB = BasicBlock::Create(TheContext, "else", func);
    Value *cond = Builder.CreateICmpEQ(Builder.CreateLoad(l_ptr), c64(num));
    Builder.CreateCondBr(cond, ThenBB, ElseBB);
    Builder.SetInsertPoint(ThenBB);
    value_vec.push_back(Builder.CreateCall(TheModule->getFunction(constr->Id + "_cmp"), {l_ptr, r_ptr}));
    block_vec.push_back(ThenBB);
    Builder.CreateBr(AfterBB);
    Builder.SetInsertPoint(ElseBB);
  }
  value_vec.push_back(c1(false));
  block_vec.push_back(ElseBB);
  Builder.CreateBr(AfterBB);
  Builder.SetInsertPoint(HeadBB);
  Value *cond = Builder.CreateICmpEQ(Builder.CreateLoad(l_ptr), Builder.CreateLoad(r_ptr));
  Builder.CreateCondBr(cond, BodyBB, ElseBB);
  Builder.SetInsertPoint(AfterBB);
  PHINode *phi = Builder.CreatePHI(i1, value_vec.size());
  for (size_t i = 0; i < value_vec.size(); i++)
  {
    phi->addIncoming(value_vec[i], block_vec[i]);
  }
  Builder.CreateRet(phi);
  Builder.SetInsertPoint(PrevBB);
}

void TypeDef::compile() const
{
  for (TDef *tdef : *tdef_vec)
  {
    tdef->compile();
  }
  for (TDef *tdef : *tdef_vec)
  {
    tdef->compile2();
  }
}

void NormalDef::compile() const
{
  if (par_vec->size() == 0) // constant
  {
    llvm::Type *t = typ->compile();
    new GlobalVariable(*TheModule, t, false, GlobalValue::PrivateLinkage, ConstantAggregateZero::get(t), id);
  }
  else // function
  {
    std::vector<llvm::Type *> from = {};
    for (Par *par : *par_vec)
    {
      llvm::Type *t = par->typ->compile();
      from.push_back(t);
      new GlobalVariable(*TheModule, t, false, GlobalValue::PrivateLinkage, ConstantAggregateZero::get(t), par->id);
    }
    llvm::Type *to = typ->compile();
    FunctionType *fn_type = FunctionType::get(to, from, false);
    Function::Create(fn_type, Function::ExternalLinkage, id, TheModule.get());
  }
}

void NormalDef::compile2() const
{
  if (par_vec->size() == 0) // constant
  {
    Value *v = expr->compile();
    GlobalVariable *var = TheModule->getGlobalVariable(id, true);
    Builder.CreateStore(v, var);
  }
  else // function
  {
    std::vector<GlobalVariable *> global_vec;
    std::vector<llvm::Type *> members;
    auto start = std::prev(TheModule->global_end());
    Function *func = TheModule->getFunction(id);
    BasicBlock *PrevBB = Builder.GetInsertBlock();
    BasicBlock *HeadBB = BasicBlock::Create(TheContext, "head", func);
    BasicBlock *BodyBB = BasicBlock::Create(TheContext, "body", func);
    BasicBlock *TailBB = BasicBlock::Create(TheContext, "tail", func);
    Builder.SetInsertPoint(BodyBB);
    Function::arg_iterator arg = func->arg_begin();
    for (Par *par : *par_vec)
    {
      GlobalVariable *var = TheModule->getGlobalVariable(par->id, true);
      Builder.CreateStore(arg++, var);
      global_vec.push_back(var);
    }
    Value *v = expr->compile();
    Builder.CreateBr(TailBB);
    Builder.SetInsertPoint(HeadBB);
    for (auto global = ++start; global != TheModule->global_end(); global++)
    {
      if (!global->isConstant())
        global_vec.push_back(&*global);
    }
    for (GlobalVariable *global : global_vec)
    {
      members.push_back(global->getValueType());
    }
    llvm::Type *t = StructType::create(TheContext, {members}, id + "_bak");
    DataLayout dataLayout("");
    Value *size = c64(dataLayout.getTypeSizeInBits(t) / 8);
    Value *alloc = Builder.CreateCall(TheModule->getFunction("malloc"), {size});
    Value *ptr = Builder.CreateBitCast(alloc, PointerType::get(t, 0));
    int i = 0;
    for (GlobalVariable *global : global_vec)
    {
      Value *MemberPointer = Builder.CreateStructGEP(t, ptr, i++);
      Builder.CreateStore(Builder.CreateLoad(global), MemberPointer);
    }
    Builder.CreateBr(BodyBB);
    Builder.SetInsertPoint(TailBB);
    i = 0;
    for (GlobalVariable *global : global_vec)
    {
      Value *MemberPointer = Builder.CreateStructGEP(t, ptr, i++);
      Builder.CreateStore(Builder.CreateLoad(MemberPointer), global);
    }
    Builder.CreateCall(TheModule->getFunction("free"), {alloc});
    Builder.CreateRet(v);
    Builder.SetInsertPoint(PrevBB);
  }
}

void MutableDef::compile() const
{
  llvm::Type *t = typ->compile();
  llvm::Type *pt = PointerType::get(t, 0);
  std::vector<Value *> value_vec;
  DataLayout dataLayout("");
  Value *size = c64(dataLayout.getTypeSizeInBits(t) / 8);
  if (expr_vec != nullptr)
  {
    for (Expr *e : *expr_vec)
    {
      Value *v = e->compile();
      value_vec.push_back(v);
      size = Builder.CreateMul(size, v);
    }
    size = Builder.CreateAdd(size, c64(expr_vec->size()));
  }
  GlobalVariable *var = new GlobalVariable(*TheModule, pt, false, GlobalValue::PrivateLinkage, ConstantAggregateZero::get(pt), id);
  Value *alloc = Builder.CreateCall(TheModule->getFunction("malloc"), {size});
  if (expr_vec != nullptr)
  {
    alloc = Builder.CreateGEP(alloc, {c64(expr_vec->size())});
    int i = 0;
    for (Value *v : value_vec)
      Builder.CreateStore(v, Builder.CreateGEP(alloc, {c64(--i)}));
  }
  Value *ptr = Builder.CreateBitCast(alloc, pt);
  Builder.CreateStore(ptr, var);
}

void LetDef::compile() const
{
  for (Def *def : *def_vec)
  {
    def->compile();
  }
  for (Def *def : *def_vec)
  {
    def->compile2();
  }
}

Value *LetIn::compile() const
{
  letdef->compile();
  return expr->compile();
}
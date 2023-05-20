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
  i1 = IntegerType::get(TheContext, 1);
  i8 = IntegerType::get(TheContext, 8);
  i32 = IntegerType::get(TheContext, 32);
  i64 = IntegerType::get(TheContext, 64);
  flo = llvm::Type::getFloatTy(TheContext);
  voi = llvm::Type::getVoidTy(TheContext);

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
  FunctionType *read_int_type = FunctionType::get(i64, {}, false);
  Function::Create(read_int_type, Function::ExternalLinkage, str_read_int, TheModule.get());
  FunctionType *read_bool_type = FunctionType::get(i1, {}, false);
  Function::Create(read_bool_type, Function::ExternalLinkage, str_read_bool, TheModule.get());
  FunctionType *read_char_type = FunctionType::get(i8, {}, false);
  Function::Create(read_char_type, Function::ExternalLinkage, str_read_char, TheModule.get());
  FunctionType *read_float_type = FunctionType::get(flo, {}, false);
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
  FunctionType *pi_type = FunctionType::get(flo, {}, false);
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

llvm::Type *Type_Ref::compile() const
{
  return PointerType::get(typ->compile(), 0);
}

Value *LetDef::compile() const
{
  for (Def *def : *def_vec)
  {
    def->compile();
  }
  for (Def *def : *def_vec)
  {
    def->compile2();
  }
  return nullptr;
}

Value *NormalDef::compile() const
{
  if (par_vec->size() == 0) // constant
  {
    if (typ->get_type() != type_unit)
    {
      llvm::Type *t = typ->compile();
      new GlobalVariable(*TheModule, t, false, GlobalValue::PrivateLinkage, ConstantAggregateZero::get(t), id);
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
        new GlobalVariable(*TheModule, t, false, GlobalValue::PrivateLinkage, ConstantAggregateZero::get(t), par->id);
        break;
      }
    }
    llvm::Type *to = typ->compile();
    FunctionType *fn_type = FunctionType::get(to, from, false);
    Function::Create(fn_type, Function::ExternalLinkage, id, TheModule.get());
    // Function::arg_iterator arg = func->arg_begin();
    // for (Par *par : *par_vec)
    // {
    //   switch (par->typ->get_type())
    //   {
    //   case type_unit:
    //     break;
    //   default:
    //     arg->setName(par->id);
    //     std::advance(arg, 1);
    //     break;
    //   }
    // }
  }
  return nullptr;
}

Value *NormalDef::compile2() const
{
  if (par_vec->size() == 0) // constant
  {
    Value *v = expr->compile();
    if (typ->get_type() != type_unit)
    {
      GlobalVariable *var = TheModule->getGlobalVariable(id, true);
      Builder.CreateStore(v, var);
    }
  }
  else // function
  {
    Function *func = TheModule->getFunction(id);
    BasicBlock *PrevBB = Builder.GetInsertBlock();
    BasicBlock *BodyBB = BasicBlock::Create(TheContext, id, func);
    Builder.SetInsertPoint(BodyBB);
    Function::arg_iterator arg = func->arg_begin();
    for (Par *par : *par_vec)
    {
      switch (par->typ->get_type())
      {
      case type_unit:
        break;
      default:
        GlobalVariable *var = TheModule->getGlobalVariable(par->id, true);
        Builder.CreateStore(arg, var);
        std::advance(arg, 1);
        break;
      }
    }
    Builder.CreateRet(expr->compile());
    Builder.SetInsertPoint(PrevBB);
  }
  return nullptr;
}

Value *MutableDef::compile() const
{
  if (typ->get_type() != type_unit)
  {
    Value *size = c64(1);
    int i = 1;
    if (expr_vec != nullptr)
      for (Expr *e : *expr_vec)
      {
        Value *v = e->compile();
        std::string name = "dim_" + std::to_string(i++) + "_" + id;
        GlobalVariable *var = new GlobalVariable(*TheModule, i64, false, GlobalValue::PrivateLinkage, ConstantAggregateZero::get(i64), name);
        Builder.CreateStore(v, var);
        size = Builder.CreateMul(size, v);
      }
    llvm::Type *t = typ->compile();
    llvm::Type *pt = PointerType::get(t, 0);
    GlobalVariable *var = new GlobalVariable(*TheModule, pt, false, GlobalValue::PrivateLinkage, ConstantAggregateZero::get(pt), id);
    AllocaInst *alloc = Builder.CreateAlloca(t, size, "alloc");
    Builder.CreateStore(alloc, var);
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
  return c1(boolean);
}

Value *Array::compile() const
{
  if (typ->getChild1()->get_type() != type_unit)
  {
    Value *offset = c64(0);
    Value *coeff = c64(1);
    int i = 1;
    if (expr_vec != nullptr)
      for (Expr *e : *expr_vec)
      {
        Value *v = e->compile();
        offset = Builder.CreateAdd(offset, Builder.CreateMul(v, coeff));
        std::string name = "dim_" + std::to_string(i++) + "_" + id;
        GlobalVariable *var = TheModule->getGlobalVariable(name, true);
        coeff = Builder.CreateMul(coeff, Builder.CreateLoad(var));
      }
    GlobalVariable *var = TheModule->getGlobalVariable(id, true);
    Value *ptr = Builder.CreateLoad(var, "loadtmp");
    return Builder.CreateGEP(ptr, {offset}, id + "_ptr");
  }
  return nullptr;
}

Value *id_Expr::compile() const
{
  // constant or variable
  GlobalVariable *var = TheModule->getGlobalVariable(id, true);
  if (var != nullptr)
    return Builder.CreateLoad(var, "loadtmp");
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

Value *For::compile() const
{
  GlobalVariable *var = new GlobalVariable(*TheModule, i64, false, GlobalValue::PrivateLinkage, ConstantAggregateZero::get(i64), id);
  Builder.CreateStore(start->compile(), var);
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
    loop_cond = Builder.CreateICmpSGE(iter, end->compile(), "loop_cond");
  else
    loop_cond = Builder.CreateICmpSLE(iter, end->compile(), "loop_cond");
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
  return nullptr;
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
    return Builder.CreateLoad(v, "loadtmp");
  case unop_not:
    return Builder.CreateNot(v, "nottmp");
  case unop_delete:
    return nullptr;
  default:
    return nullptr;
  }
}

Value *BinOp::compile() const
{
  Value *l = left->compile();
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
    phi->addIncoming(l, ElseBB);
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
    phi->addIncoming(l, ThenBB);
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
    if (l->getType()->isFloatingPointTy())
      return Builder.CreateFCmpOEQ(l, r, "eqtmp");
    return Builder.CreateICmpEQ(l, r, "eqtmp");
  case binop_struct_ne:
    if (l->getType()->isFloatingPointTy())
      return Builder.CreateFCmpONE(l, r, "eqtmp");
    return Builder.CreateICmpNE(l, r, "eqtmp");
  case binop_phys_eq:
    return Builder.CreateICmpEQ(Builder.CreateBitCast(l, i64), Builder.CreateBitCast(r, i64), "eqtmp");
  case binop_phys_ne:
    return Builder.CreateICmpNE(Builder.CreateBitCast(l, i64), Builder.CreateBitCast(r, i64), "eqtmp");
  case binop_l:
    if (l->getType()->isFloatingPointTy())
      return Builder.CreateFCmpOLT(l, r, "eqtmp");
    return Builder.CreateICmpSLT(l, r, "eqtmp");
  case binop_g:
    if (l->getType()->isFloatingPointTy())
      return Builder.CreateFCmpOGT(l, r, "eqtmp");
    return Builder.CreateICmpSGT(l, r, "eqtmp");
  case binop_leq:
    if (l->getType()->isFloatingPointTy())
      return Builder.CreateFCmpOLE(l, r, "eqtmp");
    return Builder.CreateICmpSLE(l, r, "eqtmp");
  case binop_geq:
    if (l->getType()->isFloatingPointTy())
      return Builder.CreateFCmpOGE(l, r, "eqtmp");
    return Builder.CreateICmpSGE(l, r, "eqtmp");
  case binop_assign:
    Builder.CreateStore(r, l);
    return nullptr;
  case binop_semicolon:
    return r;
  default:
    return nullptr;
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
  Value *v3 = nullptr;
  if (expr3 != nullptr)
    v3 = expr3->compile();
  ElseBB = Builder.GetInsertBlock();
  Builder.CreateBr(AfterBB);
  Builder.SetInsertPoint(AfterBB);
  if (typ->get_type() == type_unit)
    return nullptr;
  PHINode *phi = Builder.CreatePHI(typ->compile(), 2, "phi");
  phi->addIncoming(v2, ThenBB);
  phi->addIncoming(v3, ElseBB);
  return phi;
}

Value *LetIn::compile() const
{
  def->compile();
  return expr->compile();
}
#include "ast.hpp"

inline void semerror(std::string msg)
{
  std::cerr << msg << std::endl;
  exit(1);
}

extern SymbolTable st;
extern TypeDefTable tt;

std::string AST::str_print_int;
std::string AST::str_print_bool;
std::string AST::str_print_char;
std::string AST::str_print_float;
std::string AST::str_print_string;
std::string AST::str_read_int;
std::string AST::str_read_bool;
std::string AST::str_read_char;
std::string AST::str_read_float;
std::string AST::str_read_string;
std::string AST::str_abs;
std::string AST::str_fabs;
std::string AST::str_sqrt;
std::string AST::str_sin;
std::string AST::str_cos;
std::string AST::str_tan;
std::string AST::str_atan;
std::string AST::str_exp;
std::string AST::str_ln;
std::string AST::str_pi;
std::string AST::str_incr;
std::string AST::str_decr;
std::string AST::str_float_of_int;
std::string AST::str_int_of_float;
std::string AST::str_round;
std::string AST::str_int_of_char;
std::string AST::str_char_of_int;
std::string AST::str_strlen;
std::string AST::str_strcmp;
std::string AST::str_strcpy;
std::string AST::str_strcat;

void Program::sem()
{
  str_print_int = "print_int";
  str_print_bool = "print_bool";
  str_print_char = "print_char";
  str_print_float = "print_float";
  str_print_string = "print_string";
  str_read_int = "read_int";
  str_read_bool = "read_bool";
  str_read_char = "read_char";
  str_read_float = "read_float";
  str_read_string = "read_string";
  str_abs = "abs";
  str_fabs = "fabs";
  str_sqrt = "sqrt";
  str_sin = "sin";
  str_cos = "cos";
  str_tan = "tan";
  str_atan = "atan";
  str_exp = "exp";
  str_ln = "ln";
  str_pi = "pi";
  str_incr = "incr";
  str_decr = "decr";
  str_float_of_int = "float_of_int";
  str_int_of_float = "int_of_float";
  str_round = "round";
  str_int_of_char = "int_of_char";
  str_char_of_int = "char_of_int";
  str_strlen = "strlen";
  str_strcmp = "strcmp";
  str_strcpy = "strcpy";
  str_strcat = "strcat";
  st.openScope();
  st.insert(str_print_int, new Type_Func(new Type_Int(), new Type_Unit()));
  st.insert(str_print_bool, new Type_Func(new Type_Bool(), new Type_Unit()));
  st.insert(str_print_char, new Type_Func(new Type_Char(), new Type_Unit()));
  st.insert(str_print_float, new Type_Func(new Type_Float(), new Type_Unit()));
  st.insert(str_print_string, new Type_Func(new Type_Array(1, new Type_Char()), new Type_Unit()));
  st.insert(str_read_int, new Type_Func(new Type_Unit(), new Type_Int()));
  st.insert(str_read_bool, new Type_Func(new Type_Unit(), new Type_Bool()));
  st.insert(str_read_char, new Type_Func(new Type_Unit(), new Type_Char()));
  st.insert(str_read_float, new Type_Func(new Type_Unit(), new Type_Float()));
  st.insert(str_read_string, new Type_Func(new Type_Unit(), new Type_Array(1, new Type_Char())));
  st.insert(str_abs, new Type_Func(new Type_Int(), new Type_Int()));
  st.insert(str_fabs, new Type_Func(new Type_Float(), new Type_Float()));
  st.insert(str_sqrt, new Type_Func(new Type_Float(), new Type_Float()));
  st.insert(str_sin, new Type_Func(new Type_Float(), new Type_Float()));
  st.insert(str_cos, new Type_Func(new Type_Float(), new Type_Float()));
  st.insert(str_tan, new Type_Func(new Type_Float(), new Type_Float()));
  st.insert(str_atan, new Type_Func(new Type_Float(), new Type_Float()));
  st.insert(str_exp, new Type_Func(new Type_Float(), new Type_Float()));
  st.insert(str_ln, new Type_Func(new Type_Float(), new Type_Float()));
  st.insert(str_pi, new Type_Func(new Type_Unit(), new Type_Float()));
  st.insert(str_incr, new Type_Func(new Type_Ref(new Type_Int), new Type_Unit()));
  st.insert(str_decr, new Type_Func(new Type_Ref(new Type_Int), new Type_Unit()));
  st.insert(str_float_of_int, new Type_Func(new Type_Int(), new Type_Float()));
  st.insert(str_int_of_float, new Type_Func(new Type_Float(), new Type_Int()));
  st.insert(str_round, new Type_Func(new Type_Float(), new Type_Int()));
  st.insert(str_int_of_char, new Type_Func(new Type_Char(), new Type_Int()));
  st.insert(str_char_of_int, new Type_Func(new Type_Int(), new Type_Char()));
  st.insert(str_strlen, new Type_Func(new Type_Array(1, new Type_Char()), new Type_Int()));
  st.insert(str_strcmp, new Type_Func(new Type_Array(1, new Type_Char()), new Type_Func(new Type_Array(1, new Type_Char()), new Type_Int())));
  st.insert(str_strcpy, new Type_Func(new Type_Array(1, new Type_Char()), new Type_Func(new Type_Array(1, new Type_Char()), new Type_Unit())));
  st.insert(str_strcat, new Type_Func(new Type_Array(1, new Type_Char()), new Type_Func(new Type_Array(1, new Type_Char()), new Type_Unit())));

  for (Stmt *stmt : *statements)
  {
    stmt->sem();
  }
}

void LetDef::sem()
{
  if (rec)
  {
    for (Def *def : *def_vec)
    {
      def->sem();
    }
    for (Def *def : *def_vec)
    {
      def->sem2();
    }
  }
  else
  {
    for (Def *def : *def_vec)
    {
      def->sem2();
    }
    for (Def *def : *def_vec)
    {
      def->sem();
    }
  }
}

void NormalDef::sem()
{
  typ->sem();
  if (typ->get_type() == type_func)
  {
    semerror("Return type function not allowed");
  }
  ::Type *tmp = typ;
  for (auto i = par_vec->rbegin(); i != par_vec->rend(); i++)
  {
    tmp = new Type_Func((*i)->typ, tmp);
  }
  st.insert(id, tmp);
}

void NormalDef::sem2()
{
  st.openScope();
  for (Par *par : *par_vec)
  {
    par->sem();
  }
  expr->sem();
  expr->type_check(typ);
  st.closeScope();
}

void MutableDef::sem()
{
  typ->sem();
  if (expr_vec == nullptr)
  {
    st.insert(id, new Type_Ref(typ));
  }
  else
  {
    for (Expr *e : *expr_vec)
    {
      e->sem();
      e->type_check(new Type_Int());
    }
    st.insert(id, new Type_Array(expr_vec->size(), typ));
  }
}

void TypeDef::sem()
{
  if (tdef_vec == nullptr)
  {
    return;
  }
  for (TDef *tdef : *tdef_vec)
  {
    tdef->sem();
  }
  for (TDef *tdef : *tdef_vec)
  {
    tdef->sem2();
  }
}

void TDef::sem() { tt.insert(id); }

void TDef::sem2()
{
  for (Constr *c : *constr_vec)
  {
    c->id = id;
    c->sem();
  }
}

void Constr::sem()
{
  ::Type *tmp = new Type_id(id);
  for (auto i = type_vec->rbegin(); i != type_vec->rend(); i++)
  {
    tmp = new Type_Func(*i, tmp);
  }
  st.insert(Id, tmp);
}

void Par::sem()
{
  typ->sem();
  st.insert(id, typ);
}

::Type *::Type::getChild1()
{
  return nullptr;
}

::Type *::Type::getChild2()
{
  return nullptr;
}

int ::Type::getDim()
{
  return 0;
}

std::string (::Type::get_id)()
{
  return "";
}

bool ::Type::equals(::Type *other)
{
  if (other == nullptr)
  {
    return false;
  }
  if (other->get_type() == type_undefined)
  {
    return other->equals(this);
  }
  return this->get_type() == other->get_type();
}

main_type Type_Unit::get_type() { return type_unit; }

main_type Type_Int::get_type() { return type_int; }

main_type Type_Float::get_type() { return type_float; }

main_type Type_Char::get_type() { return type_char; }

main_type Type_Bool::get_type() { return type_bool; }

main_type Type_Func::get_type() { return type_func; }

::Type *Type_Func::getChild1() { return from; }

::Type *Type_Func::getChild2() { return to; }

bool Type_Func::equals(::Type *other)
{
  if (other == nullptr)
  {
    return false;
  }
  if (other->get_type() == type_undefined)
  {
    return other->equals(this);
  }
  if (this->get_type() != other->get_type())
  {
    return false;
  }
  return from->equals(other->getChild1()) && to->equals(other->getChild2());
}

void Type_Func::sem()
{
  from->sem();
  to->sem();
}

main_type Type_Ref::get_type() { return type_ref; }

::Type *Type_Ref::getChild1() { return typ; }

bool Type_Ref::equals(::Type *other)
{
  if (other == nullptr)
  {
    return false;
  }
  if (other->get_type() == type_undefined)
  {
    return other->equals(this);
  }
  if (this->get_type() != other->get_type())
  {
    return false;
  }
  return typ->equals(other->getChild1());
}

void Type_Ref::sem() { typ->sem(); }

main_type Type_Array::get_type() { return type_array; }

::Type *Type_Array::getChild1() { return typ; }

int Type_Array::getDim() { return dim; }

bool Type_Array::equals(::Type *other)
{
  if (other == nullptr)
  {
    return false;
  }
  if (other->get_type() == type_undefined)
  {
    return other->equals(this);
  }
  if (this->get_type() != other->get_type())
  {
    return false;
  }
  return typ->equals(other->getChild1()) && dim == other->getDim();
}
void Type_Array::sem() { typ->sem(); }

main_type Type_id::get_type() { return type_id; }

std::string Type_id::get_id() { return id; }

bool Type_id::equals(::Type *other)
{
  if (other == nullptr)
  {
    return false;
  }
  if (other->get_type() == type_undefined)
  {
    return other->equals(this);
  }
  if (this->get_type() != other->get_type())
  {
    return false;
  }
  return id == other->get_id();
}

void Type_id::sem() { tt.lookup(id); }

main_type Type_Undefined::get_type()
{
  return typ == nullptr ? type_undefined : typ->get_type();
}

::Type *Type_Undefined::getChild1()
{
  return typ == nullptr ? this : typ->getChild1();
}

::Type *Type_Undefined::getChild2()
{
  return typ == nullptr ? nullptr : typ->getChild2();
}

int Type_Undefined::getDim()
{
  return typ == nullptr ? 0 : typ->getDim();
}

std::string Type_Undefined::get_id()
{
  return typ == nullptr ? "" : typ->get_id();
}

bool Type_Undefined::equals(::Type *other)
{
  if (other->get_type() == type_undefined)
  {
    other = other->getChild1();
  }
  if (this == other)
  {
    return 1;
  }
  if (typ == nullptr)
  {
    typ = other;
    return 1;
  }
  else
  {
    return typ->equals(other);
  }
}

void Expr::type_check(::Type *t)
{
  if (!t->equals(this->typ))
  {
    std::cerr << *this;
    semerror("Type mismatch");
  }
};

void Int_Expr::sem() { typ = new Type_Int(); }

void Float_Expr::sem() { typ = new Type_Float(); }

void Char_Expr::sem() { typ = new Type_Char(); }

void Str_Expr::sem()
{
  typ = new Type_Array(1, new Type_Char());
}

void Bool_Expr::sem() { typ = new Type_Bool(); }

void Unit_Expr::sem() { typ = new Type_Unit(); }

void UnOp::sem()
{
  expr->sem();
  switch (op)
  {
  case unop_plus:
  case unop_minus:
    expr->type_check(new Type_Int());
    typ = new Type_Int();
    break;
  case unop_float_plus:
  case unop_float_minus:
    expr->type_check(new Type_Float());
    typ = new Type_Float();
    break;
  case unop_exclamation:
    if (expr->typ->get_type() == type_undefined)
    {
      expr->type_check(new Type_Ref(new Type_Undefined()));
    }
    else if (expr->typ->get_type() != type_ref)
    {
      semerror("exclamation: Type mismatch");
    }
    typ = expr->typ->getChild1();
    break;
  case unop_not:
    expr->type_check(new Type_Bool());
    typ = new Type_Bool();
    break;
  case unop_delete:
    if (expr->typ->get_type() == type_undefined)
    {
      expr->type_check(new Type_Ref(new Type_Undefined()));
    }
    else if (expr->typ->get_type() != type_ref)
    {
      semerror("delete: Type mismatch");
    }
    typ = new Type_Unit();
    break;
  }
}

void BinOp::sem()
{
  left->sem();
  right->sem();
  switch (op)
  {
  case binop_plus:
  case binop_minus:
  case binop_mult:
  case binop_div:
  case binop_mod:
    left->type_check(new Type_Int);
    right->type_check(new Type_Int);
    typ = new Type_Int();
    break;
  case binop_float_plus:
  case binop_float_minus:
  case binop_float_mult:
  case binop_float_div:
  case binop_pow:
    left->type_check(new Type_Float);
    right->type_check(new Type_Float);
    typ = new Type_Float();
    break;
  case binop_struct_eq:
  case binop_struct_ne:
  case binop_phys_eq:
  case binop_phys_ne:
    left->type_check(right->typ);
    switch (left->typ->get_type())
    {
    case type_array:
    case type_func:
      semerror("Type not allowed");
      break;
    default:
      break;
    }
    typ = new Type_Bool();
    break;
  case binop_l:
  case binop_g:
  case binop_leq:
  case binop_geq:
    left->type_check(right->typ);
    switch (left->typ->get_type())
    {
    case type_int:
    case type_float:
    case type_char:
    case type_undefined:
      break;
    default:
      semerror("Type not allowed");
      break;
    }
    typ = new Type_Bool();
    break;
  case binop_and:
  case binop_or:
    left->type_check(new Type_Bool);
    right->type_check(new Type_Bool);
    typ = new Type_Bool();
    break;
  case binop_assign:
    left->type_check(new Type_Ref(right->typ));
    typ = new Type_Unit();
    break;
  case binop_semicolon:
    typ = right->typ;
    break;
  }
}

void id_Expr::sem()
{
  typ = st.lookup(id)->type;
}

void Id_Expr::sem() { typ = st.lookup(Id)->type; }

void call::sem()
{
  ::Type *tmp = st.lookup(id)->type;
  for (Expr *e : *expr_vec)
  {
    e->sem();
    if (tmp->get_type() == type_undefined)
    {
      tmp->equals(new Type_Func(new Type_Undefined(), new Type_Undefined()));
    }
    if (tmp->get_type() != type_func)
    {
      semerror("Parameter number mismatch");
    }
    e->type_check(tmp->getChild1());
    tmp = tmp->getChild2();
  }
  if (tmp->get_type() == type_func)
  {
    semerror("Parameter number mismatch");
  }
  typ = tmp;
}

void Array::sem()
{
  SymbolEntry *se = st.lookup(id);
  typ = se->type;
  if (!typ->equals(new Type_Array(expr_vec->size(), new Type_Undefined())))
  {
    semerror("Array: Type mismatch");
  }
  for (Expr *e : *expr_vec)
  {
    e->sem();
    e->type_check(new Type_Int());
  }
  typ = new Type_Ref(typ->getChild1());
}

void Dim::sem()
{
  SymbolEntry *se = st.lookup(id);
  if (se->type->get_type() != type_undefined)
  {
    if (se->type->get_type() != type_array)
    {
      semerror("Dim: Type mismatch");
    }
    else if (ind < 1 || ind > se->type->getDim())
    {
      semerror("Array dimensions mismatch");
    }
  }
  typ = new Type_Int();
}

void New::sem()
{
  if (ty->get_type() == type_array)
  {
    semerror("Reference cannot be of Type array");
  }
  ty->sem();
  typ = new Type_Ref(ty);
}

void LetIn::sem()
{
  st.openScope();
  letdef->sem();
  expr->sem();
  typ = expr->typ;
  st.closeScope();
}

void If::sem()
{
  expr1->sem();
  expr1->type_check(new Type_Bool());
  expr2->sem();
  if (expr3 == nullptr)
  {
    expr2->type_check(new Type_Unit());
  }
  else
  {
    expr3->sem();
    expr2->type_check(expr3->typ);
  }
  typ = expr2->typ;
}

void While::sem()
{
  cond->sem();
  cond->type_check(new Type_Bool());
  stmt->sem();
  typ = new Type_Unit();
}

void For::sem()
{
  start->sem();
  start->type_check(new Type_Int());
  end->sem();
  end->type_check(new Type_Int());
  st.openScope();
  st.insert(id, new Type_Int());
  stmt->sem();
  st.closeScope();
  typ = new Type_Unit();
}

void Match::sem()
{
  expr->sem();
  typ = new Type_Undefined();
  for (Clause *cl : *cl_vec)
  {
    st.openScope();
    cl->sem();
    expr->type_check(cl->pat->typ);
    cl->expr->type_check(typ);
    st.closeScope();
  }
  typ = ((*cl_vec)[0])->expr->typ;
}

void Clause::sem()
{
  pat->sem();
  expr->sem();
}

void Pattern_Int_Expr::sem()
{
  typ = new Type_Int();
}

void Pattern_Float_Expr::sem()
{
  typ = new Type_Float();
}

void Pattern_Char_Expr::sem()
{
  typ = new Type_Char();
}

void Pattern_Bool_Expr::sem()
{
  typ = new Type_Bool();
}

void Pattern_id::sem()
{
  typ = new Type_Undefined();
  st.insert(id, typ);
}

void Pattern_Id::sem()
{
  typ = st.lookup(Id)->type;
}

void Pattern_Call::sem()
{
  ::Type *tmp = st.lookup(Id)->type;
  for (Pattern *p : *pattern_vec)
  {
    p->sem();
    if (tmp->get_type() != type_func)
    {
      semerror("Parameter number mismatch");
    }
    else
    {
      if (!p->typ->equals(tmp->getChild1()))
      {
        semerror("Type mismatch");
      }
      tmp = tmp->getChild2();
    }
  }
  if (tmp->get_type() == type_func)
  {
    semerror("Parameter number mismatch");
  }
  typ = tmp;
}

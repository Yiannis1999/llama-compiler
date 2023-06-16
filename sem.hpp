#include "ast.hpp"

inline void semanticError(std::string msg)
{
  std::cerr << msg << std::endl;
  exit(1);
}

extern SymbolTable st;
extern TypeDefTable tt;
// extern ConstrTable ct;

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

// class Program

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

// class Type_Unit

main_type Type_Unit::get_type() { return type_unit; }

// class Type_Int

main_type Type_Int::get_type() { return type_int; }

// class Type_Float

main_type Type_Float::get_type() { return type_float; }

// class Type_Char

main_type Type_Char::get_type() { return type_char; }

// class Type_Bool

main_type Type_Bool::get_type() { return type_bool; }

// class Type_Func

main_type Type_Func::get_type() { return type_func; }

::Type *Type_Func::getChild1() { return from; }

::Type *Type_Func::getChild2() { return to; }

bool Type_Func::equals(::Type *other)
{
  if (other == nullptr)
    return false;
  if (other->get_type() == type_undefined)
    return other->equals(this);
  if (this->get_type() != other->get_type())
    return false;
  return from->equals(other->getChild1()) && to->equals(other->getChild2());
}

void Type_Func::sem()
{
  from->sem();
  to->sem();
}

// class Type_Ref

main_type Type_Ref::get_type() { return type_ref; }

::Type *Type_Ref::getChild1() { return typ; }

bool Type_Ref::equals(::Type *other)
{
  if (other == nullptr)
    return false;
  if (other->get_type() == type_undefined)
    return other->equals(this);
  if (this->get_type() != other->get_type())
    return false;
  return typ->equals(other->getChild1());
}

void Type_Ref::sem() { typ->sem(); }

// class Type_Array

main_type Type_Array::get_type() { return type_array; }

::Type *Type_Array::getChild1() { return typ; }

int Type_Array::getDim() { return dim; }

bool Type_Array::equals(::Type *other)
{
  if (other == nullptr)
    return false;
  if (other->get_type() == type_undefined)
    return other->equals(this);
  if (this->get_type() != other->get_type())
    return false;
  return typ->equals(other->getChild1()) && dim == other->getDim();
}
void Type_Array::sem() { typ->sem(); }

// class Type_id

main_type Type_id::get_type() { return type_id; }

std::string Type_id::get_id() { return id; }

bool Type_id::equals(::Type *other)
{
  if (other == nullptr)
    return false;
  if (other->get_type() == type_undefined)
    return other->equals(this);
  if (this->get_type() != other->get_type())
    return false;
  return id == other->get_id();
}

void Type_id::sem() { tt.lookup(id); }

// class Type_Undefined

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
  // std::cerr << *this << " " << *other << std::endl;
  if (other->get_type() == type_undefined)
    other = other->getChild1();
  if (this == other)
    return 1;
  if (typ == nullptr)
  {
    typ = other;
    // std::cerr << *this << std::endl;
    return 1;
  }
  else
    return typ->equals(other);
}

// class Expr

void Expr::type_check(::Type *t)
{
  if (!t->equals(this->typ))
  {
    std::cerr << *this;
    semanticError("Type mismatch");
  }
};

// class Int_Expr

void Int_Expr::sem() { typ = new Type_Int(); }

// class Float_Expr

void Float_Expr::sem() { typ = new Type_Float(); }

// class Char_Expr

void Char_Expr::sem() { typ = new Type_Char(); }

// class Str_Expr

void Str_Expr::sem()
{
  typ = new Type_Array(1, new Type_Char());
}

// class Bool_Expr

void Bool_Expr::sem() { typ = new Type_Bool(); }

// class Unit_Expr

void Unit_Expr::sem() { typ = new Type_Unit(); }

// class Array

void Array::sem()
{
  SymbolEntry *se = st.lookup(id);
  typ = se->type;
  if (!typ->equals(new Type_Array(expr_vec->size(), new Type_Undefined())))
    semanticError("Array: Type mismatch");
  for (Expr *e : *expr_vec)
  {
    e->sem();
    e->type_check(new Type_Int());
  }
  typ = new Type_Ref(typ->getChild1());
}

// class Dim

void Dim::sem()
{
  SymbolEntry *se = st.lookup(id);
  if (se->type->get_type() != type_undefined)
  {
    if (se->type->get_type() != type_array)
      semanticError("Dim: Type mismatch");
    else if (ind < 1 || ind > se->type->getDim())
      semanticError("Array dimensions mismatch");
  }
  typ = new Type_Int();
}

// class id

void id_Expr::sem()
{
  typ = st.lookup(id)->type;
  if(typ->get_type() == type_func)
    semanticError("Parameter number mismatch");
}

// class Id

void Id_Expr::sem() { typ = st.lookup(Id)->type; }

// class While

void While::sem()
{
  cond->sem();
  cond->type_check(new Type_Bool());
  stmt->sem();
  typ = new Type_Unit();
}

// class For

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

// class call

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
      semanticError("Parameter number mismatch");
    }
    else
    {
      e->type_check(tmp->getChild1());
      tmp = tmp->getChild2();
    }
  }
  if (tmp->get_type() == type_func)
  {
    semanticError("Parameter number mismatch");
  }
  typ = tmp;
}

// class UnOp

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
      expr->type_check(new Type_Ref(new Type_Undefined()));
    else if (expr->typ->get_type() != type_ref)
      semanticError("exclamation: Type mismatch");
    typ = expr->typ->getChild1();
    break;
  case unop_not:
    expr->type_check(new Type_Bool());
    typ = new Type_Bool();
    break;
  case unop_delete:
    if (expr->typ->get_type() == type_undefined)
      expr->type_check(new Type_Ref(new Type_Undefined()));
    else if (expr->typ->get_type() != type_ref)
      semanticError("delete: Type mismatch");
    typ = new Type_Unit();
    break;
  }
}

// class BinOp

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
      semanticError("Type not allowed");
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
      semanticError("Type not allowed");
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

// class If

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

// class New

void New::sem()
{
  if (ty->get_type() == type_array)
    semanticError("Reference cannot be of Type array");
  ty->sem();
  typ = new Type_Ref(ty);
}

// class Pattern_Int_Expr

void Pattern_Int_Expr::sem()
{
  typ = new Type_Int();
}

// class Pattern_Float_Expr

void Pattern_Float_Expr::sem()
{
  typ = new Type_Float();
}

// class Pattern_Char_Expr

void Pattern_Char_Expr::sem()
{
  typ = new Type_Char();
}

// class Pattern_Bool_Expr

void Pattern_Bool_Expr::sem()
{
  typ = new Type_Bool();
}

// class Pattern_id

void Pattern_id::sem()
{
  typ = new Type_Undefined();
  st.insert(id, typ);
}

// class Pattern_Id

void Pattern_Id::sem()
{
  typ = st.lookup(Id)->type;
}

// class Pattern_Call

void Pattern_Call::sem()
{
  ::Type *tmp = st.lookup(Id)->type;
  for (Pattern *p : *pattern_vec)
  {
    p->sem();
    if (tmp->get_type() != type_func)
    {
      semanticError("Parameter number mismatch");
    }
    else
    {
      if (!p->typ->equals(tmp->getChild1()))
        semanticError("Type mismatch");
      tmp = tmp->getChild2();
    }
  }
  if (tmp->get_type() == type_func)
  {
    semanticError("Parameter number mismatch");
  }
  typ = tmp;
}

// class Clause

void Clause::sem()
{
  pat->sem();
  expr->sem();
}

// class Match

void Match::sem()
{
  expr->sem();
  typ = new Type_Undefined();
  for (Clause *cl : *vec)
  {
    st.openScope();
    cl->sem();
    expr->type_check(cl->pat->typ);
    cl->expr->type_check(typ);
    st.closeScope();
  }
  typ = ((*vec)[0])->expr->typ;
}

// class Constr

void Constr::sem(std::string id)
{
  ::Type *tmp = new Type_id(id);
  for (auto i = type_vec->rbegin(); i != type_vec->rend(); i++)
    tmp = new Type_Func(*i, tmp);
  st.insert(Id, tmp);
}

// class Par

void Par::sem()
{
  typ->sem();
  st.insert(id, typ);
}

// class TDef

void TDef::sem() { tt.insert(id); }

void TDef::sem2()
{
  for (Constr *c : *constr_vec)
  {
    c->sem(id);
  }
}

// class TypeDef

void TypeDef::sem()
{
  if (tdef_vec == nullptr)
    return;
  for (TDef *tdef : *tdef_vec)
  {
    tdef->sem();
  }
  for (TDef *tdef : *tdef_vec)
  {
    tdef->sem2();
  }
}

// class MutableDef

void MutableDef::sem()
{
  typ->sem();
  if (expr_vec == nullptr)
    st.insert(id, new Type_Ref(typ));
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

// class NormalDef

void NormalDef::sem()
{
  typ->sem();
  if (typ->get_type() == type_func)
    semanticError("Return type function not allowed");
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

// class LetDef

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

// class LetIn

void LetIn::sem()
{
  st.openScope();
  def->sem();
  expr->sem();
  typ = expr->typ;
  st.closeScope();
}
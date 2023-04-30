#include "ast.hpp"

template <typename T>
inline std::ostream &operator<<(std::ostream &out, const std::vector<T> &v)
{
  bool first = true;
  for (T t : v)
  {
    if (!first)
      out << ", ";
    first = false;
    out << *t;
  }
  return out;
}

// Class Program

void Program::printOn(std::ostream &out) const
{
  std::cout << "AST(" << *statements << ")" << std::endl;
}

// Class Type_Unit

void Type_Unit::printOn(std::ostream &out) const { out << "Type_Unit()"; }

// Class Type_Int

void Type_Int::printOn(std::ostream &out) const { out << "Type_Int()"; }

// Class Type_Float

void Type_Float::printOn(std::ostream &out) const { out << "Type_Float()"; }

// Class Type_Char

void Type_Char::printOn(std::ostream &out) const { out << "Type_Char()"; }

// Class Type_Bool

void Type_Bool::printOn(std::ostream &out) const { out << "Type_Bool()"; }

// Class Type_Func

void Type_Func::printOn(std::ostream &out) const
{
  out << "Type_Func(" << *from << ", " << *to << ")";
}

// Class Type_Ref

void Type_Ref::printOn(std::ostream &out) const
{
  out << "Type_Ref(" << *t << ")";
}

// Class Type_Array

void Type_Array::printOn(std::ostream &out) const
{
  out << "Type_Array(" << dim << ", " << *t << ")";
}

// class Type_id

void Type_id::printOn(std::ostream &out) const
{
  out << "Type_id(" << id << ")";
}

// class Type_Undefined

void Type_Undefined::printOn(std::ostream &out) const
{
  if (t == nullptr)
    out << "Type_Undefined()";
  else
    out << *t;
}

// class Int_Expr

void Int_Expr::printOn(std::ostream &out) const
{
  out << "Int_Expr(" << num << ")";
}

// class Float_Expr

void Float_Expr::printOn(std::ostream &out) const
{
  out << "Float_Expr(" << num << ")";
}

// class Char_Expr

void Char_Expr::printOn(std::ostream &out) const
{
  out << "Char_Expr(" << chr << ")";
}

// class Str_Expr

void Str_Expr::printOn(std::ostream &out) const
{
  out << "Str_Expr(" << str << ")";
}

// class Bool_Expr

void Bool_Expr::printOn(std::ostream &out) const
{
  out << "Bool_Expr(" << var << ")";
}

// class Unit_Expr

void Unit_Expr::printOn(std::ostream &out) const { out << "Unit_Expr()"; }

// class Array

void Array::printOn(std::ostream &out) const
{
  out << "Array(" << id << ", [" << *expr_vec << "])";
}

// class Dim

void Dim::printOn(std::ostream &out) const
{
  out << "Dim(" << ind << ", " << id << ")";
}

// class id

void id_Expr::printOn(std::ostream &out) const { out << "id(" << id << ")"; }

// class Id

void Id_Expr::printOn(std::ostream &out) const { out << "Id(" << id << ")"; }

// class While

void While::printOn(std::ostream &out) const
{
  out << "While(" << *cond << ", " << *stmt << ")";
}

// class For

void For::printOn(std::ostream &out) const
{
  std::string for_str = down ? " down to " : " to ";
  out << "For(" << id << " from " << *start << for_str << *end << ") do ("
      << *do_stmt << ")";
}

// class call

void call::printOn(std::ostream &out) const
{
  out << "call(" << id << ", (" << *expr_vec << "))";
}

// class UnOp

void UnOp::printOn(std::ostream &out) const
{
  out << "Unop(" << op << ", " << *e << ")";
}

// class BinOp

void BinOp::printOn(std::ostream &out) const
{
  out << "Binop(" << *left << ", " << op << ", " << *right << ")";
}

// class If

void If::printOn(std::ostream &out) const
{
  std::string ifelse = (expr3 != nullptr ? "If_Else(" : "If(");
  out << ifelse << *expr1 << ", " << *expr2;
  if (expr3 != nullptr)
    out << ", " << *expr3;
  out << ")";
}

// class New

void New::printOn(std::ostream &out) const { out << "New(" << *typ << ")"; }

// class Pattern_Int_Expr

void Pattern_Int_Expr::printOn(std::ostream &out) const
{
  out << "Pattern_Int_Expr(" << num << ")";
}

// class Pattern_Float_Expr

void Pattern_Float_Expr::printOn(std::ostream &out) const
{
  out << "Pattern_Float_Expr(" << num << ")";
}

// class Pattern_Char_Expr

void Pattern_Char_Expr::printOn(std::ostream &out) const
{
  out << "Pattern_Char_Expr(" << chr << ")";
}

// class Pattern_Bool_Expr

void Pattern_Bool_Expr::printOn(std::ostream &out) const
{
  out << "Pattern_Bool_Expr(" << var << ")";
}

// class Pattern_id

void Pattern_id::printOn(std::ostream &out) const
{
  out << "Pattern_id(" << id << ")";
}

// class Pattern_Id

void Pattern_Id::printOn(std::ostream &out) const
{
  out << "Pattern_Id(" << id << ")";
}

// class Pattern_Call

void Pattern_Call::printOn(std::ostream &out) const
{
  out << "Pattern_Call(" << id << ", (" << *pattern_vec << "))";
}

// class Clause

void Clause::printOn(std::ostream &out) const
{
  out << "Clause(" << *p << ", " << *e << ")";
}

// class Match

void Match::printOn(std::ostream &out) const
{
  out << "Match(" << *e << ", " << *vec << ")";
}

// class Constr

void Constr::printOn(std::ostream &out) const
{
  out << "Constr(" << Id;
  if (type_vec != nullptr)
    out << ", " << *type_vec;
  out << ")";
}

// class Par

void Par::printOn(std::ostream &out) const
{
  out << "Par(" << id;
  if (t != nullptr)
    out << ", " << *t;
  out << ")";
}

// class TDef

void TDef::printOn(std::ostream &out) const
{
  out << "TDef(" << id << ", " << *constr_vec << ")";
}

// class TypeDef

void TypeDef::printOn(std::ostream &out) const
{
  out << "Typedef(" << *tdef_vec << ")";
}

// class MutableDef

void MutableDef::printOn(std::ostream &out) const
{
  out << "MutableDef(" << id;
  if (expr_vec != nullptr)
    out << ", [" << *expr_vec << "]";
  if (typ != nullptr)
    out << ", " << *typ;
  out << ")";
}

// class NormalDef

void NormalDef::printOn(std::ostream &out) const
{
  out << "Def(" << id << ", [" << *par_vec << "], ";
  if (typ != nullptr)
    out << *typ << ", ";
  out << *expr << ")";
}

// class LetDef

void LetDef::printOn(std::ostream &out) const
{
  out << "Letdef(" << rec << ", " << *def_vec << ")";
}

// class LetIn

void LetIn::printOn(std::ostream &out) const
{
  out << "LetIn(" << *def << ", " << *expr << ")";
}

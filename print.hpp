#include "ast.hpp"

template <typename T>
inline std::ostream &operator<<(std::ostream &out, const std::vector<T> &v)
{
  bool first = true;
  for (T t : v)
  {
    if (!first)
    {
      out << ", ";
    }
    first = false;
    out << *t;
  }
  return out;
}

void Program::printOn(std::ostream &out) const
{
  std::cout << "AST(" << *statements << ")" << std::endl;
}

void LetDef::printOn(std::ostream &out) const
{
  out << "Letdef(" << rec << ", " << *def_vec << ")";
}

void NormalDef::printOn(std::ostream &out) const
{
  out << "Def(" << id << ", [" << *par_vec << "], ";
  if (typ != nullptr)
  {
    out << *typ << ", ";
  }
  out << *expr << ")";
}

void MutableDef::printOn(std::ostream &out) const
{
  out << "MutableDef(" << id;
  if (expr_vec != nullptr)
  {
    out << ", [" << *expr_vec << "]";
  }
  if (typ != nullptr)
  {
    out << ", " << *typ;
  }
  out << ")";
}

void TypeDef::printOn(std::ostream &out) const
{
  out << "Typedef(" << *tdef_vec << ")";
}

void TDef::printOn(std::ostream &out) const
{
  out << "TDef(" << id << ", " << *constr_vec << ")";
}

void Constr::printOn(std::ostream &out) const
{
  out << "Constr(" << Id;
  if (type_vec != nullptr)
  {
    out << ", " << *type_vec;
  }
  out << ")";
}

void Par::printOn(std::ostream &out) const
{
  out << "Par(" << id;
  if (typ != nullptr)
  {
    out << ", " << *typ;
  }
  out << ")";
}

void Type_Unit::printOn(std::ostream &out) const
{
  out << "Type_Unit()";
}

void Type_Int::printOn(std::ostream &out) const
{
  out << "Type_Int()";
}

void Type_Char::printOn(std::ostream &out) const
{
  out << "Type_Char()";
}

void Type_Bool::printOn(std::ostream &out) const
{
  out << "Type_Bool()";
}

void Type_Float::printOn(std::ostream &out) const
{
  out << "Type_Float()";
}

void Type_Func::printOn(std::ostream &out) const
{
  out << "Type_Func(" << *from << ", " << *to << ")";
}

void Type_Ref::printOn(std::ostream &out) const
{
  out << "Type_Ref(" << *typ << ")";
}

void Type_Array::printOn(std::ostream &out) const
{
  out << "Type_Array(" << dim << ", " << *typ << ")";
}

void Type_id::printOn(std::ostream &out) const
{
  out << "Type_id(" << id << ")";
}

void Type_Undefined::printOn(std::ostream &out) const
{
  if (typ == nullptr)
  {
    out << "Type_Undefined()";
  }
  else
  {
    out << *typ;
  }
}

void Int_Expr::printOn(std::ostream &out) const
{
  out << "Int(" << num << ")";
}

void Float_Expr::printOn(std::ostream &out) const
{
  out << "Float(" << num << ")";
}

void Char_Expr::printOn(std::ostream &out) const
{
  out << "Char(" << ch << ")";
}

void Str_Expr::printOn(std::ostream &out) const
{
  out << "Str(" << str << ")";
}

void Bool_Expr::printOn(std::ostream &out) const
{
  out << "Bool(" << boolean << ")";
}

void Unit_Expr::printOn(std::ostream &out) const
{
  out << "Unit()";
}

void UnOp::printOn(std::ostream &out) const
{
  out << "Unop(" << op << ", " << *expr << ")";
}

void BinOp::printOn(std::ostream &out) const
{
  out << "Binop(" << *left << ", " << op << ", " << *right << ")";
}

void id_Expr::printOn(std::ostream &out) const
{
  out << "id(" << id << ")";
}

void Id_Expr::printOn(std::ostream &out) const
{
  out << "Id(" << Id << ")";
}

void call::printOn(std::ostream &out) const
{
  out << "call(" << id << ", (" << *expr_vec << "))";
}

void Array::printOn(std::ostream &out) const
{
  out << "Array(" << id << ", [" << *expr_vec << "])";
}

void Dim::printOn(std::ostream &out) const
{
  out << "Dim(" << ind << ", " << id << ")";
}

void New::printOn(std::ostream &out) const
{
  out << "New(" << *typ << ")";
}

void LetIn::printOn(std::ostream &out) const
{
  out << "LetIn(" << *letdef << ", " << *expr << ")";
}

void If::printOn(std::ostream &out) const
{
  std::string ifelse = (expr3 != nullptr ? "If_Else(" : "If(");
  out << ifelse << *expr1 << ", " << *expr2;
  if (expr3 != nullptr)
  {
    out << ", " << *expr3;
  }
  out << ")";
}

void While::printOn(std::ostream &out) const
{
  out << "While(" << *cond << ", " << *stmt << ")";
}

void For::printOn(std::ostream &out) const
{
  std::string for_str = down ? " down to " : " to ";
  out << "For(" << id << " from " << *start << for_str << *end << ") do ("
      << *stmt << ")";
}

void Match::printOn(std::ostream &out) const
{
  out << "Match(" << *expr << ", " << *clause_vec << ")";
}

void Clause::printOn(std::ostream &out) const
{
  out << "Clause(" << *pat << ", " << *expr << ")";
}

void Pattern_Int_Expr::printOn(std::ostream &out) const
{
  out << "Pattern_Int(" << num << ")";
}

void Pattern_Float_Expr::printOn(std::ostream &out) const
{
  out << "Pattern_Float(" << num << ")";
}

void Pattern_Char_Expr::printOn(std::ostream &out) const
{
  out << "Pattern_Char(" << ch << ")";
}

void Pattern_Bool_Expr::printOn(std::ostream &out) const
{
  out << "Pattern_Bool(" << boolean << ")";
}

void Pattern_id::printOn(std::ostream &out) const
{
  out << "Pattern_id(" << id << ")";
}

void Pattern_Id::printOn(std::ostream &out) const
{
  out << "Pattern_Id(" << Id << ")";
}

void Pattern_Call::printOn(std::ostream &out) const
{
  out << "Pattern_Call(" << Id << ", (" << *pattern_vec << "))";
}

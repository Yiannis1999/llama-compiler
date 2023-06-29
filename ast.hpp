#pragma once

#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "symbol.hpp"

// llvm includes
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/Transforms/Utils.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>

using namespace llvm;

typedef enum
{
  unop_plus,
  unop_minus,
  unop_float_plus,
  unop_float_minus,
  unop_exclamation,
  unop_not,
  unop_delete
} unop_enum;

typedef enum
{
  binop_plus,
  binop_minus,
  binop_mult,
  binop_div,
  binop_float_plus,
  binop_float_minus,
  binop_float_mult,
  binop_float_div,
  binop_mod,
  binop_pow,
  binop_struct_eq,
  binop_struct_ne,
  binop_l,
  binop_g,
  binop_leq,
  binop_geq,
  binop_phys_eq,
  binop_phys_ne,
  binop_and,
  binop_or,
  binop_semicolon,
  binop_assign
} binop_enum;

typedef enum
{
  type_unit,
  type_int,
  type_char,
  type_bool,
  type_float,
  type_func,
  type_ref,
  type_array,
  type_id,
  type_undefined
} main_type;

class AST
{
public:
  virtual ~AST() {}
  virtual void printOn(std::ostream &out) const = 0;
  virtual void sem() {}

protected:
  static std::string str_print_int;
  static std::string str_print_bool;
  static std::string str_print_char;
  static std::string str_print_float;
  static std::string str_print_string;
  static std::string str_read_int;
  static std::string str_read_bool;
  static std::string str_read_char;
  static std::string str_read_float;
  static std::string str_read_string;
  static std::string str_abs;
  static std::string str_fabs;
  static std::string str_sqrt;
  static std::string str_sin;
  static std::string str_cos;
  static std::string str_tan;
  static std::string str_atan;
  static std::string str_exp;
  static std::string str_ln;
  static std::string str_pi;
  static std::string str_incr;
  static std::string str_decr;
  static std::string str_float_of_int;
  static std::string str_int_of_float;
  static std::string str_round;
  static std::string str_int_of_char;
  static std::string str_char_of_int;
  static std::string str_strlen;
  static std::string str_strcmp;
  static std::string str_strcpy;
  static std::string str_strcat;

  static LLVMContext TheContext;
  static IRBuilder<> Builder;
  static std::unique_ptr<Module> TheModule;
  static std::unique_ptr<legacy::FunctionPassManager> TheFPM;

  // Type Shortcuts
  static llvm::Type *i1;
  static llvm::Type *i8;
  static llvm::Type *i32;
  static llvm::Type *i64;
  static llvm::Type *flo;
  static llvm::StructType *voi;

  // LLVM helper functions
  static ConstantInt *c1(bool b)
  {
    return ConstantInt::get(TheContext, APInt(1, b, true));
  }
  static ConstantInt *c8(char c)
  {
    return ConstantInt::get(TheContext, APInt(8, c, true));
  }
  static ConstantInt *c32(int n)
  {
    return ConstantInt::get(TheContext, APInt(32, n, true));
  }
  static ConstantInt *c64(int n)
  {
    return ConstantInt::get(TheContext, APInt(64, n, true));
  }
  static Constant *cfloat(float f)
  {
    return ConstantFP::get(flo, f);
  }
  static Constant *cvoid()
  {
    return llvm::ConstantStruct::get(voi, {});
  }
};

inline std::ostream &operator<<(std::ostream &out, const AST &t)
{
  t.printOn(out);
  return out;
}

class Stmt : public AST
{
public:
  virtual void compile() const {}
};

class Program : public AST
{
public:
  Program(std::vector<Stmt *> *s) : statements(s) {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual void compile() const;
  void llvm_compile_and_dump(bool optimize, llvm::raw_fd_ostream* imm_file, llvm::raw_fd_ostream* asm_file);

private:
  std::vector<Stmt *> *statements;
};

class Type : public AST
{
public:
  virtual main_type get_type() { return type_undefined; }
  virtual ::Type *getChild1() { return nullptr; }
  virtual ::Type *getChild2() { return nullptr; }
  virtual int getDim() { return 0; }
  virtual std::string get_id() { return ""; }
  virtual bool equals(::Type *other)
  {
    if (other == nullptr)
      return false;
    if (other->get_type() == type_undefined)
      return other->equals(this);
    return this->get_type() == other->get_type();
  }
  virtual llvm::Type *compile() const { return nullptr; }
};

class Type_Unit : public ::Type
{
public:
  Type_Unit() {}
  virtual void printOn(std::ostream &out) const override;
  virtual main_type get_type() override;
  virtual llvm::Type *compile() const override { return voi; };
};

class Type_Int : public ::Type
{
public:
  Type_Int() {}
  virtual void printOn(std::ostream &out) const override;
  virtual main_type get_type() override;
  virtual llvm::Type *compile() const override { return i64; };
};

class Type_Float : public ::Type
{
public:
  Type_Float() {}
  virtual void printOn(std::ostream &out) const override;
  virtual main_type get_type() override;
  virtual llvm::Type *compile() const override { return flo; };
};

class Type_Char : public ::Type
{
public:
  Type_Char() {}
  virtual void printOn(std::ostream &out) const override;
  virtual main_type get_type() override;
  virtual llvm::Type *compile() const override { return i8; };
};

class Type_Bool : public ::Type
{
public:
  Type_Bool() {}
  virtual void printOn(std::ostream &out) const override;
  virtual main_type get_type() override;
  virtual llvm::Type *compile() const override { return i1; };
};

class Type_Func : public ::Type
{
public:
  Type_Func(::Type *t1, ::Type *t2) : from(t1), to(t2) {}
  virtual void printOn(std::ostream &out) const override;
  virtual main_type get_type() override;
  virtual ::Type *getChild1() override;
  virtual ::Type *getChild2() override;
  virtual bool equals(::Type *other) override;
  virtual void sem() override;
  virtual llvm::Type *compile() const override;

private:
  ::Type *from, *to;
};

class Type_Ref : public ::Type
{
public:
  Type_Ref(::Type *t) : typ(t) {}
  virtual void printOn(std::ostream &out) const override;
  virtual main_type get_type() override;
  virtual ::Type *getChild1() override;
  virtual bool equals(::Type *other) override;
  virtual void sem() override;
  virtual llvm::Type *compile() const override;

private:
  ::Type *typ;
};

class Type_Array : public ::Type
{
public:
  Type_Array(int i, ::Type *t1) : dim(i), typ(t1) {}
  virtual void printOn(std::ostream &out) const override;
  virtual main_type get_type() override;
  virtual ::Type *getChild1() override;
  virtual int getDim() override;
  virtual bool equals(::Type *other) override;
  virtual void sem() override;
  virtual llvm::Type *compile() const override { return PointerType::get(typ->compile(), 0); };

private:
  int dim;
  ::Type *typ;
};

class Type_id : public ::Type
{
public:
  Type_id(std::string s) : id(s) {}
  virtual void printOn(std::ostream &out) const override;
  virtual main_type get_type() override;
  virtual std::string get_id() override;
  virtual bool equals(::Type *other) override;
  virtual void sem() override;
  virtual llvm::Type *compile() const override;

private:
  std::string id;
};

class Type_Undefined : public ::Type
{
public:
  Type_Undefined() : typ(nullptr) {}
  virtual void printOn(std::ostream &out) const override;
  virtual main_type get_type() override;
  virtual ::Type *getChild1() override;
  virtual ::Type *getChild2() override;
  virtual int getDim() override;
  virtual std::string get_id() override;
  virtual bool equals(::Type *other) override;
  virtual llvm::Type *compile() const override { return typ == nullptr ? i64 : typ->compile(); };

private:
  ::Type *typ;
};

class Expr : public AST
{
public:
  virtual void type_check(::Type *t);
  ::Type *typ;
  virtual Value *compile() const { return nullptr; };
};

class Int_Expr : public Expr
{
public:
  Int_Expr(int n) : num(n) {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual Value *compile() const override;

private:
  int num;
};

class Float_Expr : public Expr
{
public:
  Float_Expr(float n) : num(n) {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual Value *compile() const override;

private:
  float num;
};

class Char_Expr : public Expr
{
public:
  Char_Expr(std::string s) : ch(s[1])
  {
    if (s[1] == '\\')
    {
      switch (s[2])
      {
      case 'n':
        ch = '\n';
        break;
      case 't':
        ch = '\t';
        break;
      case 'r':
        ch = '\r';
        break;
      case '0':
        ch = '\0';
        break;
      case '\\':
        ch = '\\';
        break;
      case '\'':
        ch = '\'';
        break;
      case '\"':
        ch = '\"';
        break;
      case 'x':
      {
        std::string hex_str;
        hex_str += s[3];
        hex_str += s[4];
        char hex_value = std::stoi(hex_str, nullptr, 16);
        ch = hex_value;
        break;
      }
      }
    }
  }
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual Value *compile() const override;

private:
  char ch;
};

class Str_Expr : public Expr
{
public:
  Str_Expr(std::string s) : str("")
  {
    for (std::size_t i = 1; i < s.size() - 1; i++)
    {
      if (s[i] == '\\')
      {
        i++;
        switch (s[i])
        {
        case 'n':
          str += '\n';
          break;
        case 't':
          str += '\t';
          break;
        case 'r':
          str += '\r';
          break;
        case '0':
          str += '\0';
          break;
        case '\\':
          str += '\\';
          break;
        case '\'':
          str += '\'';
          break;
        case '\"':
          str += '\"';
          break;
        case 'x':
        {
          std::string hex_str;
          hex_str += s[++i];
          hex_str += s[++i];
          char hex_value = std::stoi(hex_str, nullptr, 16);
          str += hex_value;
          break;
        }
        }
      }
      else
      {
        str += s[i];
      }
    }
  }
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual Value *compile() const override;

private:
  std::string str;
};

class Bool_Expr : public Expr
{
public:
  Bool_Expr(bool b) : boolean(b) {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual Value *compile() const override;

private:
  bool boolean;
};

class Unit_Expr : public Expr
{
public:
  Unit_Expr() {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual Value *compile() const override;
};

class Array : public Expr
{
public:
  Array(std::string s, std::vector<Expr *> *v) : id(s), expr_vec(v) {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual Value *compile() const override;

private:
  std::string id;
  std::vector<Expr *> *expr_vec;
};

class Dim : public Expr
{
public:
  Dim(std::string s, int i = 1) : id(s), ind(i) {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual Value *compile() const override;

private:
  std::string id;
  int ind;
};

class id_Expr : public Expr
{
public:
  id_Expr(std::string s) : id(s) {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual Value *compile() const override;

private:
  std::string id;
};

class Id_Expr : public Expr
{
public:
  Id_Expr(std::string s) : Id(s) {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual Value *compile() const override;

private:
  std::string Id;
};

class While : public Expr
{
public:
  While(Expr *e1, Expr *e2) : cond(e1), stmt(e2) {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual Value *compile() const override;

private:
  Expr *cond, *stmt;
};

class For : public Expr
{
public:
  For(std::string id1, Expr *start1, Expr *end1, Expr *stmt1, bool down1)
      : id(id1), start(start1), end(end1), stmt(stmt1), down(down1) {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual Value *compile() const override;

private:
  std::string id;
  Expr *start, *end, *stmt;
  bool down;
};

class call : public Expr
{
public:
  call(std::string s, std::vector<Expr *> *v) : id(s), expr_vec(v) {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual Value *compile() const override;

private:
  std::string id;
  std::vector<Expr *> *expr_vec;
};

class UnOp : public Expr
{
public:
  UnOp(unop_enum op1, Expr *e1) : op(op1), expr(e1) {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual Value *compile() const override;

private:
  unop_enum op;
  Expr *expr;
};

class BinOp : public Expr
{
public:
  BinOp(Expr *e1, binop_enum op1, Expr *e2) : left(e1), op(op1), right(e2) {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual Value *compile() const override;

private:
  Expr *left;
  binop_enum op;
  Expr *right;
};

class If : public Expr
{
public:
  If(Expr *e1, Expr *e2, Expr *e3) : expr1(e1), expr2(e2), expr3(e3) {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual Value *compile() const override;

private:
  Expr *expr1, *expr2, *expr3;
};

class New : public Expr
{
public:
  New(::Type *t) : ty(t) {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual Value *compile() const override;

private:
  ::Type *ty;
};

class Pattern : public AST
{
public:
  virtual void sem() {}
  virtual Value *compile(Value *v) const { return nullptr; }
  ::Type *typ;
};

class Pattern_Int_Expr : public Pattern
{
public:
  Pattern_Int_Expr(int n) : num(n) {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual Value *compile(Value *v) const override;

private:
  int num;
};

class Pattern_Float_Expr : public Pattern
{
public:
  Pattern_Float_Expr(float n) : num(n) {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual Value *compile(Value *v) const override;

private:
  float num;
};

class Pattern_Char_Expr : public Pattern
{
public:
  Pattern_Char_Expr(std::string s) : ch(s[1])
  {
    if (s[1] == '\\')
    {
      switch (s[2])
      {
      case 'n':
        ch = '\n';
        break;
      case 't':
        ch = '\t';
        break;
      case 'r':
        ch = '\r';
        break;
      case '0':
        ch = '\0';
        break;
      case '\\':
        ch = '\\';
        break;
      case '\'':
        ch = '\'';
        break;
      case '\"':
        ch = '\"';
        break;
      case 'x':
      {
        std::string hex_str;
        hex_str += s[3];
        hex_str += s[4];
        char hex_value = std::stoi(hex_str, nullptr, 16);
        ch = hex_value;
        break;
      }
      }
    }
  }
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual Value *compile(Value *v) const override;

private:
  char ch;
};

class Pattern_Bool_Expr : public Pattern
{
public:
  Pattern_Bool_Expr(bool b) : boolean(b) {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual Value *compile(Value *v) const override;

private:
  bool boolean;
};

class Pattern_id : public Pattern
{
public:
  Pattern_id(std::string s) : id(s) {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual Value *compile(Value *v) const override;

private:
  std::string id;
};

class Pattern_Id : public Pattern
{
public:
  Pattern_Id(std::string s) : Id(s) {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual Value *compile(Value *v) const override;

private:
  std::string Id;
};

class Pattern_Call : public Pattern
{
public:
  Pattern_Call(std::string s, std::vector<Pattern *> *v)
      : Id(s), pattern_vec(v) {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual Value *compile(Value *v) const override;

private:
  std::string Id;
  std::vector<Pattern *> *pattern_vec;
};

class Clause : public AST
{
public:
  Clause(Pattern *p1, Expr *e1) : pat(p1), expr(e1) {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  Pattern *pat;
  Expr *expr;
};

class Match : public Expr
{
public:
  Match(Expr *e1, std::vector<Clause *> *v) : expr(e1), vec(v) {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual Value *compile() const override;

private:
  Expr *expr;
  std::vector<Clause *> *vec;
};

class Constr : public AST
{
public:
  Constr(std::string s, std::vector<::Type *> *v) : Id(s), type_vec(v) {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual void compile() const;
  std::string id, Id;

private:
  std::vector<::Type *> *type_vec;
};

class Par : public AST
{
public:
  Par(std::string s, ::Type *t) : typ(t), id(s) {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  ::Type *typ;
  std::string id;
};

class TDef : public AST
{
public:
  TDef(std::string s, std::vector<Constr *> *v) : id(s), constr_vec(v) {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual void sem2();
  virtual void compile() const;
  virtual void compile2() const;

private:
  std::string id;
  std::vector<Constr *> *constr_vec;
};

class TypeDef : public Stmt
{
public:
  TypeDef(std::vector<TDef *> *v) : tdef_vec(v) {}
  virtual void sem() override;
  virtual void printOn(std::ostream &out) const override;
  virtual void compile() const override;

private:
  std::vector<TDef *> *tdef_vec;
};

class Def : public AST
{
public:
  virtual void sem2(){};
  virtual void compile() const {}
  virtual void compile2() const {}
};

class NormalDef : public Def
{
public:
  NormalDef(std::string s, std::vector<Par *> *v, ::Type *t, Expr *e)
      : id(s), par_vec(v), typ(t), expr(e) {}
  virtual void sem() override;
  virtual void sem2() override;
  virtual void printOn(std::ostream &out) const override;
  virtual void compile() const override;
  virtual void compile2() const override;

private:
  std::string id;
  std::vector<Par *> *par_vec;
  ::Type *typ;
  Expr *expr;
};

class MutableDef : public Def
{
public:
  MutableDef(std::string s, std::vector<Expr *> *e, ::Type *t)
      : id(s), expr_vec(e), typ(t) {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual void compile() const override;

private:
  std::string id;
  std::vector<Expr *> *expr_vec;
  ::Type *typ;
};

class LetDef : public Stmt
{
public:
  LetDef(bool b, std::vector<Def *> *v) : rec(b), def_vec(v) {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual void compile() const override;

private:
  bool rec;
  std::vector<Def *> *def_vec;
};

class LetIn : public Expr
{
public:
  LetIn(LetDef *d, Expr *e) : letdef(d), expr(e) {}
  virtual void printOn(std::ostream &out) const override;
  virtual void sem() override;
  virtual Value *compile() const override;

private:
  LetDef *letdef;
  Expr *expr;
};
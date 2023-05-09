%{
#include <cstdio>
#include "ast.hpp"
#include "lexer.hpp"

SymbolTable st;
//ConstrTable ct;
TypeDefTable tt;
%}

%token T_and
%token T_dim
%token T_false
%token T_let
%token T_of
%token T_type
%token T_array
%token T_do
%token T_float
%token T_match
%token T_rec
%token T_unit
%token T_begin
%token T_done
%token T_for
%token T_mod
%token T_ref
%token T_while
%token T_bool
%token T_downto
%token T_if
%token T_mutable
%token T_then
%token T_with
%token T_char
%token T_else
%token T_in
%token T_new
%token T_to
%token T_delete
%token T_end
%token T_int
%token T_not
%token T_true
%token T_Id
%token T_id
%token T_int_expr
%token T_float_expr
%token T_char_expr
%token T_str_expr
%token T_arrow_op
%token T_plus_op
%token T_minus_op
%token T_mult_op
%token T_div_op
%token T_pow_op
%token T_and_op
%token T_or_op
%token T_struct_diff_op
%token T_leq_op
%token T_geq_op
%token T_eq_op
%token T_diff_op
%token T_assign_op

%nonassoc LET_IN
%left ';'
%nonassoc IF_THEN_ELSE
%right T_else
%nonassoc T_assign_op
%left T_or_op
%left T_and_op
%nonassoc '=' T_struct_diff_op '>' '<' T_leq_op T_geq_op T_eq_op T_diff_op
%left '+' '-' T_plus_op T_minus_op
%left '*' '/' T_mult_op T_div_op T_mod
%right T_pow_op
%right T_arrow_op
%right T_array T_of T_ref
%nonassoc '!'
%nonassoc T_new

%union {

  Program *program;
  std::vector<Stmt *> *stmt_vec;
  Stmt *stmt;
  std::vector<Def *> *def_vec;
  LetDef *letdef;
  Def *def;
  std::vector<Par *> *par_vec;
  std::vector<Expr *> *expr_vec;
  TypeDef *type_def;
  std::vector<TDef *> *tdef_vec;
  TDef *tdef;
  std::vector<Constr *> *constr_vec;
  Constr *constr;
  std::vector<::Type *> *type_vec;
  Par *par;
  ::Type *type;
  int stars;
  Expr *expr;
  int int_expr;
  float float_expr;
  char char_expr;
  char* str_expr;
  char* var;
  Pattern* pattern;
  Clause* clause;
  std::vector<Clause *> *clause_vec;
  std::vector<Pattern *> *pattern_vec;
}

%type<program> program
%type<stmt_vec> stmt_list
%type<stmt> stmt
%type<def_vec> and_def_list
%type<letdef> letdef
%type<def> def
%type<par_vec> par_list
%type<expr_vec> comma_expr_list expr_list
%type<type_def> typedef
%type<tdef_vec> and_tdef_list
%type<tdef> tdef 
%type<constr_vec> constr_list
%type<constr> constr
%type<type_vec> constr_type_list
%type<par> par
%type<type> type
%type<stars> comma_star_list
%type<expr> expr expr1 expr2 expr3 expr4 expr5
%type<int_expr> T_int_expr
%type<float_expr> T_float_expr
%type<str_expr> T_char_expr
%type<str_expr> T_str_expr
%type<var> T_id T_Id
%type<pattern> pattern pattern1
%type<clause> clause
%type<clause_vec> or_clause_list
%type<pattern_vec> pattern_list
%%

program:
  stmt_list {
    $$ = new Program($1);
    $$->sem();
    // std::cout << *$$;
    $$->llvm_compile_and_dump(false);
   }
;

stmt_list:
  %empty { $$ = new std::vector<Stmt *>; }
| stmt_list stmt { $1->push_back($2); $$ = $1; }
;

stmt:
  letdef { $$ = $1; }
| typedef { $$ = $1; }
;

letdef:
  T_let and_def_list { $$ = new LetDef(false, $2); }
| T_let T_rec and_def_list { $$ = new LetDef(true, $3); }
;

and_def_list:
  def { $$ = new std::vector<Def *>; $$->push_back($1); }
| and_def_list T_and def { $1->push_back($3); $$ = $1; }
;

def:
  T_id par_list '=' expr { $$ = new NormalDef($1, $2, new Type_Undefined(), $4); }
| T_id par_list ':' type '=' expr { $$ = new NormalDef($1, $2, $4, $6); }
| T_mutable T_id { $$ = new MutableDef($2, nullptr, new Type_Undefined()); }
| T_mutable T_id '[' comma_expr_list ']' { $$ = new MutableDef($2, $4, new Type_Undefined()); }
| T_mutable T_id ':' type { $$ = new MutableDef($2, nullptr, $4); }
| T_mutable T_id '[' comma_expr_list ']' ':' type { $$ = new MutableDef($2, $4, $7); }
;

par_list:
  %empty { $$ = new std::vector<Par *>; }
| par_list par { $1->push_back($2); $$ = $1; }
;

comma_expr_list:
  expr { $$ = new std::vector<Expr *>; $$->push_back($1); }
| comma_expr_list ',' expr { $1->push_back($3); $$ = $1; }
;

typedef:
  T_type and_tdef_list { $$ = new TypeDef($2); }
;

and_tdef_list:
  tdef { $$ = new std::vector<TDef *>; $$->push_back($1); }
| and_tdef_list T_and tdef { $1->push_back($3); $$ = $1; }
;

tdef:
  T_id '=' constr_list { $$ = new TDef($1, $3); }
;
constr_list:
  constr { $$ = new std::vector<Constr *>; $$->push_back($1); }
| constr_list '|' constr { $1->push_back($3); $$ = $1; }
;

constr:
  T_Id { $$ = new Constr($1, new std::vector<::Type *>); }
| T_Id T_of constr_type_list { $$ = new Constr($1, $3); }
;

constr_type_list:
  type { $$ = new std::vector<::Type *>; $$->push_back($1); }
| constr_type_list type { $1->push_back($2); $$ = $1; }
;

par:
  T_id { $$ = new Par($1, new Type_Undefined()); }
| '(' T_id ':' type ')' { $$ = new Par($2, $4); }
;

type:
  T_unit { $$ = new Type_Unit(); }
| T_int { $$ = new Type_Int(); }
| T_char { $$ = new Type_Char(); }
| T_bool { $$ = new Type_Bool(); }
| T_float { $$ = new Type_Float(); }
| '(' type ')' { $$ = $2; }
| type T_arrow_op type { $$ = new Type_Func($1, $3); }
| type T_ref { $$ = new Type_Ref($1); }
| T_array T_of type { $$ = new Type_Array(1, $3); }
| T_array '[' comma_star_list ']' T_of type { $$ = new Type_Array($3, $6); }
| T_id { $$ = new Type_id($1); }
;

comma_star_list:
  '*' { $$ = 1; }
| comma_star_list ',' '*' { $$ = $1 + 1; }
;

expr:
  expr5 { $$ = $1; }
| letdef T_in expr %prec LET_IN { $$ = new LetIn($1, $3); }
| expr ';' expr { $$ = new BinOp($1, binop_semicolon, $3); }
;

expr1:
  T_int_expr { $$ = new Int_Expr($1); }
| T_float_expr { $$ = new Float_Expr($1); }
| T_char_expr { $$ = new Char_Expr($1); }
| T_str_expr { $$ = new Str_Expr($1); }
| T_true { $$ = new Bool_Expr(true); }
| T_false { $$ = new Bool_Expr(false); }
| '(' ')' { $$ = new Unit_Expr(); }
| '(' expr ')' { $$ = $2; }
| T_begin expr T_end { $$ = $2; }
| T_id '[' comma_expr_list ']' { $$ = new Array($1, $3); }
| T_dim T_id { $$ = new Dim($2); }
| T_dim T_int_expr T_id { $$ = new Dim($3, $2); }
| T_new type { $$ = new New($2); }
| T_id { $$ = new id_Expr($1); }
| T_Id { $$ = new Id_Expr($1); }
| '!' expr1 { $$ = new UnOp(unop_exclamation, $2); }
| T_while expr T_do expr T_done { $$ = new While($2, $4); }
| T_for T_id '=' expr T_to expr T_do expr T_done { $$ = new For($2, $4, $6, $8, false); }
| T_for T_id '=' expr T_downto expr T_do expr T_done { $$ = new For($2, $4, $6, $8, true); }
| T_match expr T_with or_clause_list T_end { $$ = new Match($2,$4); }
;

expr2:
  expr1 { $$ = $1; }
| T_id expr_list { $$ = new call($1, $2); }
| T_Id expr_list { $$ = new call($1, $2); }
;

expr3:
  expr2 { $$ = $1; }
| '+' expr2 { $$ = new UnOp(unop_plus, $2); }
| '-' expr2 { $$ = new UnOp(unop_minus, $2); }
| T_plus_op expr2 { $$ = new UnOp(unop_float_plus, $2); }
| T_minus_op expr2 { $$ = new UnOp(unop_float_minus, $2); }
| T_not expr2 { $$ = new UnOp(unop_not, $2); }
| T_delete expr2 { $$ = new UnOp(unop_delete, $2); }
;

expr4:
  expr3 { $$ = $1; }
| expr4 '+' expr4 { $$ = new BinOp($1, binop_plus , $3); }
| expr4 '-' expr4 { $$ = new BinOp($1, binop_minus , $3); }
| expr4 '*' expr4 { $$ = new BinOp($1, binop_mult , $3); }
| expr4 '/' expr4 { $$ = new BinOp($1, binop_div , $3); }
| expr4 T_plus_op expr4 { $$ = new BinOp($1, binop_float_plus , $3); }
| expr4 T_minus_op expr4 { $$ = new BinOp($1, binop_float_minus , $3); }
| expr4 T_mult_op expr4 { $$ = new BinOp($1, binop_float_mult , $3); }
| expr4 T_div_op expr4 { $$ = new BinOp($1, binop_float_div , $3); }
| expr4 T_mod expr4 { $$ = new BinOp($1, binop_mod , $3); }
| expr4 T_pow_op expr4 { $$ = new BinOp($1, binop_pow , $3); }
| expr4 '=' expr4 { $$ = new BinOp($1, binop_struct_eq , $3); }
| expr4 T_struct_diff_op expr4 { $$ = new BinOp($1, binop_struct_diff , $3); }
| expr4 '<' expr4 { $$ = new BinOp($1, binop_l , $3); }
| expr4 '>' expr4 { $$ = new BinOp($1, binop_g , $3); }
| expr4 T_leq_op expr4 { $$ = new BinOp($1, binop_leq , $3); }
| expr4 T_geq_op expr4 { $$ = new BinOp($1, binop_geq , $3); }
| expr4 T_eq_op expr4 { $$ = new BinOp($1, binop_logic_eq , $3); } 
| expr4 T_diff_op expr4 { $$ = new BinOp($1, binop_logic_diff , $3); }
| expr4 T_and_op expr4 { $$ = new BinOp($1, binop_and , $3); }
| expr4 T_or_op expr4 { $$ = new BinOp($1, binop_or , $3); }
| expr4 T_assign_op expr4 { $$ = new BinOp($1, binop_assign , $3); }
;

expr5:
  expr4 { $$ = $1; }
| T_if expr5 T_then expr %prec IF_THEN_ELSE { $$ = new If($2 ,$4, nullptr ); }
| T_if expr5 T_then expr T_else expr %prec IF_THEN_ELSE { $$ = new If($2 ,$4, $6 ); }
;

or_clause_list:
  clause { $$ = new std::vector<Clause *>; $$->push_back($1); }
| or_clause_list '|' clause { $1->push_back($3); $$ = $1; }
;

expr_list:
  expr1 { $$ = new std::vector<Expr *>; $$->push_back($1); }
| expr_list expr1 { $1->push_back($2); $$ = $1; }
;

clause:
  pattern T_arrow_op expr { $$ = new Clause($1, $3); }
;

pattern:
  pattern1 { $$ = $1; }
| T_Id pattern_list { $$ = new Pattern_Call($1, $2); }
;

pattern1:
  T_int_expr { $$ = new Pattern_Int_Expr($1); }
| '+' T_int_expr { $$ = new Pattern_Int_Expr($2); }
| '-' T_int_expr { $$ = new Pattern_Int_Expr(-$2); }
| T_float_expr { $$ = new Pattern_Float_Expr($1); }
| T_plus_op T_float_expr { $$ = new Pattern_Float_Expr($2); }
| T_minus_op T_float_expr { $$ = new Pattern_Float_Expr(-$2); }
| T_char_expr { $$ = new Pattern_Char_Expr($1); }
| T_true { $$ = new Pattern_Bool_Expr(true); }
| T_false { $$ = new Pattern_Bool_Expr(false); }
| T_id { $$ = new Pattern_id($1); }
| '(' pattern ')' { $$ = $2; }
| T_Id { $$ = new Pattern_Id($1); }
;

pattern_list:
  pattern1 { $$ = new std::vector<Pattern *>; $$->push_back($1); }
| pattern_list pattern1 { $1->push_back($2); $$ = $1; }
;

%%

void yyerror(const char *msg) {
  fprintf (stderr, "Error: %s\n", msg);
  exit(1);
}

int main() {
  int result = yyparse();
  return result;
}
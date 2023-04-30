%option noyywrap
%x COMMENT

%{
#include <cstdio>
#include <cstdlib>
#include "ast.hpp"
#include "parser.hpp"
#include "lexer.hpp"
%}

%{
#define YYEOF 0
int lineno = 1;
int commentno = 0;
%}

l [a-z]
L [A-Z]
D [0-9]
W [ \t\r]
HEX x[0-9A-Fa-f][0-9A-Fa-f]
ESC_CHAR \\([ntr0\\\'\"]|{HEX})
SIMPLE_CHAR [^\'\"\x00-\x19\x80-\xff]
CHAR {SIMPLE_CHAR}|{ESC_CHAR}

%%

"and"   { return T_and; }
"dim" { return T_dim; }
"false"   { return T_false; }
"let"    { return T_let; }
"of"  { return T_of; }
"type"    { return T_type; }
"array" { return T_array; }
"do"   { return T_do; }
"float"   { return T_float; }
"match"   { return T_match; }
"rec"   { return T_rec; }
"unit"   { return T_unit; }
"begin"   { return T_begin; }
"done"   { return T_done; }
"for"   { return T_for; }
"mod"   { return T_mod; }
"ref"   { return T_ref; }
"while" { return T_while; }
"bool"   { return T_bool; }
"downto"    { return T_downto; }
"if"  { return T_if; }
"mutable"    { return T_mutable; }
"then" { return T_then; }
"with"   { return T_with; }
"char"   { return T_char; }
"else" { return T_else; }
"in"   { return T_in; }
"new"    { return T_new; }
"to"  { return T_to; }
"delete"    { return T_delete; }
"end" { return T_end; }
"int"   { return T_int; }
"not"    { return T_not; }
"true"  { return T_true; }

[A-Z][A-Za-z_0-9]* { yylval.var = strdup(yytext); return T_Id; }
[a-z][A-Za-z_0-9]*   { yylval.var = strdup(yytext); return T_id; }
[0-9]+\.[0-9]+([eE][\-+][0-9]+)? { yylval.float_expr = atof(yytext); return T_float_expr; }

\'{CHAR}\' { yylval.str_expr = strdup(yytext); return T_char_expr; }

\"{CHAR}*\" { yylval.str_expr = strdup(yytext); return T_str_expr; }

{D}+ { yylval.int_expr = atoi(yytext); return T_int_expr; }

"->"  { return T_arrow_op; }
"+."  { return T_plus_op; }
"-."  { return T_minus_op; }
"*."  { return T_mult_op; }
"/."  { return T_div_op; }
"**"  { return T_pow_op; }
"&&"  { return T_and_op; }
"||" { return T_or_op; }
"<>" { return T_struct_diff_op; }
"<=" { return T_leq_op; }
">=" { return T_geq_op; }
"==" { return T_eq_op; }
"!=" { return T_diff_op; }
":=" { return T_assign_op; }

\-\-.*  { /* nothing' */ }

"(*"  { BEGIN(COMMENT); commentno++; }
<COMMENT>"(*"  { commentno++; }
<COMMENT>"*)" { if (--commentno <= 0) BEGIN(INITIAL); }
<COMMENT>\n { lineno++; }
<COMMENT>"*"  { /* nothing */ }
<COMMENT>[^*\n]  { /* nothing */ }

[()\[\],:=|+\-*\/!;<>]  { return yytext[0]; }

{W}+    { /* nothing */ }
\n  { lineno++; }

. { yyerror("Illegal character"); }

%%
/*
#ifdef yyFlexLexer
int main () {
  yyFlexLexer lexer;
  int token;
  do {
    token = lexer.yylex();
    printf("token=%d, lexeme=\"%s\"\n", token, lexer.YYText());
  } while (token != YYEOF);
}
#else
int main () {
  int token;
  do {
    token = yylex();
    printf("token=%d, lexeme=\"%s\"\n", token, yytext);
  } while (token != YYEOF);
  if (commentno) printf("error\n");
}
#endif
*/
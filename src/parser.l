%option nodefault noyywrap

%{
#include "list.h"
#include "parser.tab.h"
#define TRUE 1
#define FALSE 0
%}

%%
"letrec" { return LETREC; }
"#t" { return BOOL_TRUE; }
"#f" { return BOOL_FALSE; }
"set!" { return SETBANG; }
"lambda" { return LAMBDA; }
"if" { return IF; }
"begin" { return BEGN; }
"quote" { return QUOTE; }
"primcall" { return PRIMCALL; }
"("  { return LPAR; }
")"  { return RPAR; }
[\*\+\-] { yylval.str = strdup(yytext); return VPRIM; }
[<>]=? { yylval.str = strdup(yytext); return PPRIM; }
"=" { yylval.str = strdup(yytext); return PPRIM; }
[0-9]+ { yylval.integer = atoi(yytext); return INTEGER; }
[a-zA-Z][a-zA-Z0-9]* { yylval.str = strdup(yytext) ; return VARIABLE; }
[ \t\n\r] { /* ignore whitespace */ }
. { printf("bad input character: '%s' at line %d\n", yytext, yylineno); }
%%

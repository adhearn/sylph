%option nodefault noyywrap

%{
#include "parser.tab.h"
#define TRUE 1
#define FALSE 0
%}

%%
"program" { return PROGRAM; }
"return" { return RETURN; }
"#t" { yylval.integer = TRUE; return BOOLEAN; }
"#f" { yylval.integer = FALSE; return BOOLEAN; }
"set!" { return SETBANG; }
[\*\+\-] { yylval.str = strdup(yytext); return OPERATOR; }
"("  { return LPAR; }
")"  { return RPAR; }
[0-9]+ { yylval.integer = atoi(yytext); return NUMBER; }
[a-zA-Z][a-zA-Z0-9]* { yylval.str = strdup(yytext) ; return IDENTIFIER; }
[ \t\n\r] { /* ignore whitespace */ }
. { printf("bad input character: '%s' at line %d\n", yytext, yylineno); }
%%

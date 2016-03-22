%{
#include <stdio.h>
#include "pasm.h"
%}

%union {
    struct Program *prog;
    struct Pstmt *pstmt;
    struct Pstmt_l *pstmt_l;
    struct Value *value;
    struct Binop *binop;
    struct Rvalue *rvalue;
    struct Retval *retval;
    char *str;
    int integer;
}

%start program
%token PROGRAM
%token RETURN
%token BOOLEAN NUMBER
%token IDENTIFIER
%token OPERATOR
%token SETBANG
%token LPAR RPAR

%type   <prog>          program
%type   <pstmt>         pasm-stmt
%type   <pstmt_l>       pasm-list
%type   <value>         val
%type   <rvalue>        rval
%type   <retval>        retval
%type   <str>           OPERATOR IDENTIFIER
%type   <integer>       NUMBER BOOLEAN

%%
program:        LPAR PROGRAM retval RPAR { printf("PROGRAM (no pasm-list)\n"); $$ = make_program(NULL, $3); }
        |       LPAR PROGRAM pasm-list retval RPAR { printf("PROGRAM\n"); $$ = make_program($3, $4); }
;

pasm-list:  pasm-stmt { printf("pasm-list\n"); $$ = make_pstmt_l($1); }
/*               |   pasm-stmt pasm-list { printf("pasm-list\n"); struct Pstmt_l *ls = make_pstmt_l($1); ls->next = $2; $$ = ls; } */
|   pasm-list pasm-stmt { printf("pasm-list\n"); struct Pstmt_l *ls = make_pstmt_l($2); $1->next = ls; $$ = $1; }
;

retval:         LPAR RETURN val RPAR { printf("retval\n"); }
;

pasm-stmt: LPAR SETBANG val rval RPAR { printf("pasm-stmt\n"); $$ = make_pstmt($3, $4); }
;

rval: val { printf("rval\n"); $$ = make_value_rvalue($1); }
|     LPAR OPERATOR val val RPAR { printf("rval binop\n"); $$ = make_binop_rvalue(make_binop($2, $3, $4));}
        ;

val:    NUMBER { printf("NUMBER\n"); $$ = make_integer_value($1);}
|       BOOLEAN { printf("BOOLEAN\n"); $$ = make_boolean_value($1);}
|       IDENTIFIER { printf("IDENTIFIER\n"); $$ = make_identifier_value($1);}
;

%%
int main(int argc, char **argv) {
    yyparse();
    return 0;
}

int yyerror (char *s) {
    fprintf(stderr, "error: %s\n", s);
    return 1;
}

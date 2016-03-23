%{
#include <stdio.h>
#include "pasm.h"
#include "compiler.h"
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

//%parse-param {struct Program *prog}

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
program:        LPAR PROGRAM retval RPAR { $$ = make_program(NULL, $3);; }
        |       LPAR PROGRAM pasm-list retval RPAR { $$ = make_program($3, $4); prog = $$;}
;

pasm-list:  pasm-stmt {  $$ = make_pstmt_l($1); }
        |       pasm-list pasm-stmt { struct Pstmt_l *ls = make_pstmt_l($2);
     Pstmt_l_append($1, ls);
     $$ = $1; }
;

retval:         LPAR RETURN val RPAR { /*printf("retval\n");*/ $$ = make_retval($3);}
;

pasm-stmt: LPAR SETBANG val rval RPAR { /*printf("pasm-stmt\n");*/ $$ = make_pstmt($3, $4); }
;

rval: val { /*printf("rval\n");*/ $$ = make_value_rvalue($1); }
          |     LPAR OPERATOR val val RPAR { /*printf("rval binop\n");*/ $$ = make_binop_rvalue(make_binop($2, $3, $4));}
        ;

val:            NUMBER { /*printf("NUMBER\n");*/ $$ = make_integer_value($1);}
        |       BOOLEAN { /*printf("BOOLEAN\n");*/ $$ = make_boolean_value($1);}
        |       IDENTIFIER { /*printf("IDENTIFIER\n");*/ $$ = make_identifier_value($1); }
;

%%

int yyerror (char *s) {
    fprintf(stderr, "error: %s\n", s);
    return 1;
}

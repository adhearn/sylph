%{
#include <stdio.h>
#include "list.h"
#include "sil.h"
#define YYDEBUG 1
%}

%union {
    struct Program *prog;
    struct Function *function;
    struct Function_list *function_list;
    struct SimpleValue *svalue;
    struct Value *value;
    struct Predicate *predicate;
    struct Effect *effect;
    List *list;
    char *str;
    int integer;
}

//%parse-param {struct Program *prog}

%start program
%token LETREC LAMBDA SETBANG IF BEGN PRIMCALL QUOTE
%token BOOL_TRUE BOOL_FALSE INTEGER
%token VARIABLE
%token VPRIM EPRIM PPRIM
%token LPAR RPAR

%type   <prog>          program
%type   <function>      l-expr
%type   <function_list> fn-list
%type   <svalue>        simple-value boolean constant
%type   <value>         value
%type   <effect>        effect
%type   <predicate>     predicate
%type   <list>          effect-list value-list arg-list
%type   <str>           VARIABLE VPRIM EPRIM PPRIM
%type   <integer>       INTEGER

%%
program:        LPAR LETREC LPAR fn-list RPAR value RPAR {
    printf("Program parsed\n");
                    $$ = make_Program($4, $6); }
;

fn-list: /* Empty */ { $$ = NULL; printf("Empty fn-list\n"); }
        |       fn-list LPAR VARIABLE l-expr RPAR {
            //struct Function_list *ls = make_Function_list($3, $4);
            $$ = Function_list_append($1, $3, $4); }
;

l-expr: LPAR LAMBDA LPAR arg-list RPAR value RPAR {
    $$ = make_Function($4, $6); }
;

arg-list: /* Empty */ { $$ = List_new(free); }
        |       arg-list VARIABLE { List_append($1, $2); $$ = $1; }
;

value:  LPAR IF predicate value value RPAR { $$ = make_Value_if($3, $4, $5); }
|       LPAR BEGN effect-list value RPAR { $$ = make_Value_begin($3, $4); }
|       LPAR PRIMCALL VPRIM  value-list RPAR { $$ = make_Value_primcall($3, $4); }
|       LPAR value value-list RPAR { $$ = make_Value_funcall($2, $3); }
|       simple-value { $$ = make_Value_simple($1); printf("Simple value\n");}
;

predicate:      LPAR IF predicate predicate predicate RPAR {
    $$ = make_Predicate_if($3, $4, $5); }
        |       LPAR BEGN effect-list predicate RPAR {
    $$ = make_Predicate_begin($3, $4); }
        |       LPAR PRIMCALL PPRIM value-list RPAR {
    $$ = make_Predicate_primcall($3, $4); }
                ;

effect:         LPAR SETBANG VARIABLE value RPAR {
    $$ = make_Effect_set($3, $4); }
        |       LPAR IF predicate effect effect RPAR {
    $$ = make_Effect_if($3, $4, $5); }
        |       LPAR BEGN effect-list effect RPAR {
    $$ = make_Effect_begin($3, $4); }
        |       LPAR PRIMCALL EPRIM value-list RPAR {
    $$ = make_Effect_primcall($3, $4); }
        ;

simple-value:   VARIABLE { $$ = make_SimpleValue_var($1); }
        |       LPAR QUOTE constant RPAR { $$ = $3; }
        ;

 constant:       INTEGER { $$ = make_SimpleValue_int($1); printf("Integer\n"); }
        |       boolean { $$ = $1; }
                ;

boolean:        BOOL_TRUE { $$ = make_SimpleValue_bool(TRUE); }
        |       BOOL_FALSE { $$ = make_SimpleValue_bool(FALSE); }
                ;

effect-list: /* Empty */ { $$ = List_new((freeFunction)free_Effect); }
|       effect-list effect { List_append($1, $2); $$ = $1; }
;

value-list: /* Empty */ { $$ = List_new((freeFunction)free_Value); }
|       value-list value { List_append($1, $2); $$ = $1; }
;

%%

int yyerror (char *s) {
    fprintf(stderr, "error: %s\n", s);
    return 1;
}

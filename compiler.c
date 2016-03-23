#include <stdlib.h>
#include <stdio.h>
#include "parser.tab.h"
#include "pasm.h"

struct Program *prog;

/* Returns a string representation of the value, as is used in our
 * target assembly language.
 * e.g. if value is the integer 0, this function will return "$0".
 * Similarly, if it's the register RAX, this function should return
 * "%rax".
 */
int Value_asm_str(struct Value *value, char *str) {

    switch (value->type) {
    case BOOLEAN_T:
        return sprintf(str, "$%d", value->value.ival);
        break;
    case INTEGER_T:
        return sprintf(str, "$%d", value->value.ival);
        break;
    case IDENTIFIER_T:
        return sprintf(str, "%%%s", value->value.sval);
        break;
    };

    return -1;
}

void emit_Pstmt(struct Pstmt *pstmt) {
    char *str = calloc(32, 1);
    struct Rvalue *rval = pstmt->rvalue;

    if (rval->type == VALUE_T) {
        Value_asm_str(rval->value.value, str);
        printf("movq %s, %%%s\n", str, pstmt->identifier);
    }
    else if(rval->type == BINOP_T) {
        struct Binop *binop = rval->value.binop;
        char *rator = binop->rator;
        Value_asm_str(binop->val2, str);
        char *op;
        if (strcmp(rator, "+") == 0) {
            op = "add";
        } else if (strcmp(rator, "-") == 0) {
            op = "sub";
        } else if (strcmp(rator, "*") == 0) {
            op = "imul";
        } else {
            fprintf(stderr, "Invalid operator: %s\n", rator);
        }
        printf("%s %s, %%%s\n", op, str, pstmt->identifier);
    }

    free(str);
}

void emit_Pstmt_l(struct Pstmt_l *ls) {
    while (ls->next != NULL) {
        emit_Pstmt(ls->pstmt);
        ls = ls->next;
    }
    emit_Pstmt(ls->pstmt);
}

/*
 * This function is a bit unique, because we actually have to
 * move the value into the RAX register. It's one of the only
 * instructions we have that does something other than a direct
 * translation of our current source language
 */
void emit_Retval(struct Retval *retval) {
    char *str = calloc(32, 1);
    Value_asm_str(retval->value, str);
    printf("movq %s, %%rax\n", str);
    free(str);
}

void emit_x86_64(struct Program *prog) {
    printf(".text\n");
    printf(".globl main\n");
    printf("main:\n");
    printf("pushq %%rbp\n");
    printf("movq %%rsp, %%rbp\n");

    emit_Pstmt_l(prog->body);
    emit_Retval(prog->retval);

    printf("popq %%rbp\n");
    printf("ret\n");
}

int main(int argc, char *argv[]) {
  prog = (struct Program *)malloc(sizeof(struct Program));
  int res = yyparse();

  if (res != 0) {
    fprintf(stderr, "Error while parsing\n");
  }
  //print_program(prog);

  emit_x86_64(prog);

  return 0;
}

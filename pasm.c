#include <stdio.h>
#include <stdlib.h>
#include "pasm.h"

struct Value *make_value() {
    struct Value *value = malloc(sizeof(struct Value));
    if (value == NULL) {
        fprintf(stderr, "Error in make_value: out of memory");
        exit(1);
    }
    return value;
}

struct Value *make_integer_value(int val) {
    struct Value *value = make_value();
    value->type = INTEGER_T;
    value->value.ival = val;
    return value;
}

struct Value *make_boolean_value(int val) {
    struct Value *value = make_value();
    value->type = BOOLEAN_T;
    value->value.ival = val;
    return value;
}

struct Value *make_identifier_value(char *val) {
    struct Value *value = make_value();
    value->type = IDENTIFIER_T;
    value->value.sval = val;
    return value;
}

void free_value(struct Value *val) {
    if (val->type == IDENTIFIER_T) {
        free(val->value.sval);
    }
    free(val);
}

struct Binop *make_binop(char *rator, struct Value *val1, struct Value *val2) {
    struct Binop *binop = malloc(sizeof(struct Binop));
    if (binop == NULL) {
        fprintf(stderr, "Error in make_binop: out of memory");
        exit(1);
    }
    binop->rator = rator;
    binop->val1 = val1;
    binop->val2 = val2;
    return binop;
}

void free_binop(struct Binop *binop) {
    free(binop->rator);
    free_value(binop->val1);
    free_value(binop->val2);
    free(binop);
}

struct Rvalue *make_rvalue() {
    struct Rvalue *rval = malloc(sizeof(struct Rvalue));
    if (rval == NULL) {
        fprintf(stderr, "Error in make_rvalue: out of memory");
        exit(1);
    }
    return rval;
}

struct Rvalue *make_binop_rvalue(struct Binop *binop) {
    struct Rvalue *rval = make_rvalue();
    rval->type = BINOP_T;
    rval->value.binop = binop;
    return rval;
}

struct Rvalue *make_value_rvalue(struct Value *value) {
    struct Rvalue *rval = make_rvalue();
    rval->type = VALUE_T;
    rval->value.value = value;
    return rval;
}

void free_rvalue(struct Rvalue *rval) {
    if (rval->type == BINOP_T) {
        free_binop(rval->value.binop);
    } else if (rval->type == VALUE_T) {
        free_value(rval->value.value);
    }

    free(rval);
}

struct Retval *make_retval(struct Value *value) {
    struct Retval *retval = malloc(sizeof(struct Retval));
    if (retval == NULL) {
        fprintf(stderr, "Error in make_retvalue: out of memory");
        exit(1);
    }
    retval->value = value;
    return retval;
}

void free_retval(struct Retval *retval) {
    free_value(retval->value);
    free(retval);
}

struct Pstmt *make_pstmt(struct Value *value, struct Rvalue *rval) {
    struct Pstmt *pstmt = malloc(sizeof(struct Pstmt));
    if (pstmt == NULL) {
        fprintf(stderr, "Error in make_pstmt: out of memory");
        exit(1);
    }

    if (value->type != IDENTIFIER_T) {
        fprintf(stderr, "First argument to make_pstmt must be an identifier\n");
        exit(2);
    }
    pstmt->identifier = value->value.sval;
    pstmt->rval = rval;
    return pstmt;
}

void free_pstmt(struct Pstmt *pstmt) {
    free(pstmt->identifier);
    free_rvalue(pstmt->rval);
    free(pstmt);
}

struct Pstmt_l *make_pstmt_l(struct Pstmt *pstmt) {
    struct Pstmt_l *ls = malloc(sizeof(struct Pstmt_l));
    if (ls == NULL) {
        fprintf(stderr, "Error in make_pstmt_l: out of memory");
        exit(1);
    }
    ls->pstmt = pstmt;
    ls->next = NULL;
    return ls;
}

void Pstmt_l_append(struct Pstmt_l *ls, struct Pstmt_l *pstmt) {
  while (ls->next != NULL) {
    ls = ls->next;
  }

  ls->next = pstmt;
}

void free_pstmt_l(struct Pstmt_l *ls) {
    if (ls->next != NULL) {
        free_pstmt_l(ls->next);
    }
    free_pstmt(ls->pstmt);
    free(ls);
}

struct Program *make_program(struct Pstmt_l *body, struct Retval *retval) {
    struct Program *program = malloc(sizeof(struct Program));
    if (program == NULL) {
        fprintf(stderr, "Error in make_program: out of memory");
        exit(1);
    }
    program->body = body;
    program->retval = retval;
    return program;
}

void print_value(struct Value *value) {
  switch (value->type) {
  case BOOLEAN_T:
    0;
    char *str = value->value.ival ? "#t" : "#f";
    printf("%s", str);
    break;
  case INTEGER_T:
    printf("%d", value->value.ival);
    break;
  case IDENTIFIER_T:
    printf("%s", value->value.sval);
    break;
  }
}

void print_binop(struct Binop *binop) {
  printf("(%s ", binop->rator);
  print_value(binop->val1);
  printf(" ");
  print_value(binop->val2);
  printf(")");
}

void print_rvalue(struct Rvalue *rvalue) {
  if (rvalue->type == BINOP_T) {
    print_binop(rvalue->value.binop);
  } else {
    print_value(rvalue->value.value);
  }
}

void print_pstmt(struct Pstmt *pstmt) {
  printf("SET! %s ", pstmt->identifier);
  print_rvalue(pstmt->rval);
}

void print_retval(struct Retval *retval) {
  printf("RETURN ");
  print_value(retval->value);
}

void print_program(struct Program *prog) {
  printf("PROGRAM\n");
  struct Pstmt_l *ls = prog->body;

  while(ls != NULL) {
    print_pstmt(ls->pstmt);
    printf("\n");
    ls = ls->next;
  }

  print_retval(prog->retval);
  printf("\n");
}

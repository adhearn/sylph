#ifndef _PASM_H
#define _PASM_H

enum value_t {
    BOOLEAN_T,
    INTEGER_T,
    IDENTIFIER_T
};

struct Value {
    enum value_t type;
    union value_values {
        int ival;
        char *sval;
    } value;
};

struct Binop {
    char *rator;
    struct Value *val1;
    struct Value *val2;
};

enum rvalue_t {
    BINOP_T,
    VALUE_T
};

struct Rvalue {
    enum rvalue_t type;
    union rvalue_values {
        struct Value *value;
        struct Binop *binop;
    } value;
};

struct Retval {
    struct Value *value;
};

struct Pstmt {
    char *identifier;
    struct Rvalue *rval;
};

struct Pstmt_l {
    struct Pstmt *pstmt;
    struct Pstmt_l *next;
};


struct Program {
    struct Pstmt_l *body;
    struct Retval *retval;
};

struct Value *make_integer_value(int val);
struct Value *make_boolean_value(int val);
struct Value *make_identifier_value(char *val);
void free_value(struct Value *val);
struct Binop *make_binop(char *rator, struct Value *val1, struct Value *val2);
void free_binop(struct Binop *binop);
struct Rvalue *make_binop_rvalue(struct Binop *binop);
struct Rvalue *make_value_rvalue(struct Value *value);
void free_rvalue(struct Rvalue *rval);
struct Retval *make_retval(struct Value *value);
void free_retval(struct Retval *retval);
struct Pstmt *make_pstmt(struct Value *value, struct Rvalue *rval);
void free_pstmt(struct Pstmt *pstmt);
struct Pstmt_l *make_pstmt_l(struct Pstmt *pstmt);
void free_pstmt_l(struct Pstmt_l *ls);
struct Program *make_program(struct Pstmt_l *body, struct Retval *retval);
void print_program(struct Program *prog);
#endif

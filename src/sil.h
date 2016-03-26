#include "list.h"

struct Program {
    struct Function_list *functions;
    struct Value *body;
};

struct Function_list {
    struct Function *function_head;
    char *fun_names_head;
    struct Function_list *next;
};

struct Function {
    List *arg_list;
    struct Value *body;
};

/* The fact I have to declare 3 of these seems a bit painful and wasteful.
 * I'm not a good enough C programmer to know a better way, though
 * Actually, I thought about it for 3 seconds and fixed it.
 */
/*
struct Argument_list {
    char *arg;
    struct Argument_list *next;
};
*/

enum bool {
    FALSE,
    TRUE
};

enum SimpleValue_t {
    SV_VAR,
    SV_INTEGER,
    SV_BOOLEAN
};

struct SimpleValue {
    enum SimpleValue_t type;
    union {
        char *var;
        int integer;
        enum bool boolean;
    } val;
};

enum Value_t {
    V_IF,
    V_BEGIN,
    V_PRIMCALL,
    V_FUNCALL,
    V_SIMPLE
};

struct Value_if {
    struct Predicate *pred;
    struct Value *value1;
    struct Value *value2;
};

struct Value_begin {
    List *effect_list;
    struct Value *value;
};

struct Value_primcall {
    char *vprim;
    List *value_list;
};

struct Value_funcall {
    struct Value *rator;
    List *rand_list;
};

struct Value {
    enum Value_t type;
    union {
        struct Value_if *vif;
        struct Value_begin *vbegin;
        struct Value_primcall *vprimcall;
        struct Value_funcall *vfuncall;
        struct SimpleValue *sval;
    } val;
};

enum Predicate_t {
    P_IF,
    P_BEGIN,
    P_PRIMCALL
};

struct Predicate_if {
    struct Predicate *pred1;
    struct Predicate *pred2;
    struct Predicate *pred3;
};

struct Predicate_begin {
    List *effect_list;
    struct Predicate *pred;
};

struct Predicate_primcall {
    char *pprim;
    List *value_list;
};

struct Predicate {
    enum Predicate_t type;
    union {
        struct Predicate_if *pif;
        struct Predicate_begin *pbegin;
        struct Predicate_primcall *pprimcall;
    } pred;
};

enum Effect_t {
    E_SET,
    E_IF,
    E_BEGIN,
    E_PRIMCALL
};

struct Effect_set {
    char *var;
    struct Value *rhs;
};

struct Effect_if {
    struct Predicate *pred;
    struct Effect *effect1;
    struct Effect *effect2;
};

struct Effect_begin {
    List *effect_list;
    struct Effect *effect;
};

struct Effect_primcall {
    char *eprim;
    List *value_list;
};

struct Effect {
    enum Effect_t type;
    union {
        struct Effect_set *eset;
        struct Effect_if *eif;
        struct Effect_begin *ebegin;
        struct Effect_primcall *eprimcall;
    } effect;
};

/*
struct Effect_list {
    struct Effect *effect;
    struct Effect_list *next;
};

stnruct Value_list {
    struct Value *value;
    struct Value_list *next;
};
*/

struct Program *make_program(struct Function_list *fns, struct Value *body);
void free_Program(struct Program *prog);
struct Function *make_Function(List *args, struct Value *body);
void free_Function(struct Function *fn);
struct Value *make_Value_if(struct Predicate *pred, struct Value *value1, struct Value *value2);
void free_Value_if(struct Value_if *vif);
struct Value *make_Value_begin(List *effect_list, struct Value *value1);
void free_Value_begin(struct Value_begin *vbegin);
struct Value *make_Value_primcall(char *vprim, List *vlist);
void free_Value_primcall(struct Value_primcall *vprimcall);
struct Value *make_Value_funcall(struct Value *rator, List *rand_list);
void free_Value_funcall(struct Value_funcall *vfuncall);
struct Value *make_Value_simple(struct SimpleValue *sval);
struct SimpleValue *make_SimpleValue_var(char *var);
struct SimpleValue *make_SimpleValue_int(int integer);
struct SimpleValue *make_SimpleValue_bool(enum bool val);
void free_SimpleValue(struct SimpleValue *sval);
struct Effect *make_Effect_set(char *var, struct Value *rhs);
void free_Effect_set(struct Effect_set *eset);
struct Effect *make_Effect_if(struct Predicate *pred, struct Effect *effect1, struct Effect *effect2);
void free_Effect_if(struct Effect_if *eif);
struct Effect *make_Effect_begin(List *effect_list, struct Effect *effect1);
void free_Effect_begin(struct Effect_begin *ebegin);
struct Effect *make_Effect_primcall(char *eprim, List *value_list);
void free_Effect_primcall(struct Effect_primcall *eprimcall);
struct Predicate *make_Predicate_if(struct Predicate *pred1, struct Predicate *pred2, struct Predicate *pred3);
void free_Predicate_if(struct Predicate_if *pif);
struct Predicate *make_Predicate_begin(List *effect_list, struct Predicate *pred1);
void free_Predicate_begin(struct Predicate_begin *pbegin);
struct Predicate *make_Predicate_primcall(char *pprim, List *value_list);
void free_Predicate_primcall(struct Predicate_primcall *pprimcall);
void free_Value(struct Value *value);
void free_Effect(struct Effect *effect);
void free_Predicate(struct Predicate *predicate);
/*
struct Argument_list *make_Argument_list(char *var);
struct Argument_list *Argument_list_append(struct Argument_list *arglist, char *var);
void free_Argument_list(struct Argument_list *arglist);
*/
struct Function_list *make_Function_list(char *var, struct Function *fn);
struct Function_list *Function_list_append(struct Function_list *fn_list, char *var, struct Function *fn);
void free_Function_list(struct Function_list *fn_list);

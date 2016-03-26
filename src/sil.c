/* Support functions for our Simple Intermediate Language (SIL). */
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "sil.h"
#include "util.h"

struct Program *make_Program(struct Function_list *fns, struct Value *body) {
    struct Program *prog = malloc(sizeof(struct Program));
    check_mem(prog);

    prog->functions = fns;
    prog->body = body;
    return prog;
}

void free_Program(struct Program *prog) {
    free_Value(prog->body);
    free_Function_list(prog->functions);
    free(prog);
}

struct Function *make_Function(List *args, struct Value *body) {
    struct Function *fn = malloc(sizeof(struct Function));
    check_mem(fn);

    fn->arg_list = args;
    fn->body = body;
    return fn;
}

void free_Function(struct Function *fn) {
    List_free(fn->arg_list);
    free_Value(fn->body);
    free(fn);
}

struct SimpleValue *make_SimpleValue_var(char *var) {
    struct SimpleValue *sval = malloc(sizeof(struct SimpleValue));
    check_mem(sval);

    sval->type = SV_VAR;
    sval->val.var = var;
    return sval;
}

struct SimpleValue *make_SimpleValue_int(int integer) {
    struct SimpleValue *sval = malloc(sizeof(struct SimpleValue));
    check_mem(sval);

    sval->type = SV_INTEGER;
    sval->val.integer = integer;
    return sval;
}

struct SimpleValue *make_SimpleValue_bool(enum bool val) {
    struct SimpleValue *sval = malloc(sizeof(struct SimpleValue));
    check_mem(sval);

    sval->type = SV_BOOLEAN;
    sval->val.boolean = val;
    return sval;
}

void free_SimpleValue(struct SimpleValue *sval) {
    switch (sval->type) {
    case SV_VAR:
        free(sval->val.var);
        break;
    }; // So far, no other types need to be handled

    free(sval);
}

struct Value *make_Value_if(struct Predicate *pred, struct Value *value1, struct Value *value2) {
    struct Value *value = malloc(sizeof(struct Value));
    check_mem(value);
    struct Value_if *vif = malloc(sizeof(struct Value_if));
    check_mem(vif);

    vif->pred = pred;
    vif->value1 = value1;
    vif->value2 = value2;
    value->type = V_IF;
    value->val.vif = vif;
    return value;
}

void free_Value_if(struct Value_if *vif) {
    free_Predicate(vif->pred);
    free_Value(vif->value1);
    free_Value(vif->value2);
    free(vif);
}

struct Value *make_Value_begin(List *effect_list, struct Value *value1) {
    struct Value *value = malloc(sizeof(struct Value));
    check_mem(value);
    struct Value_begin *vbegin = malloc(sizeof(struct Value_begin));
    check_mem(vbegin);

    vbegin->effect_list = effect_list;
    vbegin->value = value1;
    value->type = V_BEGIN;
    value->val.vbegin = vbegin;
    return value;
}

void free_Value_begin(struct Value_begin *vbegin) {
    List_free(vbegin->effect_list);
    free_Value(vbegin->value);
    free(vbegin);
}

struct Value *make_Value_primcall(char *vprim, List *vlist) {
    struct Value *value = malloc(sizeof(struct Value));
    check_mem(value);
    struct Value_primcall *vprimcall = malloc(sizeof(struct Value_primcall));
    check_mem(vprimcall);

    vprimcall->vprim = vprim;
    vprimcall->value_list = vlist;
    value->val.vprimcall = vprimcall;
    return value;
}

void free_Value_primcall(struct Value_primcall *vprimcall) {
    free(vprimcall->vprim);
    List_free(vprimcall->value_list);
    free(vprimcall);
}

struct Value *make_Value_funcall(struct Value *rator, List *rand_list) {
    struct Value *value = malloc(sizeof(struct Value));
    check_mem(value);
    struct Value_funcall *vfuncall = malloc(sizeof(struct Value_funcall));
    check_mem(vfuncall);

    vfuncall->rator = rator;
    vfuncall->rand_list = rand_list;
    value->val.vfuncall = vfuncall;
    return value;
}

void free_Value_funcall(struct Value_funcall *vfuncall) {
    free_Value(vfuncall->rator);
    List_free(vfuncall->rand_list);
    free(vfuncall);
}

struct Value *make_Value_simple(struct SimpleValue *sval) {
    struct Value *value = malloc(sizeof(struct Value));
    check_mem(value);

    value->type = V_SIMPLE;
    value->val.sval = sval;
    return value;
}

struct Effect *make_Effect_set(char *var, struct Value *rhs) {
    struct Effect *effect = malloc(sizeof(struct Effect));
    check_mem(effect);
    struct Effect_set *eset = malloc(sizeof(struct Effect_set));
    check_mem(eset);

    eset->var = var;
    eset->rhs = rhs;
    effect->effect.eset = eset;
    return effect;
}

void free_Effect_set(struct Effect_set *eset) {
    free(eset->var);
    free_Value(eset->rhs);
    free(eset);
}

struct Effect *make_Effect_if(struct Predicate *pred, struct Effect *effect1, struct Effect *effect2) {
    struct Effect *effect = malloc(sizeof(struct Effect));
    check_mem(effect);
    struct Effect_if *eif = malloc(sizeof(struct Effect_if));
    check_mem(eif);

    eif->pred = pred;
    eif->effect1 = effect1;
    eif->effect2 = effect2;
    effect->effect.eif = eif;
    return effect;
}

void free_Effect_if(struct Effect_if *eif) {
    free_Predicate(eif->pred);
    free_Effect(eif->effect1);
    free_Effect(eif->effect2);
    free(eif);
}

struct Effect *make_Effect_begin(List *effect_list, struct Effect *effect1) {
    struct Effect *effect = malloc(sizeof(struct Effect));
    check_mem(effect);
    struct Effect_begin *ebegin = malloc(sizeof(struct Effect_begin));
    check_mem(ebegin);

    ebegin->effect_list = effect_list;
    ebegin->effect = effect1;
    effect->effect.ebegin = ebegin;
    return effect;
}

void free_Effect_begin(struct Effect_begin *ebegin) {
    List_free(ebegin->effect_list);
    free_Effect(ebegin->effect);
    free(ebegin);
}

struct Effect *make_Effect_primcall(char *eprim, List *value_list) {
    struct Effect *effect = malloc(sizeof(struct Effect));
    check_mem(effect);
    struct Effect_primcall *eprimcall = malloc(sizeof(struct Effect_primcall));
    check_mem(eprimcall);

    eprimcall->eprim = eprim;
    eprimcall->value_list = value_list;
    effect->effect.eprimcall = eprimcall;
    return effect;
}

void free_Effect_primcall(struct Effect_primcall *eprimcall) {
    free(eprimcall->eprim);
    List_free(eprimcall->value_list);
    free(eprimcall);
}

struct Predicate *make_Predicate_if(struct Predicate *pred1, struct Predicate *pred2, struct Predicate *pred3) {
    struct Predicate *predicate = malloc(sizeof(struct Predicate));
    check_mem(predicate);
    struct Predicate_if *pif = malloc(sizeof(struct Predicate_if));
    check_mem(pif);

    pif->pred1 = pred1;
    pif->pred2 = pred2;
    pif->pred3 = pred3;
    predicate->pred.pif = pif;
    return predicate;
}

void free_Predicate_if(struct Predicate_if *pif) {
    free_Predicate(pif->pred1);
    free_Predicate(pif->pred2);
    free_Predicate(pif->pred3);
    free(pif);
}

struct Predicate *make_Predicate_begin(List *effect_list, struct Predicate *pred1) {
    struct Predicate *predicate = malloc(sizeof(struct Predicate));
    check_mem(predicate);
    struct Predicate_begin *pbegin = malloc(sizeof(struct Predicate_begin));
    check_mem(pbegin);

    pbegin->effect_list = effect_list;
    pbegin->pred = pred1;
    predicate->pred.pbegin = pbegin;
    return predicate;
}

void free_Predicate_begin(struct Predicate_begin *pbegin) {
    List_free(pbegin->effect_list);
    free_Predicate(pbegin->pred);
    free(pbegin);
}

struct Predicate *make_Predicate_primcall(char *pprim, List *value_list) {
    struct Predicate *predicate = malloc(sizeof(struct Predicate));
    check_mem(predicate);
    struct Predicate_primcall *pprimcall = malloc(sizeof(struct Predicate_primcall));
    check_mem(pprimcall);

    pprimcall->pprim = pprim;
    pprimcall->value_list = value_list;
    predicate->pred.pprimcall = pprimcall;
    return predicate;
}

void free_Predicate_primcall(struct Predicate_primcall *pprimcall) {
    free(pprimcall->pprim);
    List_free(pprimcall->value_list);
    free(pprimcall);
}

void free_Value(struct Value *value) {
    switch (value->type) {
    case (V_IF):
        free_Value_if(value->val.vif);
        break;
    case (V_BEGIN):
        free_Value_begin(value->val.vbegin);
        break;
    case (V_PRIMCALL):
        free_Value_primcall(value->val.vprimcall);
        break;
    case (V_FUNCALL):
        free_Value_funcall(value->val.vfuncall);
        break;
    case (V_SIMPLE):
        free_SimpleValue(value->val.sval);
        break;
    }

    free(value);
}

void free_Effect(struct Effect *effect) {
    switch(effect->type) {
    case (E_SET):
        free_Effect_set(effect->effect.eset);
        break;
    case (E_IF):
        free_Effect_if(effect->effect.eif);
        break;
    case (E_BEGIN):
        free_Effect_begin(effect->effect.ebegin);
        break;
    case (E_PRIMCALL):
        free_Effect_primcall(effect->effect.eprimcall);
        break;
    }

    free(effect);
}

void free_Predicate(struct Predicate *predicate) {
    switch(predicate->type) {
    case (P_IF):
        free_Predicate_if(predicate->pred.pif);
        break;
    case (P_BEGIN):
        free_Predicate_begin(predicate->pred.pbegin);
        break;
    case (P_PRIMCALL):
        free_Predicate_primcall(predicate->pred.pprimcall);
        break;
    }

    free(predicate);
}

/*
struct Argument_list *make_Argument_list(char *var) {
    struct Argument_list *arglist = malloc(sizeof(struct Argument_list));
    check_mem(arglist);

    arglist->arg = var;
    arglist->next = NULL;
    return arglist;
}

struct Argument_list *Argument_list_append(struct Argument_list *arglist, char *var) {
    struct Argument_list *new_arg_list = make_Argument_list(var);
    struct Argument_list *p = arglist;
    if (arglist == NULL) {
        return new_arg_list;
    } else {
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = new_arg_list;
        return arglist;
    }
}

void free_Argument_list(struct Argument_list *arglist) {
    if (arglist != NULL) {
        free_Argument_list(arglist->next);
        free(arglist);
    }
}
*/
struct Function_list *make_Function_list(char *var, struct Function *fn) {
    struct Function_list *fn_list = malloc(sizeof(struct Function_list));
    check_mem(fn_list);

    fn_list->fun_names_head = var;
    fn_list->function_head = fn;
    fn_list->next = NULL;
    return fn_list;
}

struct Function_list *Function_list_append(struct Function_list *fn_list, char *var, struct Function *fn) {
    struct Function_list *new_list = make_Function_list(var, fn);
    if (fn_list == NULL) {
        return new_list;
    } else {
        struct Function_list *p = fn_list;
        while(p->next != NULL) {
            p = p->next;
        }
        p->next = fn_list;
    }

    return fn_list;
}

void free_Function_list(struct Function_list *fn_list) {
    if (fn_list != NULL) {
        free_Function_list(fn_list->next);
        free(fn_list->fun_names_head);
        free_Function(fn_list->function_head);
        free(fn_list);
    }
}

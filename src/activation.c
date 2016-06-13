#include <stdlib.h>
#include "activation.h"

void activation_initialize(activation* act, activation* parent) {
    act->parent = parent;
    act->env = malloc(sizeof(hashtbl));
    hashtbl_initialize(act->env, scheme_symbol_hash_function, scheme_symbol_eq_function);
}

void activation_add_binding(activation* act, scheme_symbol binding, sexp value) {
    hashtbl_insert(act->env, (void*)binding, value);
}

bool activation_mutate_binding(activation* act, scheme_symbol binding, sexp value) {
    return hashtbl_update(act->env, (void*)binding, value);
}

bool activation_get_binding(activation* act, scheme_symbol binding, sexp* result) {
    activation* cursor = act;
    while (cursor) {
        if (hashtbl_lookup(cursor->env, (void*)binding, (void**)result)) {
            return true;
        }

        cursor = cursor->parent;
    }

    return false;
}

void activation_destroy(activation* act) {
    hashtbl_destroy(act->env);
    free(act->env);
}
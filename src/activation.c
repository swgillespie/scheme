#include <stdlib.h>
#include <assert.h>
#include "activation.h"

void activation_initialize(activation* act, activation* parent, scheme_symbol name) {
    assert(act != NULL);
    act->parent = parent;
    act->name = name;
    act->env = malloc(sizeof(hashtbl));
    hashtbl_initialize(act->env, scheme_symbol_hash_function, scheme_symbol_eq_function);
}

void activation_add_binding(activation* act, scheme_symbol binding, struct _sexp* value) {
    assert(act != NULL);
    hashtbl_insert(act->env, (void*)binding, value);
}

bool activation_mutate_binding(activation* act, scheme_symbol binding, struct _sexp* value) {
    assert(act != NULL);
    return hashtbl_update(act->env, (void*)binding, value);
}

bool activation_get_binding(activation* act, scheme_symbol binding, struct _sexp** result) {
    assert(act != NULL);
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
    assert(act != NULL);
    hashtbl_destroy(act->env);
    free(act->env);
}

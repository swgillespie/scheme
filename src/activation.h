#pragma once

#include "types.h"
#include "hashtbl.h"

typedef struct _sexp* sexp;

// An activation represents a "frame" in scheme.
// Activations will be, in the future heap allocated to facilitate call/cc.
typedef struct activation {
    struct activation* parent;
    hashtbl* env;
} activation;

// Initializes a new activation with a given parent activation.
void activation_initialize(activation* act, activation* parent);

// Adds a binding to the activation, introduced through 
// e.g. `let`, `lambda`, or `define`.
void activation_add_binding(activation* act, scheme_symbol binding, sexp value);

// Mutates a binding to in the activation, e.g. through `set!`. Returns
// false if no such binding exists, true if the rebinding was successful.
bool activation_mutate_binding(activation* act, scheme_symbol binding, sexp value);

// Retrieves a binding from the chain of activations, returning false if no such
// binding exists and returning true and setting the result out param if the binding
// does exist.
bool activation_get_binding(activation* act, scheme_symbol binding, sexp* result);

// Destroys an activation.
void activation_destroy(activation* act);
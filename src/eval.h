#pragma once

#include "sexp.h"
#include "activation.h"

// Initializes the interpreter.
void scheme_initialize(void);

// Evaluate an s-expression in the global activation.
sexp scheme_global_eval(sexp program);

// Evaluate an s-expression using the given activation.
sexp scheme_eval(sexp program, activation* activation);
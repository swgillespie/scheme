#pragma once

#include "sexp.h"
#include "activation.h"

// Initializes the interpreter.
void scheme_initialize(void);

// Evaluates a file.
void scheme_eval_file(const char* file);

// Evaluate an s-expression in the global activation.
sexp scheme_global_eval(sexp program);

// Evaluate an s-expression using the given activation.
sexp scheme_eval(sexp program, activation* activation);

// Prints a stack trace.
void scheme_print_stack_trace();

// Prints a message, prints a stack trace of executing scheme
// code, and aborts the process.
void scheme_runtime_error(scheme_string message_fmt, ...);
#pragma once

#include "sexp.h"
#include "activation.h"

// Garbage collector and allocator routines.
sexp gc_allocate_empty();
sexp gc_allocate_string(const char* value);
sexp gc_allocate_number(scheme_number number);
sexp gc_allocate_symbol(const char* sym);
sexp gc_allocate_bool(bool value);
sexp gc_allocate_cons(sexp car, sexp cdr);
activation* gc_allocate_activation();
sexp gc_allocate_proc(scheme_number arity, sexp args, sexp body, activation* activation);
sexp gc_allocate_native_proc(scheme_number arity, sexp (*function_pointer)(sexp*), const char* name);

// Forces a garbage collection.
void gc_collect(void);
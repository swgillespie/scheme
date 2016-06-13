#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "gc.h"
#include "util.h"

static bool empty_sexp_cleared;
static struct _sexp empty_sexp;

// Yeah, all of these functions just leak memory. I know.
// I'll fix it later. It's just like a GC that never collects
// right now.

sexp gc_allocate_empty() {
    // since there is only one empty list, a single
    // global value is used.
    if (!empty_sexp_cleared) {
        empty_sexp_cleared = true;
        memset(&empty_sexp, 0, sizeof(struct _sexp));
    }

    return &empty_sexp;
}

sexp gc_allocate_cons(sexp car, sexp cdr) {
    sexp s = malloc(sizeof(struct _sexp));
    if (!s) {
        PANIC("out of memory");
    }

    s->kind = SEXP_CONS;
    s->car = car;
    s->cdr = cdr;
    return s;
}

sexp gc_allocate_string(const char* value) {
    sexp s = malloc(sizeof(struct _sexp));
    if (!s) {
        PANIC("out of memory");
    }

    s->kind = SEXP_STRING;

    size_t len = strlen(value);
    if (len == SIZE_MAX) {
        PANIC("string too large");
    }

    char* buffer = malloc(len + 1);
    if (!buffer) {
        PANIC("out of memory");
    }

    strncpy(buffer, value, len + 1);
    s->string_value = buffer;
    return s;
}

sexp gc_allocate_number(scheme_number number) {
    sexp s = malloc(sizeof(struct _sexp));
    if (!s) {
        PANIC("out of memory");
    }

    s->kind = SEXP_NUMBER;
    s->number_value = number;
    return s;
}

sexp gc_allocate_symbol(const char* value) {
    sexp s = malloc(sizeof(struct _sexp));
    if (!s) {
        PANIC("out of memory");
    }

    s->kind = SEXP_SYMBOL;

    size_t len = strlen(value);
    if (len == SIZE_MAX) {
        PANIC("string too large");
    }

    char* buffer = malloc(len + 1);
    if (!buffer) {
        PANIC("out of memory");
    }

    strncpy(buffer, value, len + 1);
    s->string_value = buffer;
    return s;
}

sexp gc_allocate_bool(bool value) {
    sexp s = malloc(sizeof(struct _sexp));
    if (!s) {
        PANIC("out of memory");
    }

    s->kind = SEXP_BOOL;
    s->bool_value = value;
    return s;   
}

activation* gc_allocate_activation() {
    activation* act = malloc(sizeof(activation));
    if (!act) {
        PANIC("out of memory");
    }

    return act;
}

sexp gc_allocate_proc(scheme_number arity, sexp args, sexp body, activation* activation) {
    sexp s = malloc(sizeof(struct _sexp));
    if (!s) {
        PANIC("out of memory");
    }

    s->kind = SEXP_PROC;
    s->arity = arity;
    s->arguments = args;
    s->body = body;
    s->activation = activation;
    return s;
}

sexp gc_allocate_native_proc(scheme_number arity, sexp (*function_pointer)(sexp*), const char* name) {
    sexp s = malloc(sizeof(struct _sexp));
    if (!s) {
        PANIC("out of memory");
    }

    s->kind = SEXP_NATIVE_PROC;
    s->native_arity = arity;
    s->name = name;
    s->function_pointer = function_pointer;
    return s;
}

void gc_collect() {
    // do nothing
}
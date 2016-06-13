#pragma once

#include <stdio.h>
#include "types.h"


// A s-expression. This value is always used behind a pointer.
typedef struct _sexp {
    enum {
        SEXP_EMPTY,
        SEXP_CONS,
        SEXP_STRING,
        SEXP_NUMBER,
        SEXP_SYMBOL,
        SEXP_BOOL,
        SEXP_PROC,
        SEXP_NATIVE_PROC
    } kind;
    union {
        struct {
            struct _sexp* car;
            struct _sexp* cdr;
        };
        struct {
            scheme_number arity;
            struct _sexp* arguments;
            struct _sexp* body;
            struct activation* activation;
        };
        struct {
            const char* name;
            scheme_number native_arity;
            struct _sexp* (*function_pointer)(struct _sexp**);
        };
        scheme_string string_value;
        scheme_number number_value;
        scheme_symbol symbol_value;
        scheme_bool bool_value; 
    };
}* sexp;

static inline bool sexp_is_truthy(sexp s) {
    return !(s->kind == SEXP_BOOL && s->bool_value == false);
}

static inline bool sexp_is_empty(sexp s) {
    return s->kind == SEXP_EMPTY;
}

static inline bool sexp_is_cons(sexp s) {
    return s->kind == SEXP_CONS;
}

static inline bool sexp_is_proc(sexp s) {
    return s->kind == SEXP_PROC;
}

static inline bool sexp_is_native_proc(sexp s) {
    return s->kind == SEXP_NATIVE_PROC;
}

static inline bool sexp_extract_cons(sexp s, sexp* car, sexp* cdr) {
    if (s->kind != SEXP_CONS) {
        return false;
    }

    if (car) *car = s->car;
    if (cdr) *cdr = s->cdr;
    return true;
}

static inline bool sexp_extract_string(sexp s, scheme_string* string) {
    if (s->kind != SEXP_STRING) {
        return false;
    }

    *string = s->string_value;
    return true;
}

static inline bool sexp_extract_number(sexp s, scheme_number* i) {
    if (s->kind != SEXP_NUMBER) {
        return false;
    }

    *i = s->number_value;
    return true;
}

static inline bool sexp_extract_symbol(sexp s, scheme_symbol* sym) {
    if (s->kind != SEXP_SYMBOL) {
        return false;
    }

    *sym = s->symbol_value;
    return true;
}

static inline bool sexp_extract_bool(sexp s, scheme_bool* b) {
    if (s->kind != SEXP_BOOL) {
        return false;
    }

    *b = s->bool_value;
    return true;
}

#define FOR_EACH_LIST(value, ident, body) \
    do {                                 \
        sexp ident = NULL;               \
        sexp __car = NULL;               \
        sexp __cdr = NULL;               \
        sexp __cursor = (value);         \
        while (1) {                      \
            if (sexp_is_empty(__cursor)) { \
                break;                   \
            }                            \
                                         \
            if (!sexp_extract_cons(__cursor, &__car, &__cdr)) { \
                fatal_error("invalid list"); \
            }                            \
                                         \
            ident = __car;               \
            (body);                      \
            __cursor = __cdr;            \
        }                                \
    } while(0)

#define DEBUG_DUMP(sexp) do {              \
        sexp_pretty_print((sexp), stdout); \
        fprintf(stdout, "\n");             \
        fflush(stdout);                    \
    } while(0)

#define FOR_EACH_LIST_2(value_one, value_two, ident_one, ident_two, body) \
    do {                                             \
        sexp ident_one = NULL;                       \
        sexp ident_two = NULL;                       \
        sexp __car_one = NULL;                       \
        sexp __car_two = NULL;                       \
        sexp __cdr_one = NULL;                       \
        sexp __cdr_two = NULL;                       \
        sexp __cursor_one = (value_one);             \
        sexp __cursor_two = (value_two);             \
        while (1) {                                  \
            if (sexp_is_empty(__cursor_one)) {       \
                assert(sexp_is_empty(__cursor_two)); \
                break;                               \
            }                                        \
                                                     \
            if (!sexp_extract_cons(__cursor_one, &__car_one, &__cdr_one)) { \
                fatal_error("invalid list");         \
            }                                        \
                                                     \
            if (!sexp_extract_cons(__cursor_two, &__car_two, &__cdr_two)) { \
                fatal_error("invalid list");        \
            }                                        \
                                                     \
            ident_one = __car_one;                   \
            ident_two = __car_two;                   \
            (body);                                  \
            __cursor_one = __cdr_one;                \
            __cursor_two = __cdr_two;                \
        }                                            \
    } while(0);

// Pretty prints an s-expression to the given output stream
void sexp_pretty_print(sexp s, FILE* output_stream);
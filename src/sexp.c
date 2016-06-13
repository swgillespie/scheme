#include "sexp.h"
#include "util.h"

static void print_atom(sexp s, FILE* output_stream) {
    scheme_string str;
    scheme_symbol sym;
    scheme_number num;
    scheme_bool b;
    if (sexp_extract_string(s, &str)) {
        fprintf(output_stream, "\"%s\"", str);
        return;
    }

    if (sexp_extract_number(s, &num)) {
        fprintf(output_stream, "%d", num);
        return;
    }

    if (sexp_extract_symbol(s, &sym)) {
        fprintf(output_stream, "%s", sym);
        return;
    }

    if (sexp_is_empty(s)) {
        fputs("()", output_stream);
        return;
    }

    if (sexp_extract_bool(s, &b)) {
        if (b) {
            fputs("#t", output_stream);
        } else {
            fputs("#f", output_stream);
        }

        return;
    }

    if (sexp_is_proc(s)) {
        fputs("#<procedure>", output_stream);
        return;
    }

    PANIC("unknown or invalid s-expression");
}

void sexp_pretty_print(sexp s, FILE* output_stream) {
    // Simple algorithm for pretty-printing an S-expression.
    // The algorithm goes like this:
    // 1) If self isn't a list, print it and return.
    // 2) If self is a list, print an open paren.
    //   2a) Print the car of the list.
    //   2b) If cdr is a list, recurse to step 2a with cdr as the new list
    //   2c) If cdr is (), print nothing,
    //   2d) If cdr is anything else, print a "." followed by a space
    //       and recursively print the cdr.
    sexp car;
    sexp cdr;
    if (!sexp_extract_cons(s, &car, &cdr)) {
        print_atom(s, output_stream);
        return;
    }

    fputc('(', output_stream);

    while (1) {
        sexp_pretty_print(car, output_stream);
        if (sexp_is_empty(cdr)) {
            break;
        }

        if (sexp_extract_cons(cdr, &car, &cdr)) {
            fputc(' ', output_stream);
            continue;
        }

        fputs(" . ", output_stream);
        print_atom(cdr, output_stream);
    }

    fputc(')', output_stream);
}
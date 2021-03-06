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
        if (strcmp(sym, ".") == 0) {
            // . as a symbol leads to goofy output
            // this is what guile does
            fputs("#{.}", output_stream);
            return;
        }

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
        fprintf(output_stream, "#<procedure \"%s\">", s->name);
        return;
    }

    if (sexp_is_native_proc(s)) {
        fprintf(output_stream, "#<native procedure \"%s\">", s->native_name);
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
        sexp_pretty_print(cdr, output_stream);
        break;
    }

    fputc(')', output_stream);
}
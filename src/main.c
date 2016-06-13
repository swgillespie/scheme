#include <stdio.h>
#include "util.h"
#include "reader.h"
#include "sexp.h"
#include "eval.h"

int main() {
    sexp out;
    sexp error;
    sexp eval_result;
    scheme_initialize();
    fprintf(stdout, ">> ");
    while(reader_read(stdin, &out, &error)) {
        eval_result = scheme_global_eval(out);
        fprintf(stdout, "==> ");
        sexp_pretty_print(eval_result, stdout);
        fprintf(stdout, "\n");
        fprintf(stdout, ">> ");
    }

    scheme_string s;
    if (!sexp_extract_string(error, &s)) {
        printf("error wasn't a string");
        return 1;
    }

    printf("error: %s\n", s);
}
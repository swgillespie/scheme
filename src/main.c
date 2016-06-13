#include <stdio.h>
#include "util.h"
#include "reader.h"
#include "sexp.h"
#include "eval.h"

static int repl() {
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
    return 0;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        return repl();
    }
 
    if (argc == 2) {
        FILE* f = fopen(argv[1], "r");
        if (f == NULL) {
            perror("failed to open file");
        }

        if (feof(f)) {
            // nothing to do with empty files.
            return 0;
        }

        scheme_initialize();

        sexp out;
        sexp error;
        bool had_error = true;
        while (reader_read(f, &out, &error)) {
            scheme_global_eval(out);

            // feof really sucks and doesn't work right unless the EOF
            // has actually been read
            ungetc(fgetc(f), f);
            if (feof(f)) {
                had_error = false;
                break;
            }
        }

        if (had_error) {
            printf("read error: %s\n", error->string_value);
        }

        return 0;
    }

    puts("usage: scheme [file.scm]");
    return 1;
}
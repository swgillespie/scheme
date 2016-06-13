#include <stdio.h>
#include "util.h"
#include "reader.h"
#include "sexp.h"
#include "eval.h"

int main(int argc, char** argv) {
    if (argc == 2) {
        scheme_initialize();
        scheme_eval_file(argv[1]);
        return 0;
    }

    puts("usage: scheme [file.scm]");
    return 1;
}
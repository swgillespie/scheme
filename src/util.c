#include "util.h"

void panic_impl(
    const char* file, 
    int line, 
    const char* func, 
    const char* msg, ...) {
    fprintf(stderr, "internal interpreter error: ");
    
    va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);
    
    fprintf(stderr, "\n");
    fprintf(stderr, "  at %s:%d, function %s()", file, line, func);
    fprintf(stderr, "\n");
    // we're going down.
    abort();
}

void fatal_error(const char* msg, ...) {
    fprintf(stdout, "fatal error: ");
    
    va_list args;
    va_start(args, msg);
    vfprintf(stdout, msg, args);
    va_end(args);
    fprintf(stdout, "\n");
    exit(1);
}
#include <string.h>
#include "types.h"

// from stackoverflow: http://stackoverflow.com/a/7666577/1030074
size_t scheme_symbol_hash_function(void* sym) {
    const char* str = (const char*)sym;
    size_t hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}


bool scheme_symbol_eq_function(void* sym1, void* sym2) {
    const char* str1 = (const char*)sym1;
    const char* str2 = (const char*)sym2;
    return strcmp(str1, str2) == 0;
}
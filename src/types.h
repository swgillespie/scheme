#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef int scheme_number;
typedef const char* scheme_string;
typedef const char* scheme_symbol;
typedef bool scheme_bool;

size_t scheme_symbol_hash_function(void* sym);
bool scheme_symbol_eq_function(void* sym1, void* sym2);

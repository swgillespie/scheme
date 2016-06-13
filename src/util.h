#pragma once

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <assert.h>

#define PANIC(msg, ...) panic_impl(__FILE__, __LINE__, __func__, msg, ##__VA_ARGS__)
#define UNREACHABLE() PANIC("entered unreachable code")
#define NYI() PANIC("not implemented")

// Prints a message to standard error and aborts. 
// Should be called through the PANIC macro instead, since that adds line and file information.
void panic_impl(const char* file, int line, const char* func, const char* msg, ...);

// Prints an error to standard out and exits with an
// exit code of 1.
void fatal_error(const char* msg, ...);
#pragma once

#include <stdio.h>
#include "sexp.h"

// The reader is responsible for reading s-expressions from an input stream.
// This function returns true if a sexp was successfully read, and false
// if it was not. In the case of an error, the "error" out param will be
// set to a string indicating the cause of the error.
//
// Will return an error if stream is pointed at EOF, which is probably
// not what you'd want. You'll need to wrap this call in something else
// to ensure proper EOF detection.
bool reader_read(FILE* stream, sexp* result_sexp, sexp* error);
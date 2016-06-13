#include "intrinsics.h"
#include "reader.h"
#include "eval.h"

#define INTRINSIC_DEF(scheme_name, arity, c_name, implementation) sexp c_name(sexp* args) implementation
#include "intrinsics.def"
#undef INTRINSIC_DEF

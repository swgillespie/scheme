#pragma once

#include "sexp.h"
#include "util.h"
#include "gc.h"

#define INTRINSIC_DEF(scheme_name, arity, c_name, impl) sexp c_name(sexp* args);
#include "intrinsics.def"
#undef INTRINSIC_DEF
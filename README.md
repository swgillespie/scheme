# Quick and dirty mini-Scheme interpreter
This is a quick and dirty Scheme interpreter. If I get the time,
in the future, this interpreter will be used to bootstrap a real scheme
compiler.

This repo builds with cmake and accepts two parameters: `CMAKE_BUILD_TYPE`, which may be one
of `Debug` or `Release` (defaults to debug) and `UBSAN` (1 or 0), which turns on the host compiler's
undefined behavior sanitizer.

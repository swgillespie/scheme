cmake_minimum_required(VERSION 2.8.12)
project(scheme)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "CMAKE_BUILD_TYPE not defined, defaulting to Debug build")
    set(CMAKE_BUILD_TYPE Debug)
endif()

include_directories(
        ${CMAKE_CURRENT_BINARY_DIR}/include)

set(CMAKE_C_FLAGS "-Wall -Werror -std=c99")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -DNDEBUG")

if (UBSAN)
    message(STATUS "Compiling with -fsanitize=undefined")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined -fno-sanitize-recover")
endif()

add_executable(scheme
    main.c
    gc.c
    reader.c
    util.c
    sexp.c
    hashtbl.c
    eval.c
    activation.c
    types.c
    intrinsics.c)

install (TARGETS scheme DESTINATION bin)
install (FILES 
    "scm/prelude.scm" 
    "scm/test.scm"
    DESTINATION lib/scheme)
#include <string.h>
#include <ctype.h>
#include "reader.h"
#include "gc.h"
#include "util.h"

#define ERROR_MESSAGE_BUFSIZE 1024
#define MAX_IDENTIFIER_SIZE 64
#define MAX_NUMBER_SIZE 10

#define TRY(expr) if (!(expr)) return false

#define ERROR_AND_RETURN(message, ...) \
    do {                    \
        snprintf(error_message_buf, ERROR_MESSAGE_BUFSIZE, message, ##__VA_ARGS__); \
        return false; \
    } while(0)        \

// prototypes
static bool read_sublist(FILE*, sexp*, char*);
static bool reader_read_toplevel(FILE*, sexp*, char*);

static char peek(FILE* stream) {
    static bool peek_saw_eof = false;
    if (peek_saw_eof) return EOF;

    char c = fgetc(stream);
    if (c != EOF) {
        char result = ungetc(c, stream);
        if (result == EOF) {
            PANIC("ungetc failed");
        }
    } else {
        peek_saw_eof = true;
    }
    return c;
}

static bool expect(FILE* stream, char c, char* error_message_buf) {
    char read = fgetc(stream);
    if (read != c) {
        ERROR_AND_RETURN("unexpected char: expected %c, got %c", c, read);
    }

    return true;
}


static void skip_to_first_nonwhitespace_char(FILE* stream) {
    while (1) {
        char peeked = peek(stream);
        if (isspace(peeked)) {
            (void)fgetc(stream);
            continue;
        }

        if (peeked == ';') {
            // this is a comment. skip to the next newline.
            // TODO(xplat) windows line endings
            (void)fgetc(stream);
            while (peek(stream) != '\n') {
                (void)fgetc(stream);
            }

            continue;
        }

        break;
    }
}

static bool is_scheme_allowed_ident_start(char c) {
    if (isalpha(c)) return true;

    switch (c) {
        case '_':
        case '-':
        case '+':
        case '/':
        case '*':
        case '?':
        case '!':
        case '=':
            return true;
        default:
            return false;
    }
}

static bool is_scheme_allowed_ident_body(char c) {
    return is_scheme_allowed_ident_start(c) || isdigit(c);
}

static bool read_symbol(FILE* stream, sexp* result_sexp, char* error_message_buf) {
    // the stream is pointed at the first letter of an identifier.
    char ident_buf[MAX_IDENTIFIER_SIZE];
    memset(ident_buf, 0, MAX_IDENTIFIER_SIZE);
    size_t idx = 0;
    ident_buf[idx++] = fgetc(stream);

    while (1) {
        char c = peek(stream);
        if (is_scheme_allowed_ident_body(c)) {
            if (idx == MAX_IDENTIFIER_SIZE - 1) {
                ERROR_AND_RETURN("identifier is too large");
            }

            ident_buf[idx++] = c;
            (void)fgetc(stream);
        } else {
            break;
        }
    }

    assert(idx < MAX_IDENTIFIER_SIZE);
    ident_buf[idx++] = 0;
    *result_sexp = gc_allocate_symbol(ident_buf);
    return true;
}

static bool read_number(FILE* stream, sexp* result_sexp, char* error_message_buf) {
    // the stream is pointed at the first digit of a number.
    char num_buf[MAX_NUMBER_SIZE];
    memset(num_buf, 0, MAX_NUMBER_SIZE);
    size_t idx = 0;
    num_buf[idx++] = fgetc(stream);

    while(1) {
        char c = peek(stream);
        if (isdigit(c)) {
            if (idx == MAX_NUMBER_SIZE - 1) {
                ERROR_AND_RETURN("number is too large");
            }

            num_buf[idx++] = c;
            (void)fgetc(stream);
        } else if (!isspace(c) && c != ')' && c != '(') {
            ERROR_AND_RETURN("unexpected char in numeric literal: %c", c);
        } else {
            break;
        }
    }

    assert(idx < MAX_NUMBER_SIZE);
    num_buf[idx++] = 0;
    int num = atoi(num_buf);
    *result_sexp = gc_allocate_number(num);
    return true;
}

static bool read_hash(FILE* stream, sexp* result_sexp, char* error_message_buf) {
    // only boolean literals are supported for hashes right now.
    // TODO char literals?
    TRY(expect(stream, '#', error_message_buf));
    if (peek(stream) == 't') {
        TRY(expect(stream, 't', error_message_buf));
        *result_sexp = gc_allocate_bool(true);
        return true;
    }
    
    if (peek(stream) == 'f') {
        TRY(expect(stream, 'f', error_message_buf));
        *result_sexp = gc_allocate_bool(false);
        return true;
    }

    ERROR_AND_RETURN("unexpected char while reading hash: %c", peek(stream));
}

static bool read_quote(FILE* stream, sexp* result_sexp, char* error_message_buf) {
    assert(peek(stream) == '\'');
    TRY(expect(stream, '\'', error_message_buf));
    sexp quoted;
    TRY(reader_read_toplevel(stream, &quoted, error_message_buf));
    *result_sexp = gc_allocate_cons(gc_allocate_symbol("quote"), gc_allocate_cons(quoted, gc_allocate_empty()));
    return true;
}

static bool read_atom(FILE* stream, sexp* result_sexp, char* error_message_buf) {
    skip_to_first_nonwhitespace_char(stream);
    char peeked = peek(stream);
    if (is_scheme_allowed_ident_start(peeked)) {
        return read_symbol(stream, result_sexp, error_message_buf);
    } else if (isdigit(peeked)) {
        return read_number(stream, result_sexp, error_message_buf);
    } else if (peeked == '(') {
        TRY(expect(stream, '(', error_message_buf));
        TRY(read_sublist(stream, result_sexp, error_message_buf));
        skip_to_first_nonwhitespace_char(stream);
        return expect(stream, ')', error_message_buf);
    } else if (peeked == '#') {
        return read_hash(stream, result_sexp, error_message_buf);  
    } else if (peeked == '\'') {
        return read_quote(stream, result_sexp, error_message_buf);  
    } else {
        ERROR_AND_RETURN("unexpected char when reading atom: %c", peeked);
    }

    return false;
}

static bool read_sublist(FILE* stream, sexp* result_sexp, char* error_message_buf) {
    // at this point, we are reading a list and we've already read
    // the opening paren.
    //   (call 1 2 3)
    //    ^--- we are here
    // we've also validated that we're not looking at a ), i.e. we are not
    // reading the empty list. We may be looking at whitespace, though.
    skip_to_first_nonwhitespace_char(stream);
    if (peek(stream) == ')') {
        *result_sexp = gc_allocate_empty();
        return true;
    }

    sexp list = gc_allocate_cons(gc_allocate_empty(), gc_allocate_empty());
    TRY(read_atom(stream, &list->car, error_message_buf));
    skip_to_first_nonwhitespace_char(stream);
    if (peek(stream) != ')') {
        // if I were more clever, this could be written iteratively.
        if (peek(stream) == '.') {
            // improper list.
            TRY(expect(stream, '.', error_message_buf));
            TRY(read_atom(stream, &list->cdr, error_message_buf));
        } else {
            TRY(read_sublist(stream, &list->cdr, error_message_buf));
        }
    }

    *result_sexp = list;
    return true;
}

static bool reader_read_toplevel(FILE* stream, sexp* result_sexp, char* error_message_buf) {
    skip_to_first_nonwhitespace_char(stream);
    if (peek(stream) != '(') {
        TRY(read_atom(stream, result_sexp, error_message_buf));
        return true;
    }

    TRY(expect(stream, '(', error_message_buf));
    skip_to_first_nonwhitespace_char(stream);
    if (peek(stream) == ')') {
        // this scheme permits the empty list.
        TRY(expect(stream, ')', error_message_buf));
        *result_sexp = gc_allocate_empty();
        return true;
    }

    TRY(read_sublist(stream, result_sexp, error_message_buf));
    skip_to_first_nonwhitespace_char(stream);
    TRY(expect(stream, ')', error_message_buf));
    return true;
}

bool reader_read(FILE* stream, sexp* result_sexp, sexp* error) {
    // we're reading a single s-expression.
    char error_message_buffer[ERROR_MESSAGE_BUFSIZE];
    memset(error_message_buffer, 0, ERROR_MESSAGE_BUFSIZE);
    if (!reader_read_toplevel(stream, result_sexp, error_message_buffer)) {
        // turn our error message into an s-expression.
        error_message_buffer[ERROR_MESSAGE_BUFSIZE - 1] = 0;
        assert(error_message_buffer[0] != 0);
        *error = gc_allocate_string(error_message_buffer);
        return false;
    }

    return true;
}
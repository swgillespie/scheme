#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef size_t (*hash_function)(void*);
typedef bool (*equality_function)(void*, void*);

typedef struct hashtbl_entry {
    struct hashtbl_entry* next;
    void* key;
    void* value;
} hashtbl_entry;

typedef struct hashtbl {
    hashtbl_entry** table;
    size_t capacity;
    size_t length;
    hash_function hash_func;
    equality_function eq_func;
} hashtbl;

// Initializes a new hash table using the given hash function.
void hashtbl_initialize(hashtbl* hash, hash_function hash_fun, equality_function eq_fun);

// Look up a key in the hash table and, if present, set the value
// associated with the given key and return true. Returns false
// if the key is not in the map.
bool hashtbl_lookup(hashtbl* hash, void* key, void** value);

// Returns true if the given key is in the hashtable, false otherwise.
static inline bool hashtbl_contains(hashtbl* hash, void* key) {
    void* value;
    return hashtbl_lookup(hash, key, &value);
}

// Inserts a key into the hash table.
void hashtbl_insert(hashtbl* hash, void* key, void* value);

// Update a value associated with a key in the hashtable. If no
// such key exists in the table, returns false, otherwise updates
// the value and returns true.
bool hashtbl_update(hashtbl* hash, void* key, void* new_value);

// Destroys a hash table.
void hashtbl_destroy(hashtbl* hash);
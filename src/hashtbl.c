#include <stdlib.h>
#include "hashtbl.h"
#include "util.h"

#define HASHTABLE_DEFAULT_SIZE 32

void hashtbl_initialize(hashtbl* hash, hash_function hash_fun, equality_function eq_fun) {
    hashtbl_entry** table  = calloc(HASHTABLE_DEFAULT_SIZE, sizeof(hashtbl_entry*));
    memset(table, 0, sizeof(hashtbl_entry*) * HASHTABLE_DEFAULT_SIZE);
    if (!table) {
        PANIC("out of memory");
    }

    hash->table = table;
    hash->capacity = HASHTABLE_DEFAULT_SIZE;
    hash->length = 0;
    hash->hash_func = hash_fun;
    hash->eq_func = eq_fun;
}

bool hashtbl_lookup(hashtbl* hash, void* key, void** value) {
    size_t index = hash->hash_func(key) % hash->capacity;
    hashtbl_entry* entry = hash->table[index];
    if (!entry) {
        return false;
    }

    while (entry) {
        if (hash->eq_func(key, entry->key)) {
            *value = entry->value;
            return true;
        }

        entry = entry->next;
    }

    return false;
}

void hashtbl_insert(hashtbl* hash, void* key, void* value) {
    // TODO(perf) maybe resize the table when the length exceeds
    // the capacity by some %?
    
    size_t index = hash->hash_func(key) % hash->capacity;
    hashtbl_entry* entry = hash->table[index];

    // entry may or may not be null here. The code's the same 
    // in either case. If entry is null, then there haven't been
    // any nodes here yet and we're the first one. If it's not null,
    // we are inserting ourselves as the head of the linked list of
    // entries.
    hashtbl_entry* new_entry = malloc(sizeof(hashtbl_entry));
    if (!new_entry) {
        PANIC("out of memory");
    }

    new_entry->next = entry;
    new_entry->key = key;
    new_entry->value = value;
    hash->table[index] = new_entry;
}

bool hashtbl_update(hashtbl* hash, void* key, void* value) {
    size_t index = hash->hash_func(key) % hash->capacity;
    hashtbl_entry* entry = hash->table[index];
    if (!entry) {
        return false;
    }

    while (entry) {
        if (hash->eq_func(key, entry->key)) {
            entry->value = value;
            return true;
        }

        entry = entry->next;
    }

    return false;
}

void hashtbl_destroy(hashtbl* hash) {
    for (size_t i = 0; i < hash->capacity; i++) {
        hashtbl_entry* entry = hash->table[i];
        while (entry) {
            hashtbl_entry* next = entry->next;
            free(entry);
            entry = next;
        }
    }

    free(hash->table);
}
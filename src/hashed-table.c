#include "hashed-table.h"

// Must be > 0
#define INITIAL_CAPACITY 16

// test for proj setup
int add(int x, int y)
{
    return x + y;
}

HashTable* HashTableCreate(void)
{
    // Allocate space for HashTable struct.
    HashTable* table = malloc(sizeof(HashTable));

    if (table == NULL) {
        return NULL;
    }

    table->length = 0;
    table->capacity = INITIAL_CAPACITY;

    // Allocate space for entries.
    // calloc() instead of malloc() for zero-initialized memory.
    table->entries = calloc(table->capacity, sizeof(Entry));

    // Error checking for memory allocation.
    if (table->entries == NULL) {
        free(table);
        return NULL;
    } else {
        return table;
    }
}

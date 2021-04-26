#include "hashed-table.h"
#include <assert.h>
#include <stdint.h>
#include <string.h>

// Must be > 0
#define INITIAL_CAPACITY 16

// Constants for FNV-1a hash algorithm
#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

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

void HashTableDestroy(HashTable* table)
{
    // Must first free allocated keys.
    for (size_t i = 0; i < table->capacity; i++) {
        if (table->entries[i].key != NULL) {
            free((void*)table->entries[i].key);
        }
    }

    // Then entries array and table can be freed.
    free(table->entries);
    free(table);
}

void* HashTableGet(HashTable* table, const char* key)
{
    // AND hash w/ capacity - 1 to ensure it's within entries array.
    // AND is only possible here as array size is guaranteed to be a power of two, for simplicity.
    uint64_t hash = HashKey(key);
    size_t index = (size_t)(hash & (uint64_t)(table->capacity - 1));

    void* foundValue = NULL;

    // Loops until empty entry.
    while (table->entries[index].key != NULL) {
        if (strcmp(key, table->entries[index].key) == 0) {
            // Key found, return value.
            foundValue = table->entries[index].value;
        }

        // Linear probing. If key isn't in slot, move to next.
        ++index;

        if (index >= table->capacity) {
            // If end of array is reached, wrap around.
            index = 0;
        }
    }

    return foundValue;
}

static const char* HashTableSetEntry(Entry* entries, size_t capacity, const char* key, void* value, size_t* plength)
{
    // AND hash w/ capacity - 1 to ensure it is within the entries array.
    uint64_t hash = HashKey(key);
    size_t index = (size_t)(hash & (uint64_t)(capacity - 1));

    // Loop until empty entry.
    while (entries[index].key != NULL) {
        if (strcmp(key, entries[index].key) == 0) {
            // Key exists, update value.
            entries[index].value = value;
            return entries[index].key;
        }

        // Linear probing if key isn't in slot. Move to next.
        ++index;

        if (index >= capacity) {
            // Wrap around.
            index = 0;
        }
    }

    // Key not found, allocate + copy if needed, then insert key.
    if (plength != NULL) {
        key = strdup(key);

        if (key == NULL) {
            return NULL;
        }

        ++(*plength);
    }

    entries[index].key = (char*)key;
    entries[index].value = value;

    return key;
}

static bool HashTableExpand(HashTable* table)
{
    bool isExpansionValid = false;

    // Allocate new entries array.
    size_t newCapacity = table->capacity * 2;

    if (newCapacity < table->capacity) {
        isExpansionValid = false;
    }

    Entry* newEntries = calloc(newCapacity, sizeof(Entry));

    if (newEntries == NULL) {
        isExpansionValid = false;
    }

    // Iterate entries, move all non-empty entries to new table's entries.
    for (size_t i = 0; i < table->capacity; ++i) {
        Entry entry = table->entries[i];

        if (entry.key != NULL) {
            HashTableSetEntry(newEntries, newCapacity, entry.key, entry.value, NULL);
        }
    }

    // Free old entries array and update this table's details.
    free(table->entries);
    table->entries = newEntries;
    table->capacity = newCapacity;

    return isExpansionValid;
}

const char* HashTableSet(HashTable* table, const char* key, void* value)
{
    assert(value != NULL);
    if (value == NULL) {
        return NULL;
    }

    // If length exceeds half of current capacity, expand it.
    if (table->length >= table->capacity / 2) {
        if (!HashTableExpand(table)) {
            return NULL;
        }
    }

    // Set entry and update length.
    return HashTableSetEntry(table->entries, table->capacity, key, value, &table->length);
}

size_t HashTableLength(HashTable* table)
{
    return table->length;
}

HTI HashTableIterator(HashTable* table)
{
    HTI iterator;
    iterator._table = table;
    iterator._index = 0;

    return iterator;
}

bool HashTableNext(HTI* iterator)
{
    bool isSuccessful = false;

    // Loop until end of entries array.
    HashTable* table = iterator->_table;

    while (iterator->_index < table->capacity) {
        size_t i = iterator->_index;
        ++iterator->_index;

        if (table->entries[i].key != NULL) {
            // Found next non-empty item, update iterator key and value.
            Entry entry = table->entries[i];
            iterator->key = entry.key;
            iterator->value = entry.value;
        }
    }
    return isSuccessful;
}

static uint64_t HashKey(const char* key)
{
    uint64_t hash = FNV_OFFSET;

    // Terminates when pointer hits '\0' or NUL
    for (const char* pointer = key; *pointer; ++pointer) {
        hash ^= (uint64_t)(unsigned char)(*pointer);
        hash *= FNV_PRIME;
    }

    return hash;
}

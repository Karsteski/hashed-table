#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// HashTable entry. Slot may be filled or empty.
typedef struct {
    const char* key; // Key is NULL if slot is empty.
    void* value;
} Entry;

// Use HashTableCreate or HashTableDestroy accordingly.
typedef struct {
    Entry* entries; // HashTable slots.
    size_t capacity; // Size of entries array.
    size_t length; // Number of items in the HashTable.
} HashTable;

// Returns a HashTable* or NULL if memory allocation failed.
HashTable* HashTableCreate(void);

// Frees memory allocated for both the HashTable and its allocated keys.
void HashTableDestroy(HashTable* table);

// Obtains item using a given key (NUL-terminated string). Returns value or NULL if key was not found.
void* HashTableGet(HashTable* table, const char* key);

// Internal function to set an entry without expanding the table.
static const char* HashTableSetEntry(Entry* entries, size_t capacity, const char* key, void* value, size_t* plength);

// Expand HashTable to twice its current size.
// Returns true on success, false if out of memory.
static bool HashTableExpand(HashTable* table);

// Sets item using a given key (NUL-terminated string) to a value that != NULL.
// If key is not present in the table, it is copied to a newly allocated memory.
// Returns address of copied key, or NULL if memory allocation failed.
const char* HashTableSet(HashTable* table, const char* key, void* value);

// Returns number of items in the hash table.
size_t HashTableLength(HashTable* table);

// HashTable iterator: Create with HashTableIterator, iterate with HashTableNext.
typedef struct {
    const char* key; // Current key.
    void* value; // Current value.

    // "Private" members. Not to be used directly.
    HashTable* _table; // Reference to hash table being iterated.
    size_t _index; // Current index into HashTable.entries.

} HTI;

// Returns new HashTable iterator (Using HashTableNext).
HTI HashTableIterator(HashTable* table);

// Move iterator to next item in the HashTable, update iterator key,value to current item and return true.
// If there are no more items, return false.
// HashTableSet should not be called during iteration.
bool HashTableNext(HTI* iterator);

// Returns 64-bit FNV-1a hash for a NUL-terminated key.
// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
static uint64_t HashKey(const char* key);
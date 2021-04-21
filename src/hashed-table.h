// test for proj setup
int add(int, int);

#include <stdbool.h>
#include <stdlib.h>

// Use HashTableCreate or HashTableDestroy accordingly.
typedef struct HashTable HashTable;

// Returns a HashTable* or NULL if memory allocation failed.
HashTable* HashTableCreate(void);

// Frees memory allocated for both the HashTable and its allocated keys.
void HashTableDestroy(HashTable* table);

// Obtains item using a given key (NULL-terminated string). Returns value or NULL if key was not found.
void* HashTableGet(HashTable* table, const char* key);

// Sets item using a given key (NULL-terminated string) to a value that != NULL.
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

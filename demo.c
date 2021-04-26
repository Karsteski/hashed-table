// Example from https://benhoyt.com/writings/hash-table-in-c/

#include <stdio.h>
#include <stdlib.h>

#include <hashed-table.h>

void exit_nomem(void)
{
    fprintf(stderr, "out of memory\n");
    exit(1);
}

int main(void)
{
    HashTable* counts = HashTableCreate();
    if (counts == NULL) {
        exit_nomem();
    }

    // Read next word from stdin (at most 100 chars long).
    char word[101];
    while (scanf("%100s", word) != EOF) {
        // Look up word.
        void* value = HashTableGet(counts, word);
        if (value != NULL) {
            // Already exists, increment int that value points to.
            int* pcount = (int*)value;
            (*pcount)++;
            continue;
        }

        // Word not found, allocate space for new int and set to 1.
        int* pcount = malloc(sizeof(int));
        if (pcount == NULL) {
            exit_nomem();
        }
        *pcount = 1;
        if (HashTableSet(counts, word, pcount) == NULL) {
            exit_nomem();
        }
    }

    // Print out words and frequencies, freeing values as we go.
    HTI it = HashTableIterator(counts);
    while (HashTableNext(&it)) {
        printf("%s %d\n", it.key, *(int*)it.value);
        free(it.value);
    }

    // Show the number of unique words.
    printf("%d\n", (int)HashTableLength(counts));

    HashTableDestroy(counts);
    return 0;
}

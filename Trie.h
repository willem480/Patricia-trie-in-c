#ifndef TRIE_H
#define TRIE_H

#include "TrieNode.h"


typedef struct {
    TrieNode **roots;     // array of root nodes, one per field
    int num_fields;

    Record *all_records;  // head of linked list for ownership/freeing

} Trie;

Trie *create_trie(int num_fields);

// Insert a new record with all field strings
void trie_insert_fields(Trie *trie, char **fields);

// Search in a given field
Record** trie_search(const Trie *trie, int field_index, const char *query, int *num_matches, Compare_counts *counts);

void Trie_print(const Trie *trie, char **titles);
void free_trie(Trie *trie);

#endif

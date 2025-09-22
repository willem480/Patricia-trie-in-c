#include <stdio.h>
#include <stdlib.h>
#include "Trie.h"
#include <assert.h>

Trie *create_trie(int num_fields) {
    Trie *trie = malloc(sizeof(Trie));
    assert(trie);

    trie->num_fields = num_fields;
    trie->roots = malloc(num_fields * sizeof(TrieNode *));
    assert(trie->roots);

    for (int i = 0; i < num_fields; i++) {
        trie->roots[i] = create_node("", 0, 0,0);
    }

    trie->all_records = NULL;
    

    return trie;
}

void trie_insert_fields(Trie *trie, char **fields) {
    if (!trie || !fields) return;

    // Create a Record using your existing factory
    Record *rec = create_record(trie->num_fields, fields);
    assert(rec);

    // Insert into each field's trie
    for (int i = 0; i < trie->num_fields; i++) {
        if (strlen(fields[i]) == 0) {
            continue;
        }
        insert_node(trie->roots[i], fields[i], rec);
       
    }

    // Ownership: prepend to all_records list
    rec->next = trie->all_records;
    trie->all_records = rec;
    
}

Record** trie_search(const Trie *trie, int field_index, const char *query, int *num_matches, Compare_counts *counts) {
    if (!trie || field_index < 0 || field_index >= trie->num_fields) return NULL;
    return trie_node_search(trie->roots[field_index], query, num_matches, counts);
}

void Trie_print(const Trie *trie, char **titles) {
    for (int i = 0; i < trie->num_fields; i++) {
        printf("Field[%d] (%s):\n", i, titles ? titles[i] : "");
        print_trie_node(trie->roots[i], 0);
    }
}

void free_trie(Trie *trie) {
    if (!trie) return;

    // Free each field's trie
    for (int i = 0; i < trie->num_fields; i++) {
        free_node(trie->roots[i]);
    }
    free(trie->roots);

    // Free all records via existing helper
    Record *cur = trie->all_records;
    while (cur) {
        Record *next = cur->next;
        free_record(cur);
        cur = next;
    }

    free(trie);
}

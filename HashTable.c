#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "HashTable.h"

// djb2 hash function
unsigned int hash_func(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    return hash % HASH_SIZE;
}

// Create a new empty hash table
HashTable *create_hashtable() {
    HashTable *ht = calloc(1, sizeof(HashTable));  // zero-initialize
    assert(ht);
    return ht;
}

// Insert record into hash table at given key
void hashtable_insert(HashTable *ht, const char *key, Record *rec) {
    unsigned int h = hash_func(key);
    HashNode *node = ht->buckets[h];

    while (node) {
        if (strcmp(node->key, key) == 0) {
            // Key exists — append record
            if (node->count == node->capacity) {
                node->capacity *= 2;
                node->records = realloc(node->records, sizeof(Record*) * node->capacity);
                assert(node->records);
            }
            node->records[node->count++] = rec;
            return;
        }
        node = node->next;
    }

    // Key not found — create new node
    HashNode *new_node = malloc(sizeof(HashNode));
    assert(new_node);
    new_node->key = strdup(key);
    new_node->capacity = 4;
    new_node->count = 1;
    new_node->records = malloc(sizeof(Record*) * new_node->capacity);
    assert(new_node->records);
    new_node->records[0] = rec;
    new_node->next = ht->buckets[h];
    ht->buckets[h] = new_node;
}

// Search records by key
Record **hashtable_search(HashTable *ht, const char *key, Compare_counts *counts, int *out_count) {
    unsigned int h = hash_func(key);
    HashNode *node = ht->buckets[h];

    while (node) {
        counts->node_count++;
        if (bit_strcmp(node->key, key, counts) == 0) {
            *out_count = node->count;
            return node->records;
        }
        node = node->next;
    }

    *out_count = 0;
    return NULL;
}

// Free hash table (does not free records)
void free_hashtable(HashTable *ht) {
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode *node = ht->buckets[i];
        while (node) {
            HashNode *tmp = node;
            node = node->next;
            free(tmp->key);
            free(tmp->records);
            free(tmp);
        }
    }
    free(ht);
}

// Print all records in hash table
void print_all_records_in_hashtable(HashTable *ht, char **titles) {
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode *node = ht->buckets[i];
        if (!node) continue;

        printf("Bucket %d:\n", i);
        while (node) {
            printf("  Key: %s\n", node->key);
            for (int j = 0; j < node->count; j++) {
                printf("    ");
                print_record(stdout, node->records[j], titles);
            }
            node = node->next;
        }
    }
}

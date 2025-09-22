// Hash node for chaining in hash table
#include "Record.h"
#include "Statistics.h"
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#define HASH_SIZE 1024
typedef struct HashNode {
    char *key;
    Record **records;   // array of pointers to Record
    int count;          // number of records stored
    int capacity;       // current capacity of the array
    struct HashNode *next;
} HashNode;


// Hash table structure
typedef struct HashTable {
    HashNode *buckets[HASH_SIZE];   // array of buckets, each bucket is a linked list of HashNodes
                                    // Each bucket corresponds to a hash value, hashnodes are evenly distributed across buckets
} HashTable;

// Hash function
unsigned int hash_func(const char *str);

// Create a new empty hash table
HashTable *create_hashtable(void);

// Insert a record into the hash table under the given key
void hashtable_insert(HashTable *ht, const char *key, Record *rec);

// Search for records by key
// Returns array of Record* and sets out_count to number of matches
Record **hashtable_search(HashTable *ht, const char *key, Compare_counts *counts, int *out_count);
void free_hashtable(HashTable *ht);
// Print all records in all non-empty buckets
void print_all_records_in_hashtable(HashTable *ht, char** titles);
#endif // HASH_TABLE_H
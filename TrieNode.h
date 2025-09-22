#ifndef TrieNode_H
#define TrieNode_H
#include "Record.h"
#include "Statistics.h"
#define BITS_PER_BYTE 8

// Core node structure
typedef struct TrieNode {
    char *stem;       // compressed bit sequence
    unsigned int stem_bitlen;  // number of valid bits

    int is_terminal;           // marks if word ends here
    char *full_key;            // full word (only at terminal)
    Record **records;          // array of records (only at terminal)
    int record_count;

    struct TrieNode **children;
    int child_count;
} TrieNode;

char *createStem(const char *oldKey, unsigned int startBit, unsigned int numBits);

// Create a new Trie node
TrieNode *create_node(const char *full_key,
                                   unsigned int start_bit,
                                   unsigned int num_bits,
                                   int is_terminal);

// Insert a key into the trie 
void insert_node(TrieNode *root, const char *key, Record *rec);
int edit_distance(const char *str1, const char *str2);
int min(int a, int b, int c);

// Search for exact match or closest match in the trie
Record **trie_node_search(TrieNode *root, const char *query, int *num_matches, Compare_counts *counts);

// Print a single trie node (recursive helper)
void print_trie_node(TrieNode *node, int depth);
void free_node(TrieNode *node);

#endif
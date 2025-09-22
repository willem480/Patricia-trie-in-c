#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include "TrieNode.h"

// -------------------- STEM UTILS --------------------

// Compare bits between stem and key from a given bit offset

/* Returns min of 3 integers
    reference: https://www.geeksforgeeks.org/edit-distance-in-c/ */
int min(int a, int b, int c)
{
    if (a < b)
    {
        if (a < c)
        {
            return a;
        }
        else
        {
            return c;
        }
    }
    else
    {
        if (b < c)
        {
            return b;
        }
        else
        {
            return c;
        }
    }
}

/* Returns the edit distance of two strings
    reference: https://www.geeksforgeeks.org/edit-distance-in-c/ */
int edit_distance(const char *str1, const char *str2)
{
    int n = strlen(str1);
    int m = strlen(str2);
    assert(m >= 0 && n >= 0 && (str1 || m == 0) && (str2 || n == 0));
    // Declare a 2D array to store the dynamic programming
    // table
    int dp[n + 1][m + 1];

    // Initialize the dp table
    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j <= m; j++)
        {
            // If the first string is empty, the only option
            // is to insert all characters of the second
            // string
            if (i == 0)
            {
                dp[i][j] = j;
            }
            // If the second string is empty, the only
            // option is to remove all characters of the
            // first string
            else if (j == 0)
            {
                dp[i][j] = i;
            }
            // If the last characters are the same, no
            // modification is necessary to the string.
            else if (str1[i - 1] == str2[j - 1])
            {
                dp[i][j] = min(1 + dp[i - 1][j], 1 + dp[i][j - 1],
                               dp[i - 1][j - 1]);
            }
            // If the last characters are different,
            // consider all three operations and find the
            // minimum
            else
            {
                dp[i][j] = 1 + min(dp[i - 1][j], dp[i][j - 1],
                                   dp[i - 1][j - 1]);
            }
        }
    }

    // Return the result from the dynamic programming table
    return dp[n][m];
}

static int getBit(const char *s, unsigned int bitIndex)
{
    assert(s && bitIndex >= 0);
    unsigned int byte = bitIndex / BITS_PER_BYTE;
    unsigned int indexFromLeft = bitIndex % BITS_PER_BYTE;
    /*
        Since we split from the highest order bit first, the bit we are interested
        will be the highest order bit, rather than a bit that occurs at the end of the
        number.
    */
    unsigned int offset = (BITS_PER_BYTE - (indexFromLeft)-1) % BITS_PER_BYTE;
    unsigned char byteOfInterest = s[byte];
    unsigned int offsetMask = (1 << offset);
    unsigned int maskedByte = (byteOfInterest & offsetMask);
    /*
        The masked byte will still have the bit in its original position, to return
        either 0 or 1, we need to move the bit to the lowest order bit in the number.
    */
    unsigned int bitOnly = maskedByte >> offset;
    return bitOnly;
}

char *createStem(const char *oldKey, unsigned int startBit, unsigned int numBits)
{
    assert(numBits > 0 && startBit >= 0 && oldKey);
    int extraBytes = 0;
    if ((numBits % BITS_PER_BYTE) > 0)
    {
        extraBytes = 1;
    }
    int totalBytes = (numBits / BITS_PER_BYTE) + extraBytes;
    char *newStem = malloc(sizeof(char) * totalBytes);
    assert(newStem);
    for (unsigned int i = 0; i < totalBytes; i++)
    {
        newStem[i] = 0;
    }
    for (unsigned int i = 0; i < numBits; i++)
    {
        unsigned int indexFromLeft = i % BITS_PER_BYTE;
        unsigned int offset = (BITS_PER_BYTE - indexFromLeft - 1) % BITS_PER_BYTE;
        unsigned int bitMaskForPosition = 1 << offset;
        unsigned int bitValueAtPosition = getBit(oldKey, startBit + i);
        unsigned int byteInNewStem = i / BITS_PER_BYTE;
        newStem[byteInNewStem] |= bitMaskForPosition * bitValueAtPosition;
    }
    return newStem;
}

// -------------------- NODE CREATION --------------------

TrieNode *create_node(const char *full_key,
                      unsigned int start_bit,
                      unsigned int num_bits,
                      int is_terminal)
{
    TrieNode *node = malloc(sizeof(TrieNode));
    assert(node);

    node->stem = (num_bits > 0) ? createStem((char *)full_key, start_bit, num_bits) : NULL;
    node->stem_bitlen = num_bits;

    node->is_terminal = is_terminal;
    node->full_key = (is_terminal && full_key) ? strdup(full_key) : NULL;
    node->records = NULL;
    node->record_count = 0;
    node->children = NULL;
    node->child_count = 0;

    return node;
}

void add_record_to_node(TrieNode *node, Record *rec)
{
    if (!rec)
        return;
    Record **tmp = realloc(node->records, (node->record_count + 1) * sizeof(Record *));
    assert(tmp);
    node->records = tmp;
    node->records[node->record_count++] = rec;
}

// Compare stem bits from two stems (used in split logic)
static unsigned int stemcmp(const char *stem1, unsigned int bits1,
                            const char *stem2, unsigned int bits2, Compare_counts *counts)
{
    unsigned int max_bits = (bits1 < bits2) ? bits1 : bits2;
    unsigned int match = 0;
    if (counts) counts -> string_count++;
    for (unsigned int i = 0; i < max_bits; i++)
    {
        if (getBit((char *)stem1, i) != getBit((char *)stem2, i)){
            if (counts) counts -> bit_count++;
            break;
        }
        match++;
    }
    return match;
}

void insert_node(TrieNode *root, const char *full_key, Record *rec)
{
    unsigned int total_bits = strlen(full_key) * 8;
    unsigned int bitpos = 0;
    TrieNode *current = root;

    while (1)
    {
        int found = 0;

        for (unsigned int i = 0; i < current->child_count; i++)
        {
            TrieNode *child = current->children[i];

            // Compare child's stem against the corresponding slice of full_key
            unsigned int rem_bits = total_bits - bitpos;
            char *newStem = createStem(full_key, bitpos, rem_bits);
            unsigned int common = stemcmp(child->stem, child->stem_bitlen, (char *)newStem, rem_bits, NULL);
            free(newStem);

            if (common == child->stem_bitlen)
            {
                // Exact match of child's stem → descend
                bitpos += common;
                current = child;

                if (bitpos == total_bits)
                {
                    // Full key consumed -> terminal node
                    add_record_to_node(current, rec);
                    current->is_terminal = 1;
                    if (!current->full_key)
                    {
                        current->full_key = strdup(full_key);
                    }

                    return;
                }

                // Restart search at new current node
                found = 1;
                break;
            }
            else if (common > 0)
            {
                // Partial match -> split

                // Create a new intermediate node for the common stem segment
                TrieNode *split_node = create_node(full_key, bitpos, common, 0);

                // Slice old child's stem to remove the common prefix bits
                char *old = child->stem;
                child->stem = createStem(old, common, child->stem_bitlen - common);
                free(old);
                child->stem_bitlen -= common;

                split_node->children = malloc(2 * sizeof(TrieNode *));
                assert(split_node->children);
                split_node->children[0] = child;

                TrieNode *new_leaf = create_node(full_key, bitpos + common,
                                                 total_bits - (bitpos + common), 1);
                add_record_to_node(new_leaf, rec);

                split_node->children[1] = new_leaf;
                split_node->child_count = 2;

                current->children[i] = split_node;

                return;
            }
        }

        if (!found)
        {
            // No child matched at this level -> create new leaf
            TrieNode *new_leaf = create_node(full_key, bitpos,
                                             total_bits - bitpos, 1);
            add_record_to_node(new_leaf, rec);

            current->children = realloc(current->children,
                                        (current->child_count + 1) * sizeof(TrieNode *));
            assert(current->children);
            current->children[current->child_count++] = new_leaf;

            return;
        }
    }
}

static void find_closest_node(TrieNode *node, const char *query, TrieNode **best_node, int *best_dist, Compare_counts *counts)
{
    counts -> node_count++;
    if (!node || !query || !best_node || !best_dist)
        return;

    // Score terminal nodes using full_key
    if (node->is_terminal && node->full_key)
    {
        int dist = edit_distance(node->full_key, query);
        if (dist < *best_dist)
        {
            *best_dist = dist;
            *best_node = node;
            if (*best_dist == 0)
                return; // early exit on perfect match
        }
    }

    // Traverse children
    for (int i = 0; i < node->child_count; i++)
    {
        find_closest_node(node->children[i], query, best_node, best_dist, counts);
    }
}

// -------------------- SEARCH --------------------
Record **trie_node_search(TrieNode *root, const char *query, int *num_matches, Compare_counts *counts)
{
    clear(counts);
    TrieNode *node = root;
    unsigned int query_bits = strlen(query) * BITS_PER_BYTE; // FIXED: removed +1
    unsigned int query_bit_offset = 0;

    while (query_bit_offset < query_bits)
    {
        int matched = 0;

        for (int i = 0; i < node->child_count; i++)
        {
            counts ->node_count++;
            TrieNode *child = node->children[i];

            // Create stem slice for comparison
            char *stem = createStem(query, query_bit_offset,
                                    query_bits - query_bit_offset);
            
            unsigned int common = stemcmp(child->stem, child->stem_bitlen,
                                          stem, query_bits - query_bit_offset, counts);
            free(stem);

            if (common > 0)
            {
                // descend into this child (even if partial match only)
                query_bit_offset += common;
                node = child;
                matched = 1;
                break;
            }
        }

        if (!matched)
        {
            // mismatch here → stop traversal
            break;
        }
    }

    // Fuzzy search from this subtree
    TrieNode *best_node = NULL;
    int best_dist = INT_MAX;
    find_closest_node(node, query, &best_node, &best_dist, counts);

    if (best_node && best_node->record_count > 0)
    {
        *num_matches = best_node->record_count;
        return best_node->records;
    }
    *num_matches = 0;
    return NULL;
}

// -------------------- FREE --------------------

void free_node(TrieNode *node)
{
    if (!node)
        return;
    for (int i = 0; i < node->child_count; i++)
    {
        free_node(node->children[i]);
    }
    free(node->children);
    free(node->records);
    free(node->stem);
    free(node->full_key);
    free(node);
}

void print_trie_node(TrieNode *node, int cur_depth)
{
    if (!node)
        return;

    // Indentation
    for (int i = 0; i < cur_depth; i++)
        printf("  ");

    // Print stem bits
    printf("|-- Stem(");
    for (unsigned int i = 0; i < node->stem_bitlen; i++)
    {
        unsigned int byte_index = i / BITS_PER_BYTE;
        unsigned int bit_offset = (BITS_PER_BYTE - 1) - (i % BITS_PER_BYTE);
        printf("%d", (node->stem[byte_index] >> bit_offset) & 1);
    }
    printf(") [%u bits]", node->stem_bitlen);

    // Terminal marker + key
    if (node->is_terminal)
    {
        printf(" (Terminal) --> \"%s\"", node->full_key ? node->full_key : "");
        if (node->record_count > 0)
        {
            printf(" [Records: %d]", node->record_count);
        }
    }
    printf("\n");

    // Recurse into children
    for (int i = 0; i < node->child_count; i++)
    {
        print_trie_node(node->children[i], cur_depth + 1);
    }
}

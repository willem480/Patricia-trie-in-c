#include <stdlib.h>
#include <string.h>
#include "HashTable.h"


#ifndef DICTIONARY_H
#define DICTIONARY_H

typedef struct Dictionary {
    int num_fields;
    HashTable **indexes;  // array of hash tables, one per field
    Record *all_records;  // linked list of all records (for freeing)
} Dictionary;

Dictionary *create_dictionary(int num_fields);
void dict_insert(Dictionary *dict, char **fields);
Record **dict_search(Dictionary *dict, int field_index, const char *value, Compare_counts *counts, int *out_count);
void free_dictionary(Dictionary *dict);
const char *get_field_value(Record *r, int index);
void print_all_records(Dictionary *dict, char** titles);
#endif // DICTIONARY_H
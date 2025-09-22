#include "Dictionary.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Create dictionary for given number of fields
Dictionary *create_dictionary(int num_fields) {
    Dictionary *dict = malloc(sizeof(Dictionary));
    assert(dict);
    dict->num_fields = num_fields;
    dict->indexes = malloc(sizeof(HashTable*) * num_fields);
    assert(dict->indexes);
    for (int i = 0; i < num_fields; i++) {
        dict->indexes[i] = create_hashtable();
    }
    dict->all_records = NULL;
    return dict;
}

// Helper: Get field value at index from linked list
const char *get_field_value(Record *r, int index) {
    Field *curr = r->fields;
    for (int i = 0; i < index && curr; i++) {
        curr = curr->next;
    }
    return curr ? curr->value : NULL;
}

// Insert a record into dictionary
void dict_insert(Dictionary *dict, char **fields) {
    Record *r = create_record(dict->num_fields, fields);

    // Add to all_records list
    r->next = dict->all_records;
    dict->all_records = r;

    // Insert into each index
    for (int i = 0; i < dict->num_fields; i++) {
        const char *key = get_field_value(r, i);
        if (key) {
            hashtable_insert(dict->indexes[i], key, r);
        }
    }
}

// Search by field index and value
Record **dict_search(Dictionary *dict, int field_index, const char *value, Compare_counts *counts, int *out_count) {
    clear(counts);
    if (field_index < 0 || field_index >= dict->num_fields) return NULL;
    return hashtable_search(dict->indexes[field_index], value, counts, out_count);
}

// Free dictionary and all records
void free_dictionary(Dictionary *dict) {
    for (int i = 0; i < dict->num_fields; i++) {
        free_hashtable(dict->indexes[i]);
    }
    free(dict->indexes);

    Record *r = dict->all_records;
    while (r) {
        Record *tmp = r;
        r = r->next;
        free_record(tmp);
    }

    free(dict);
}

// Print all records without mutating the list
void print_all_records(Dictionary *dict, char** titles) {
    Record *curr = dict->all_records;
    while (curr) {
        print_record(stdout, curr, titles);
        curr = curr->next;
    }
}

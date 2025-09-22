// Record structure: holds dynamic number of fields (strings)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef RECORD_H
#define RECORD_H

typedef struct Field {
    char* value;
    long double double_value;
    struct Field *next;       // for hash table chaining  
} Field;

typedef struct Record {
    int num_fields;
    Field *fields;           // head of linked list of fields
    struct Record *next;     // for linked list of records
} Record;

Record *create_record(int num_fields, char **fields);
void free_record(Record *r);
void print_record(FILE *fp, Record *r, char** titles);
void print_records(FILE* fp, Record **r, int num_records, char** titles);
#endif // RECORD_H
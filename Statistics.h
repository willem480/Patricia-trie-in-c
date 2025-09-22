#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#ifndef STATISTICS_H
#define STATISTICS_H

typedef struct Compare_counts {
    int bit_count;        // Total number of records in the dictionary
    int node_count;        // Total number of fields per record
    int string_count;       // Array to hold counts of unique values per field
} Compare_counts;

int bit_strcmp(const char *s1, const char *s2, Compare_counts *counts);
void print_statistics(FILE* fp, Compare_counts *counts) ;
void clear(Compare_counts *counts);
#endif // STATISTICS_H
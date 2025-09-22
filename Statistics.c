#include "Statistics.h"

void clear(Compare_counts *counts) {
    counts->bit_count = 0;
    counts->node_count = 0;
    counts->string_count = 0;
}

int bit_strcmp(const char *s1, const char *s2, Compare_counts *counts) {
    counts -> string_count++;
    assert(s1 && s2);
    if (strlen(s1) != strlen(s2)) {
        return 1;  // Strings must be of equal length
    }
    for (int i = 0; s1[i] && s2[i]; i++) {
        unsigned char a = s1[i];
        unsigned char b = s2[i];

        for (int j = 7; j >= 0; j--) {
            unsigned char mask = 1 << j;
            counts->bit_count++;

            int bit_a = (a & mask) ? 1 : 0;
            int bit_b = (b & mask) ? 1 : 0;

            if (bit_a != bit_b) {
                
                return 1;
            }
        }
    }
    return 0;
}

void print_statistics(FILE* fp, Compare_counts *counts) {
    fprintf(fp, "Total bit comparison: %d\n", counts->bit_count);
    fprintf(fp, "Total node comparison: %d\n", counts->node_count);
    fprintf(fp, "Total string comparison: %d\n", counts->string_count);
}
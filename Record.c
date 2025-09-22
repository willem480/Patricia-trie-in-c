#include "Record.h"
#include <assert.h>

// Create a new record with given fields
Record *create_record(int num_fields, char **fields) {
    Record *r = malloc(sizeof(Record));
    assert(r);
    r->num_fields = num_fields;
    r->fields = NULL;

    Field *prev = NULL;
    for (int i = 0; i < num_fields; i++) {
        Field *f = malloc(sizeof(Field));
        assert(f);
        f->value = strdup(fields[i]);
        f->next = NULL;

        if (!r->fields) {
            r->fields = f;
        } else {
            prev->next = f;
        }
        prev = f;
    }

    r->next = NULL;
    return r;
}



// Free a record
void free_record(Record *r) {
    Field *cur = r->fields;
    while (cur) {
        Field *tmp = cur;
        cur = cur->next;
        free(tmp->value);
        free(tmp);
    }
    free(r);
}


void print_record(FILE *fp, Record *r, char** titles) {
    if (!r) return;
    fprintf(fp, "-->");
    Field *cur = r->fields;
    for (int i = 0; i < r->num_fields; i++) {
        if (strcmp(titles[i], "x") == 0 || strcmp(titles[i], "y") == 0) {
            char *endptr = NULL;
            long double val = strtold(cur->value, &endptr);
            assert(endptr && *endptr == '\0'); // Ensure conversion was successful
            fprintf(fp, " %s: %.5Lf ||", titles[i], val); // Rounded to 5 decimals
        } else {
            fprintf(fp, " %s: %s ||", titles[i], cur->value);
        } 
        cur = cur->next;
    }
    fprintf(fp, "\n");
}


// Print all records in linked list
void print_records(FILE* fp, Record **r, int num_records, char** titles) {
    if (num_records == 0){
        fprintf(fp, "--> NOTFOUND\n");
        return;
    }
    for (int i = 0; i < num_records; i++) {
        assert(r[i]);
        if (r[i]) {
            print_record(fp, r[i], titles);
        } 
    }
}


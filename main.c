#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Dictionary.h"
#include <assert.h>
#include "Trie.h"
#include <time.h>
#define MAX_FIELD_LEN 127
#define MAX_LINE_LEN 511
#define MAX_FIELDS 35

char **split(const char *input, char delimiter, int *count);
void free_split(char **result, int count);

int main(int argc, char *argv[])
{
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    Dictionary *dict;
    Trie* trie;
    char buffer[MAX_LINE_LEN + 1];
    int num_fields = 0;
    int first_line = 1;
    int field_indedx = 0;
    char** titles;
    char* input_file_name = argv[2];
    char* output_file_name = argv[3];
    FILE *input_file = fopen(input_file_name, "r");
    FILE *output_file = fopen(output_file_name, "w");
    char* query = malloc(sizeof(char) * (MAX_FIELD_LEN+1));
    assert(query);
    assert(input_file);
    assert(output_file);

    Compare_counts *counts = malloc(sizeof(Compare_counts));
    assert(counts);


    while (fgets(buffer, sizeof(buffer), input_file) != NULL)
    {
        // Remove newline if present
        buffer[strcspn(buffer, "\n")] = '\0';
        char **fields = split(buffer, ',', &num_fields);
        assert(fields);

        if (first_line)
        {
            first_line = 0;
            dict = create_dictionary(num_fields);
            dict->num_fields = num_fields;
            trie = create_trie(num_fields);
            trie->num_fields = num_fields;
            
            titles = malloc(sizeof(char*) * num_fields);
            assert(titles);
            for (int i = 0; i < num_fields; i++)
            {
                titles[i] = strdup(fields[i]);
            }
            free_split(fields, num_fields);
            continue;
        }

        else
        {
            //dict_insert(dict, fields);
            trie_insert_fields(trie, fields);
            free_split(fields, num_fields);
        }
    }

    

    int out_count;
    Record **result;
    while (scanf("%[^\n]", query) != EOF)
    {
        
        getchar(); // Consume newline character
        fprintf(output_file, "%s\n", query);
        out_count = 0;
        
        result = trie_search(trie, field_indedx, query, &out_count, counts);
        //result = dict_search(dict, field_indedx, query, counts, &out_count);

        print_records(output_file, result, out_count, titles);
        if (result){
            printf( "%s --> %d records found - comparisons: b%d n%d s%d\n", query, out_count, counts->bit_count, counts->node_count, counts->string_count);
        }
        else {
            
            printf("%s --> NOT FOUND\n", query);
        }
        
    }

    // Clean up
    for (int i = 0; i < num_fields; i++)
    {
        free(titles[i]);
    }
    free(titles); 
    free_dictionary(dict);
    free_trie(trie);
    free(counts);
    fclose(input_file);
    fclose(output_file);
    free(query);

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Execution time: %.6f seconds\n", cpu_time_used);
    return 0;
}




char **split(const char *input, char delimiter, int *count)
{
    char **result = NULL;
    int tokens = 0;
    const char *start = input;
    const char *ptr = input;

    // Count tokens first
    while (*ptr)
    {
        if (*ptr == delimiter)
        {
            tokens++;
        }
        ptr++;
    }
    tokens++; // Last token after last delimiter

    result = malloc(tokens * sizeof(char *));
    assert(result);


    ptr = input;
    int index = 0;

    while (1)
    {
        if (*ptr == delimiter || *ptr == '\0')
        {
            size_t length = ptr - start;
            result[index] = malloc(length + 1);
            assert(result[index]);
            if (!result[index])
            {
                // free previously allocated memory
                for (int i = 0; i < index; i++)
                    free(result[i]);
                free(result);
                return NULL;
            }
            memcpy(result[index], start, length);
            result[index][length] = '\0';
            index++;

            if (*ptr == '\0')
                break;
            start = ptr + 1;
        }
        ptr++;
    }

    *count = tokens;
    return result;
}

void free_split(char **result, int count)
{
    for (int i = 0; i < count; i++)
    {
        free(result[i]);
    }
    free(result);
}


/*
 * Author: Lizzie Hernandez Videa
 * May 17, 2020
 * Purpose: To create a querier program that reads the indexer and answers
 *          queries from stdin.
 * 
 * See DESIGN.md for more information about the querier
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "hashtable.h"
#include <unistd.h>
#include "pagedir.h"
#include "hashtable.h"
#include "counters.h"
#include "index.h"
#include "file.h"
#include "words.h"
#include <stdbool.h>


/* ########### global variables ########### */

/* ########### local functions ########### */
int fileno(FILE *stream);
static void prompt(void);
int word_counter(char *line);
static void divide_query(char *line, char *words[], int count);
bool parse_query(char *words[], int count);
bool query(counters_t* result, hashtable_t *index, char *words[], int count);
void query_results(counters_t* result);

/* #### item functions for iterating #### */
void counters_add_all(void *arg, const int key, const int count);
void counters_merge(void *arg, const int key, const int count);
void counters_intersect(void *arg, const int key, const int count);
void counters_shrink(void *arg, const int key, const int count);
void counters_count(void *arg, const int key, const int count);

int main(int argc, char *argv[]){
    /* Check parameters */
    if(argc != 3){
        fprintf(stderr, "Usage error: Incorrect number of arguments.\n");
        return 1;
    }

    /* Get arguments */
    char *page_dir = argv[1];                   // directory produced by crawler
    char *index_name = argv[2];                 // file to write index into

    /* Check validity of arguments */
    if(!is_crawler_directory(page_dir)){
        fprintf(stderr, "Error: Incorrect 1st argument. Path does not exist or is not a crawler directory.\n");
        return 1;
    }

    FILE *index = fopen(index_name, "r");
    if(index == NULL){
        fprintf(stderr, "Error: Incorrect 2nd argument.\n");
        return 1;       
    }

    /* Build inverted index structure */
    hashtable_t *htable = hashtable_new(lines_in_file(index));
    if(htable == NULL){
        fprintf(stderr, "Error creating index structure.\n");
        return 2;       
    }

    int ret = file_to_index(index, htable);
    if(ret != 0){
        hashtable_delete(htable, index_item_delete);
        fclose(index);
        return 3;
    }
    fclose(index);

    /* Read queries */
    prompt();
    char *line;
    while ((line = freadlinep(stdin)) != NULL){
        /* Break line into an array of strings */
        int count = word_counter(line);
        if(count != 0){
            char* words[count];
            divide_query(line, words, count);
            if(parse_query(words, count)){
                /* Query */
                counters_t* result = counters_new();
                query(result, htable, words, count);

                /* Print results */
                query_results(result);

            }
        }
        
        /* Next query */
        free(line);
        prompt();
    }
    
}

/*
 * Function to print query prompt
 */ 
static void prompt(void){   
    if (isatty(fileno(stdin))) {
        printf("Query? ");
    }
}

/*
 * Function to parse a query line
 */ 
static void divide_query(char *line, char *words[], int count){
    int j = 0;
    for(int i= 0; i<count; i++){
        while(isspace(line[j]))     // skip all white spaces before word
            j++;
        
        words[i] = &line[j];        // assign item at index j to new word found
        
        while(isalpha(line[j]))     // skip all alphabetic characters
            j++;

        line[j] = '\0';             // end word
    }
}

int word_counter(char *line){
    /* Count the number of words */
    int count = 0;

    if(isalpha(line[0])) count++;

    for(int i =1; i< strlen(line); i++){
        if(isalpha(line[i]) && isspace(line[i-1]))
            count++;
        else if(!isalpha(line[i]) || !isspace(line[i])){
            fprintf(stderr, "Bad character in query %c.\n", line[i]);
            return 0;
        }
    }
    return count;
}

bool parse_query(char *words[], int count){
    /* Normalize all words (to lower case) */
    for(int i = 0; i< count; i++)
        normalize_word(words[i]);

    /* Print query */
    fprintf(stdout, "Query:");
    for(int i = 0; i< count; i++)
        fprintf(stdout, " %s", words[i]);
    fprintf(stdout, "\n");


    /* Check first word */
    if(strcmp(words[0], "and") == 0 || strcmp(words[0], "or") == 0){
        fprintf(stderr, "Syntax error: Cannot start query with %s.\n", words[0]);
        return false;
    }

    /* Check last word */
    if(strcmp(words[count-1], "and") == 0 || strcmp(words[count-1], "or") == 0){
        fprintf(stderr, "Syntax error: Cannot end query with %s.\n", words[count-1]);
        return false;
    }
    
    /* Check words in between */
    for(int i = 1; i< count - 1; i++){
        if(strcmp(words[i], "and") == 0 || (strcmp(words[i], "or") == 0)){
            /* Check that words before and after and, or are not and, or */
            if(strcmp(words[i-1], "and") == 0 || strcmp(words[i-1], "or") == 0){
                fprintf(stderr, "Syntax error: ANDs and ORs cannot be adjacent.\n");
                return false;
            }
            else if (strcmp(words[i+1], "and") == 0 || strcmp(words[i+1], "or") == 0){
                return false;
                fprintf(stderr, "Syntax error: ANDs and ORs cannot be adjacent.\n");
            }
        }
    }
    return true;
}

bool query(counters_t* result, hashtable_t *index, char *words[], int count){
    counters_t* res_and = counters_new();
    counters_t* res_or = counters_new();

    if(res_and==NULL || res_or == NULL){
        fprintf(stderr, "Could not initialize structures.\n");
        return false;
    }

    /* Add first word to res_and */

    for(int i = 0; i<= count ; i++){
        /* Resolve an end of sequence by finding th union of res_or and res_and */
        if(strcmp(words[i], "or") == 0 || i == count){
            /* Move result and to result or (merge) */
            counters_t* merged = counters_new();
            if(merged == NULL){
                return false;
            }
            
            counters_iterate(merged, res_and, counters_merge);
            counters_iterate(merged, res_or, counters_merge);

            /* restart counters */
            counters_delete(res_and);
            counters_delete(res_or);

            res_or = merged;

            res_and = counters_new();
            if(res_and == NULL){
                return false;
            }
        }
        /* Resolve sequences */
        else if(strcmp(words[i], "and") != 0){
            /* Get word's counters */
            counters_t* ctrs = hashtable_find(index, words[i]);
            /* If word found */
            if(ctrs != NULL){
                /* If found start of a new and sequence, add all to res_and */
                if(i == 0 || strcmp(words[i-1], "or")==0){
                    counters_iterate(ctrs, res_and, counters_add_all);
                }
                /* If its the rest of a sequence */
                else{
                    /* Interect ctrs with res_and */
                    counters_t* intersection = counters_new();

                    /* Find union with lesser values */
                    counters_iterate(ctrs, res_and, counters_intersect);
                    /* Keep only items in both counters */
                    counters_iterate(intersection, res_and, counters_shrink);

                    counters_delete(res_and);
                    res_and = intersection;
                }
            }
            /* Else, ignore rest of the sequence (intersection is empty) */
            else{
                /* Empty res_and */
                counters_delete(res_and);
                res_and = counters_new();

                /* Skip to next sequence or end of query */
                i++;
                while(i<=count && strcmp(words[i], "or") == 0) 
                    i++;
            }
        }
    }

    /* Add to final result */
    counters_iterate(res_or, result, counters_add_all);

    /* Clean up*/
    counters_delete(res_and);
    counters_delete(res_or);

    return true; 
}

void query_results(counters_t* result){
    /* Find counter's size */
    int size = 0;
    counters_iterate(result, &size, counters_count);
}

/* ############## item functions for iterating ############## */

void counters_add_all(void *arg, const int key, const int count){
    counters_set((counters_t*)arg, key, count);
}

void counters_merge(void *arg, const int key, const int count){
    counters_set((counters_t*)arg, key, count + counters_get(arg, key));
}

void counters_intersect(void *arg, const int key, const int count){
    /* Get value from the other counter */
    counters_t* other_counter = (counters_t*)arg;
    int other_count = counters_get(other_counter, key);

    /* Keep the smallest value */
    int lesser_value;
    if(other_count < count)
        lesser_value = other_count;
    else
        lesser_value = count;
    
    counters_set(other_counter, key, lesser_value);
}

void counters_shrink(void *arg, const int key, const int count){
    counters_t* other_counter = (counters_t*)arg;
    if(count > 0){
        counters_set(other_counter, key, count);
    }
}

void counters_count(void *arg, const int key, const int count){
    int* size = (int *)arg;
    size++;
}
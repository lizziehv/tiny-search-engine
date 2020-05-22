/*
 * Author: Lizzie Hernandez Videa
 * May 17, 2020
 * Purpose: To create a querier program that reads the indexer and answers
 *          queries from stdin.
 * 
 * See DESIGN.md for more information about the querier
 */
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hashtable.h"
#include "pagedir.h"
#include "counters.h"
#include "index.h"
#include "file.h"
#include "words.h"
#include "iterating.h"
#include "tuple.h"

/* ######## functions from imports ######## */
int fileno(FILE *stream);

/* ########### global variables ########### */

/* ########### local functions ############ */
void prompt(void);
int word_counter(char *line);
void divide_query(char *line, char *words[], int count);
bool parse_query(char *words[], int count);
bool query(counters_t* result, hashtable_t *index, char *words[], int count);
static counters_t * resolve_sequence(hashtable_t *index, char *words[], int count, int* current);
void print_results(counters_t* result, char* pade_dir);

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
        fclose(index);
        fprintf(stderr, "Error allocating memory for structures.\n");
        return 2;       
    }

    int ret = file_to_index(index, htable);
    if(ret != 0){
        hashtable_delete(htable, index_item_delete);
        fclose(index);
        return 2;
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
                if(query(result, htable, words, count)){
                    /* Print results */
                    print_results(result, page_dir);
                }
                else{
                    /* return on error */
                    free(line);
                    line = NULL;
                    hashtable_delete(htable, index_item_delete);
                    htable = NULL;

                    return 2;
                }
                counters_delete(result);
            }
        }
        
        /* Next query */
        free(line);
        line = NULL;
        prompt();
    }

    hashtable_delete(htable, index_item_delete);
    htable = NULL;

    return 0;
}

/*
 * Function to print query prompt
 */ 
void prompt(void){   
    if (isatty(fileno(stdin))) {
        printf("Query? ");
    }
}

/* @param char *line
 * 
 * Count the number of words in a line for size of word array
 * 
 * @return number of words in line
 */
int word_counter(char *line){
    /* Count the number of words */
    int count = 0;

    if(strlen(line) == 0) return 0;

    if(isalpha(line[0])) count++;

    for(int i =1; i< strlen(line); i++){
        if(isalpha(line[i]) && isspace(line[i-1]))
            count++;
        else if(!isalpha(line[i]) && !isspace(line[i])){
            fprintf(stderr, "Bad character in query %c.\n", line[i]);
            return 0;
        }
    }
    return count;
}


/* @param char *line - pointer to line to be divided
 * @param char *words[] - array of strings (empty)
 * @param int count - number of words in string array
 * 
 * Function to divide query line into an arrray of words
 * 
 * @return void
 */ 
void divide_query(char *line, char *words[], int count){
    int i = 0;
    for(char *cp = line; *cp != '\0'; cp++){
        if(isalpha(*cp)){
            /* Insert pointer to first character in word to array */
            words[i] = cp;
            i++;
            /* Skip all letters */
            while(*cp != '\0' && isalpha(*cp))
                cp++;
            /* Insert end of string at the end */
            if(isspace(*cp)){
                *cp = '\0';
            }
            else{
                break;
            }   
        }
    }
}

/* @param char *words[] - array of strings (empty)
 * @param int count - number of words in string array
 * 
 * Normalize each word in words
 * Output query
 * Check if the format is valid
 * <and sequence> or <and sequence> etc.
 * 
 * @return false if a word contains a non alphabetic character or
 *         format is invalid: consecutive ANDs, ORs, or query starts
 *         with AND, OR
 * @return true if success (valid format ans)
 */ 
bool parse_query(char *words[], int count){
    /* Normalize all words (to lower case) */
    for(int i = 0; i< count; i++){
        if(!normalize_word(words[i])) 
            return false;
    }

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

/* @param result - empty counters set to insert query results
 * @param index - inverted-index structure
 * @param words - array of strings containing words
 * @param count - number of words
 * 
 * Find query results by merging and_sequences
 * 
 * @return false if any error initializing structures
 * @return true if success
 */ 
bool query(counters_t* result, hashtable_t *index, char *words[], int count){
    int i;
    for(i = 0; i< count ; i++){
        counters_t *res_and = resolve_sequence(index, words, count, &i);
        if(res_and != NULL){
            /* Add to results by finding union */
            counters_iterate(res_and, result, counters_merge);
            counters_delete(res_and);
            res_and = NULL;
        }
        else{
            /* error loading structures */
            return false;
        }
    }
    return true; 
}

/* Helper function for query()
 * 
 * @param result - empty counters set to insert query results
 * @param index - inverted-index structure
 * @param words - array of strings containing words
 * @param count - number of words
 * @param current - index in words where current index was found
 * 
 * Function will find the intersection of all words in a sequence
 * Update current index to point to the beginning of the next sequence
 * 
 * @return pointer to counters containing intersection
 *         NULL for any error
 */ 
static counters_t * resolve_sequence(hashtable_t *index, char *words[], int count, int* current){
    counters_t *res_and = counters_new();   // keeps track of intersection of word counters in a sequence
    if(res_and == NULL){
        fprintf(stderr, "Error allocating memory for structures.\n");
        return NULL;
    }
    int i;
    for(i = *current; i< count && strcmp(words[i], "or") != 0; i++){
        if(strcmp(words[i], "and") != 0){
            /* Get word's counters */
            counters_t* ctrs = hashtable_find(index, words[i]);
            
            /* If word found */
            if(ctrs != NULL){
                /* If found start of a new and sequence, add all to result */
                if(i == 0 || strcmp(words[i-1], "or")==0){
                    counters_iterate(ctrs, res_and, counters_add_all);
                }
                /* If its the rest of a sequence */
                else{
                    /* Intersect ctrs with result */
                    counters_t* intersection = counters_new();
                    if(intersection == NULL){
                        fprintf(stderr, "Error allocating memory for structures.\n");
                        /* Clean up*/
                        counters_delete(res_and);
                        res_and = NULL;
                        return NULL;
                    }

                    /* Find intersection with lesser values */
                    counters_iterate(res_and, ctrs, counters_intersect);
                    counters_iterate(ctrs, res_and, counters_intersect);
                    /* Keep only items in both counters */
                    counters_iterate(res_and, intersection, counters_add_all);

                    counters_delete(res_and);
                    res_and = NULL;
                    res_and = intersection;
                }
            }
            /* Else, ignore rest of the sequence (intersection is empty) */
            else{
                /* Skip to next sequence or end of query */
                while(i<count && strcmp(words[i], "or") != 0)
                    i++;

                counters_delete(res_and);
                *current = i;
                return counters_new();
            }
        }
    }
    /* Move current index to end of sequence */
    *current = i;
    /* Return intersection set */
    return res_and;
}


/* @param result - counter with results from query
 * @param page_dir - crawler produced directory where pages come from 
 * 
 * Print results to stdout. Pages ranked with format
 * score <score> doc <doc>: <url>
 * 
 */ 
void print_results(counters_t* result, char* page_dir){
    /* Find counter's size */
    int size = 0;
    counters_iterate(result, &size, counters_count);

    if(size == 0){
        fprintf(stdout, "No documents match.\n");
    }
    else{
        fprintf(stdout, "Matches %d documents (ranked):\n", size);
        tuple_t* res_list[size + 1];
        tuple_t* index_pair = tuple_new(-1, 1); // first tuple inserted is used to keep track of where the 
        res_list[0] = index_pair;               // next item should be inserted, updated by iteration

        /* insert all results into array and sort */
        counters_iterate(result, res_list, counters_to_array);

        /* print each result */
        int padding = char_counter(tuple_get_score(res_list[1]));
        for(int i=1; i< size+1; i++){
            /* get url */
            char* url = get_url(page_dir, tuple_get_key(res_list[i]));
            fprintf(stdout, "score %*d doc %d: %s\n", padding, tuple_get_score(res_list[i]), tuple_get_key(res_list[i]), url);
            free(url);
        }

        /* clean up */
        for(int i=0; i< size + 1; i++){
            free(res_list[i]);
        }
        
    }

    fprintf(stdout, "-----------------------------------------------\n");
}




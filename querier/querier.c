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

/* ######## functions from imports ######## */
int fileno(FILE *stream);

/* ########### global variables ########### */

/* ########### local functions ############ */
void prompt(void);
int word_counter(char *line);
void divide_query(char *line, char *words[], int count);
bool parse_query(char *words[], int count);
bool query(counters_t* result, hashtable_t *index, char *words[], int count);
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
                if(query(result, htable, words, count)){
                    /* Print results */
                    print_results(result, page_dir);
                }
                else
                    fprintf(stderr, "Error initializing structures.\n");
            }
        }
        
        /* Next query */
        free(line);
        prompt();
    }

    hashtable_delete(htable, index_item_delete);
    
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
    // int max = strlen(line);
    // int j = 0, k = 0;
    // for(int i=0; i <max; i++){
    //     /* Found new word */
    //     if(isalpha(line[i])){
    //         words[j] = &line[i];                // assign item at index j to new word found
    //         j++;

    //         while(i < max-1 && isalpha(line[i]))  // skip all alphabetic characters
    //             i++;
            
    //         if(isspace(line[i])){
                
    //         }
    //     }
    // }
    char *token = strtok(line, " \t");

    int i = 0;
    while (token != NULL && i < count) {
        words[i] = token; 
        token = strtok(NULL, " \t");
        i++;
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
 * Find query results by mering and sequences
 * 
 * @return false if any error initializing structures
 * @return true if success
 */ 
bool query(counters_t* result, hashtable_t *index, char *words[], int count){
    counters_t* res_and = counters_new();       // keeps track of current sequence counters
    counters_t* res_or = counters_new();        // keeps track of results from all sequences seen thus far                                            

    /* return on any error */
    if(res_and==NULL || res_or == NULL){
        fprintf(stderr, "Could not initialize structures.\n");
        return false;
    }

    for(int i = 0; i<= count ; i++){
        /* Resolve an end of sequence by finding th union of res_or and res_and */
        
        if( i == count || strcmp(words[i], "or") == 0){
            /* Move result and to result or (merge) */
            counters_t* merged = counters_new();
            if(merged == NULL){
                return false;
            }
            
            counters_iterate(res_and, merged, counters_merge);
            counters_iterate(res_or, merged, counters_merge);

            /* restart counters */
            res_or = merged;

            counters_delete(res_and);
        
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
                    counters_iterate(res_and, ctrs, counters_intersect);
                    counters_iterate(ctrs, res_and, counters_intersect);
                    /* Keep only items in both counters */
                    counters_iterate(res_and, intersection, counters_add_all);

                    counters_delete(res_and);
                    res_and = intersection;
                }
            }
            /* Else, ignore rest of the sequence (intersection is empty) */
            else{
                /* Empty res_and, create new one */
                counters_delete(res_and);
                res_and = counters_new();
                if(res_and == NULL)
                    return false;

                /* Skip to next sequence or end of query */
                while(i<count-1 && strcmp(words[i + 1], "or") == 0) 
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

void print_results(counters_t* result, char* page_dir){
    /* Find counter's size */
    int size = 0;
    counters_iterate(result, &size, counters_count);

    if(size == 0){
        fprintf(stdout, "No documents match.\n");
    }
    else{
        fprintf(stdout, "Matches %d documents (ranked):\n", size);
        ctr_t* res_list[size + 1];
        ctr_t* index_pair = ctr_new(-1, 1);
        res_list[0] = index_pair;

        /* insert all results into array and sort */
        counters_iterate(result, res_list, counters_to_array);

        /* print each result */
        int padding = char_counter(ctr_get_score(res_list[1]));
        for(int i=1; i< size+1; i++){
            /* get url */
            char* url = get_url(page_dir, ctr_get_key(res_list[i]));
            fprintf(stdout, "score %*d doc %d: %s\n", padding, ctr_get_score(res_list[i]), ctr_get_key(res_list[i]), url);
            free(url);
        }

        /* clean up */
        for(int i=0; i< size + 1; i++){
            free(res_list[i]);
        }
        
    }

    fprintf(stdout, "-----------------------------------------------\n");
}

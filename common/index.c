
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "webpage.h"
#include "memory.h"
#include "hashtable.h"
#include "bag.h"
#include "pagedir.h"
#include "words.h"
#include "counters.h"
#include "file.h"

/* ################## global functions ############### */
/* ############# See index.h for details ############# */

/* ################## local functions ############### */
static void printcounters(void *f_in, const int key, const int count);
static void printword(void *f_in, const char *key, void *ctrs);

/* ################## index_build() ############### */
/* ############ See index.h for details ########### */
int index_build(char *page_dir, hashtable_t *htable){
    int current_id = 1;
    webpage_t *page;

    /* read every file and convert into webpage */
    while((page = file_to_webpage(page_dir, current_id)) != NULL){
        
        /* read every word in the file */
        int pos = 0;
        char *word;
 
        while ((word = webpage_getNextWord(page, &pos)) != NULL) {
            /* ignore words that can't be normalized */
            if(strlen(word) >= 3 && normalize_word(word)){
                counters_t *ctrs = hashtable_find(htable, word);
                /* intitialize counters if word is not in hashtable */
                if(ctrs == NULL){
                    ctrs = counters_new();
                    if(ctrs == NULL)
                        return 1;
                    hashtable_insert(htable, word, ctrs);
                }
                /* increment count for word seen */
                counters_add(ctrs, current_id);
            }
            /* cleanup */
            free(word);
        }
        /* cleanup */
        webpage_delete(page);
        current_id++;
    }
    return 0;
}

/* ################## index_save() ################ */
/* ############ See index.h for details ########### */
int index_save(char *filename, hashtable_t *htable){
    FILE *f_in = fopen(filename, "w");
    if(f_in == NULL)
        return 1;
    /* print every word, counter pair */
    hashtable_iterate(htable, f_in, printword);
    fclose(f_in);
    return 0;
}

/* ################ file_to_index() ############### */
/* ############ See index.h for details ########### */
int file_to_index(FILE* f_in, hashtable_t *htable){
    int numlines = lines_in_file(f_in);
    for(int i = 0; i<numlines; i++){
        /* read word */
        char* word = freadwordp(f_in);
        if(word == NULL){
            fprintf(stderr, "Incorrect format in line %d.\n", i);
        }

        /* create counters */
        counters_t* ctrs = counters_new();
        if(ctrs == NULL)
            return 1;
        
        /* read word */
        int ret, key, count;
        while((ret=fscanf(f_in, " %d %d", &key, &count))== 2){
            if(!counters_set(ctrs, key, count)){
                fprintf(stderr, "Error setting counters.\n");
                return 1;

            }
        }
            
        /* insert word to hashtable */
        if(!hashtable_insert(htable, word, ctrs)){
            fprintf(stderr, "Error inserting into inverted index structure.\n");
            return 1;           
        }

        free(word);
        word = NULL;
    }
    return 0;
}


/* ############### index_item_delete() ############ */
/* ############ See index.h for details ########### */
void index_item_delete(void *item) {
    counters_delete((counters_t *)item);
}

/*
 * Print one word per line and its counters
 */
static void printword(void *f_in, const char *key, void *ctrs){
    /* print every word and its counters */
    fprintf((FILE *)f_in, "%s", key);
    counters_iterate((counters_t *)ctrs, f_in, printcounters);
    fprintf((FILE *)f_in, "\n");
}

/*
 * Print a counter item with format:
 * key value
 */
static void printcounters(void *f_in, const int key, const int count){
    fprintf((FILE *)f_in, " %d %d", key, count);
}


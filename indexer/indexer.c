/*
 * Author: Lizzie Hernandez Videa
 * May 9, 2020
 * Purpose: To create an index for words in the files created by the crawler
 * 
 * See DESIGN.md for more information about the indexer
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "webpage.h"
#include "memory.h"
#include "hashtable.h"
#include "bag.h"
#include "pagedir.h"
#include "index.h"
#include "counters.h"

/* ########### global variables ########### */
int HTSIZE = 900;

/* ########### local functions ########### */
static void index_item_delete(void *item);

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
        fprintf(stderr, "Error: Incorrect 1st argument. Path does not exist or is not a directory.\n");
        return 1;
    }
    if(!is_valid_file(index_name)){
        fprintf(stderr, "Error: Incorrect 2nd argument. Incorrect path name.\n");
        return 1;
    }

    /* Initialize data structures */
    /* create a hashtable, return on error */
    hashtable_t *htable = hashtable_new(HTSIZE);
    if(htable == NULL){
        fprintf(stderr, "Error creating hashtable.\n");
        return 2;
    }

    /* build indexer */
    int ret = index_build(page_dir, htable);
    if(ret != 0){
        hashtable_delete(htable, index_item_delete);
        return 3;
    }
    
    /* save indexer */
    ret = index_save(index_name, htable);
    if(ret != 0){
        hashtable_delete(htable, index_item_delete);
        return 3;
    }

    hashtable_delete(htable, index_item_delete);
    return 0;
}

/**
 * Wrapper for counters delete function
 */ 
static void index_item_delete(void *item) {
    counters_delete((counters_t *)item);
}

/*
 * Author: Lizzie Hernandez Videa
 * May 9, 2020
 * Purpose: To create a test that compares an index file with a new index
 * 
 * See DESIGN.md for more information about the indextest
 */

#include <stdlib.h>
#include <stdio.h>
#include "pagedir.h"
#include "hashtable.h"
#include "counters.h"
#include "index.h"
#include "file.h"

/* ########### global variables ########### */


/* ########### local functions ########### */

int main(int argc, char *argv[]){
    /* Check parameters */
    if(argc != 3){
        fprintf(stderr, "Usage error: Incorrect number of arguments.\n");
        return 1;
    }

    /* Get arguments */
    char *old_index = argv[1];                   // directory produced by crawler
    char *new_index = argv[2];

    /* Check validity of arguments */
    FILE *f_in = fopen(old_index, "r");
    if(f_in == NULL){
        fprintf(stderr, "Usage error: Incorrect 1st argument.\n");
        return 1;
    }

    /* Build inverted index structure */
    hashtable_t *htable = hashtable_new(lines_in_file(f_in));
    if(htable == NULL){
        fprintf(stderr, "Error creating structures.\n");
        return 2;       
    }

    int ret = file_to_index(f_in, htable);
    if(ret != 0){
        hashtable_delete(htable, index_item_delete);
        fclose(f_in);
        return 3;
    }
    fclose(f_in);

    /* Save index into file */
    ret = index_save(new_index, htable);
    if(ret != 0){
        fprintf(stderr, "Usage error: Incorrect 2nd argument.\n");
        return 3; 
    }
    
    hashtable_delete(htable, index_item_delete);
    return 0;
}

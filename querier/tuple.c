/*
 * Author: Lizzie Hernandez Videa
 * May 19, 2020
 * Purpose: To define a new structure called tuple holding a pair of integers
 * 
 * See tuple.h for details
 * 
 * Usage: Used in querier.c to store pairs of (docID, score), which are inserted into 
 *        a querier.
 *  
 */

#include <stdbool.h>
#include "tuple.h"
#include "memory.h"

/**************** structures ****************/
typedef struct tuple {
    int key;
    int score;
} tuple_t;

/**************** tuple_new() ****************/
tuple_t *tuple_new(int key, int score){
    // allocate space for node
    tuple_t *pair = (tuple_t *)count_malloc(sizeof(tuple_t));

    if(pair!=NULL){
        // set key 
        pair->key = key;
        pair->score = score;
    }
    
    return pair;
}

/**************** get_score() ****************/
int tuple_get_score(tuple_t* pair){
    if(pair != NULL)
        return pair->score;
    return 0;
}

/**************** get_key() ****************/
int tuple_get_key(tuple_t* pair){
    if(pair != NULL)
        return pair->key;
    return 0;
}

/**************** update_score() ****************/
bool tuple_update_score(tuple_t* pair){
    if(pair != NULL){
        pair->score = pair->score + 1;
        return true;
    }
    return false;
}
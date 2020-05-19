/*
 * Author: Lizzie Hernandez Videa
 * May 17, 2020
 * Purpose: To define item functions for iterating through counters
 * 
 * See iterating.h for details
 */

#include "counters.h"
#include "iterating.h"
#include "memory.h"

/**************** local types ****************/
typedef struct counter {
    int key;
    int score;
} ctr_t;

/**************** local functions ****************/
/* not visible outside this file */
static void ctr_update_score(ctr_t* pair);

/**************** ctr_new() ****************/
ctr_t *ctr_new(int key, int score){
    // allocate space for node
    ctr_t *pair = (ctr_t *)count_malloc(sizeof(ctr_t));

    if(pair!=NULL){
        // set key 
        pair->key = key;
        pair->score = score;
    }
    
    return pair;
}

/**************** get_score() ****************/
int ctr_get_score(ctr_t* pair){
    if(pair != NULL)
        return pair->score;
    return 0;
}

/**************** get_score() ****************/
int ctr_get_key(ctr_t* pair){
    if(pair != NULL)
        return pair->key;
    return 0;
}

/**************** update_score() ****************/
static void ctr_update_score(ctr_t* pair){
    if(pair != NULL)
        pair->score = pair->score + 1;
}

/*########### counters_add_all ##########*/
/*##### See iterating.h for details #####*/
void counters_add_all(void *arg, const int key, const int count){
    counters_t* other_counter = (counters_t*)arg;
    if(count > 0){
        counters_set(other_counter, key, count);
    }
}

/*########### counters_merge ############*/
/*##### See iterating.h for details #####*/
void counters_merge(void *arg, const int key, const int count){
    counters_set((counters_t*)arg, key, count + counters_get(arg, key));
}

/*######### counters_intersect ##########*/
/*##### See iterating.h for details #####*/
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

/*############ counters_count ###########*/
/*##### See iterating.h for details #####*/
void counters_count(void *arg, const int key, const int count){
    int* size = (int *)arg;
    *size = *size + 1;
}

/*############ counters_count ###########*/
/*##### See iterating.h for details #####*/
void counters_to_array(void *arg, const int key, const int count){
    ctr_t** array = (ctr_t**)arg;
    
    /* create a new pair to be inserted */
    ctr_t* new = ctr_new(key, count);
    if(new != NULL){
        /* keep index at which it shoud be located */
        int i = ctr_get_score(array[0]);
        ctr_update_score(array[0]);

        /* move to right position using insertion sort */
        int j = i-1;
        while(j > 0 && count > ctr_get_score(array[j])){
            /* Move j to the next index */
            array[j + 1] = array[j];
            j--;
        }

        array[j+1] = new;
    }
}